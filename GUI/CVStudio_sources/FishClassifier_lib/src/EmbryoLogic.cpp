#include "EmbryoLogic.h"

#include <set>

EmbryoLogic::EmbryoLogic(const nlohmann::json& config)
{
	int current_id = 0;
	for (const auto& classes : config["classes"])
	{
		m_classesStrIdMap[classes["name"].get<std::string>()] = classes["id"].get<int>();

		
		m_classesStr_globalIdMap[classes["name"].get<std::string>()]= current_id;

		if (!classes.count("severities"))
		{
			current_id++;
		}
		else
		{
			current_id += classes["severities"].get<std::vector<int>>().size();
		}
		// эта мапа не учитывает северити. поэтому аноун 3  а не 4+4+1+1 = 10 

	}

	for (const auto& transactionList : config["transactions"])
	{
		std::vector<size_t> modelStates;
		for (const auto& transactionJson : transactionList)
		{
			const std::string& transatction = transactionJson;
			modelStates.push_back(m_classesStrIdMap.at(transatction));
		}
		m_avaliableModels.push_back(modelStates);
	}
	
}

EmbryoLogic::~EmbryoLogic()
{
}

int EmbryoLogic::getClassIdFromName(const std::string& className) const
{
	if (m_classesStr_globalIdMap.find(className) != m_classesStr_globalIdMap.end()) {

		return m_classesStr_globalIdMap.at(className);
	}
	else
	{
		return 0;
	}
}

void EmbryoLogic::feed(const FrameShot& frameShot)
{
	m_inputFrameShots.push_back(frameShot);

	for (unsigned int i = 0; i < frameShot.embryoDatas.size(); i++)
	{
		EmbryoShot es;
		es.trackID = frameShot.embryoDatas[i].trackID;
		es.classificationResult = frameShot.embryoDatas[i].classificationResult;
		es.frameId = frameShot.frameId;
		es.classID = std::distance(es.classificationResult.begin(), std::max_element(es.classificationResult.begin(), es.classificationResult.end()));
		es.bbox = frameShot.embryoDatas[i].bbox;
		es.severity = frameShot.embryoDatas[i].severity;

		m_inputEmbryoShots[es.trackID].push_back(es);
	}
}

ModelWithBordersAndQuality EmbryoLogic::findBestModel(const std::vector<EmbryoShot>& embryoShots)
{
	std::vector<ModelWithBordersAndQuality> allModelsWithBordersAndQualitySorted;

	for (const auto& currModel : m_avaliableModels)
	{
		allModelsWithBordersAndQualitySorted.push_back(calculateQuality(embryoShots, currModel));
	}

	std::sort(allModelsWithBordersAndQualitySorted.begin(),
		allModelsWithBordersAndQualitySorted.end());

	return *(allModelsWithBordersAndQualitySorted.rbegin());
}



ModelWithBordersAndQuality EmbryoLogic::calculateQuality(const std::vector<EmbryoShot>& embryoShots, const std::vector<size_t>& model)
{
	std::vector<unsigned int> borders(model.size() - 1);

	if (borders.empty())
	{
		throw std::runtime_error("Minimal number of supported states in model is 2");
	}

	for (size_t i = 0; i < borders.size(); ++i)
	{
		const auto startFrame = (i > 0 ? borders[i - 1] : 0);
		const auto endFrame = embryoShots.size();

		std::vector<int> qualityVec;

		int matchCount = 0;
		int missCount = 0;

		for (size_t frameIdx = startFrame; frameIdx < endFrame; frameIdx++)
		{
			const auto& expectedState = static_cast<decltype(embryoShots[frameIdx].classID)>(model[i]);
			const auto& actualState = embryoShots[frameIdx].classID;

			if (actualState == expectedState)
			{
				++matchCount;
			}
			else
			{
				++missCount;
			}

			qualityVec.push_back(matchCount - missCount);

		}

		if (qualityVec.empty())
		{
			borders[i] = borders[i - 1];
		}
		else
		{
			borders[i] = startFrame + std::distance(qualityVec.begin(), std::max_element(qualityVec.begin(), qualityVec.end()));
		}
	}

	int quality = 0;
	int stateIdx = 0;

	for (unsigned int frameIdx = 0; frameIdx < embryoShots.size(); ++frameIdx)
	{

		for (size_t i = 0; i < borders.size(); ++i)
		{
			if (frameIdx == borders[i])
			{
				++stateIdx;
			}
		}

		const auto& expectedState = static_cast<decltype(embryoShots[frameIdx].classID)>(model[stateIdx]);
		const auto& actualState = embryoShots[frameIdx].classID;
		if (actualState == expectedState)
		{
			++quality;
		}
		else
		{
			--quality;
		}
	}

	std::vector<size_t> modelCopy;
	for (const auto& embryoState : model)
	{
		modelCopy.push_back(embryoState);
	}


	for (auto it = borders.begin(); it != borders.end();)
	{
		if (*it == 0)
		{
			it = borders.erase(it);
			modelCopy.erase(modelCopy.begin());
		}
		else if (*it == embryoShots.size() - 1)
		{
			it = borders.erase(it);
			modelCopy.erase(modelCopy.end() - 1);
		}
		else
		{
			++it;
		}
	}


	return { modelCopy, borders, quality };
}

size_t EmbryoLogic::specifySeverity(size_t classId, const SeverityMap& severityMap) const
{
	if (!severityMap.count(classId)) //if track is very short or if class doesn't have any severity states: return 100;
	{
		return 100;
	}

	size_t maxCount = 0;
	size_t currentSeverity = 0;
	const auto severitiesPerClass = severityMap.at(classId);
	for (const auto severity: severitiesPerClass)
	{
		if (severity.second > maxCount)
		{
			currentSeverity = severity.first;
			maxCount = severity.second;
		}
	}

	return currentSeverity;
}

std::map<unsigned int, std::map<size_t, size_t> > EmbryoLogic::getConcentrationsPerClass(const std::vector<EmbryoShot>& trackShots, const std::vector<int>& ignoreFrames)
{
	SeverityMap sm;
	for (size_t i = 0; i < trackShots.size(); ++i)
	{
		if (std::find(ignoreFrames.begin(), ignoreFrames.end(), i) != ignoreFrames.end())
		{
			continue;
		}

		const auto& embryoShot = trackShots[i];
		if (sm.count(embryoShot.classID) && sm[embryoShot.classID].count(embryoShot.severity))
		{
			sm[embryoShot.classID][embryoShot.severity] += 1;
		}
		else
		{
			sm[embryoShot.classID][embryoShot.severity] = 1;
		}

	}
	return sm;
}

std::vector<EmbryoShot> EmbryoLogic::overrideStates(const std::vector<EmbryoShot>& embryoShots, const ModelWithBordersAndQuality& modelWithBorders)
{
	std::vector<EmbryoShot> outputEmbryoShots = embryoShots;
	std::vector<int> concentrationIgnoreFrames;

	const std::vector<unsigned int> borders = modelWithBorders.borders;
	int currentFrame = 0;
	for (size_t stateIdx = 0; stateIdx < modelWithBorders.model.size(); ++stateIdx)
	{
		const size_t correctState = modelWithBorders.model[stateIdx];

		const int endFrame = ((stateIdx < borders.size()) ? borders[stateIdx] : embryoShots.size());

		while (currentFrame < endFrame)
		{
			const auto oldId = embryoShots[currentFrame].classID;
			const auto newId = static_cast<decltype(embryoShots[currentFrame].classID)>(correctState);

			if (oldId != newId)
			{
				concentrationIgnoreFrames.push_back(currentFrame);
			}

			outputEmbryoShots[currentFrame].classID = newId;
			
    		std::vector<float> newClassificationResult(outputEmbryoShots[currentFrame].classificationResult.size(), .0f);
			newClassificationResult[outputEmbryoShots[currentFrame].classID] = 1.0f;
			outputEmbryoShots[currentFrame].classificationResult = newClassificationResult;
			++currentFrame;

		}
	}

	const SeverityMap concentrationsPerClass = getConcentrationsPerClass(embryoShots, concentrationIgnoreFrames);

	currentFrame = 0;
	for (size_t stateIdx = 0; stateIdx < modelWithBorders.model.size(); ++stateIdx)
	{
		const size_t correctState = modelWithBorders.model[stateIdx];

		const int endFrame = ((stateIdx < borders.size()) ? borders[stateIdx] : embryoShots.size());

		while (currentFrame < endFrame)
		{
			const auto newId = static_cast<decltype(embryoShots[currentFrame].classID)>(correctState);
			int severityBefore = outputEmbryoShots[currentFrame].severity;

			outputEmbryoShots[currentFrame].severity = specifySeverity(newId, concentrationsPerClass);
			int severityAfter = outputEmbryoShots[currentFrame].severity;

			++currentFrame;
		}
	}

	return outputEmbryoShots;
}

void EmbryoLogic::runAnalysis()
{
	for (const auto& inputEmbryoShotsEntry : m_inputEmbryoShots)
	{
		const auto trackId = inputEmbryoShotsEntry.first;

		const auto embryoTrack = inputEmbryoShotsEntry.second;

		const ModelWithBordersAndQuality outputModel = findBestModel(embryoTrack);

		m_outputEmbryoShots[trackId] = overrideStates(embryoTrack, outputModel);

	}
}

std::vector<FrameShot> EmbryoLogic::getFilteredDataByModels()
{
	m_outputFrameShots = m_inputFrameShots;

	runAnalysis();

	for (size_t frameId = 0; frameId < m_outputFrameShots.size(); ++frameId)
	{
		for (const auto& m_outputEmbryoShot : m_outputEmbryoShots)
		{
			const auto embryoTrack = m_outputEmbryoShot.second;
			for (const auto& embryoShot : embryoTrack)
			{
				if (embryoShot.frameId == frameId)
				{
					for (size_t index = 0; index < m_outputFrameShots[frameId].embryoDatas.size(); index++)
					{
						const auto trackID = m_outputFrameShots[frameId].embryoDatas[index].trackID;
						if (trackID == embryoShot.trackID)
						{
							m_outputFrameShots[frameId].embryoDatas[index].classificationResult = embryoShot.classificationResult;
							m_outputFrameShots[frameId].embryoDatas[index].severity = embryoShot.severity;
							m_outputFrameShots[frameId].embryoDatas[index].classID = embryoShot.classID;
						}
					}

				}
			}

		}
	}

	return m_outputFrameShots;
}
