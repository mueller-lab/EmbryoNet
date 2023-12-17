#include "AnnoVisRotationHelper.h"

ClosestKeyFrameOutput findClosestKeyFrames(
	const std::vector<std::vector<EmbryoBox>>& imagesEmbryoBoxes,
	const unsigned int frameID,
	const unsigned int embryoId)
{
	ClosestKeyFrameOutput output;
	
	const auto& embryoItCurrFrame = std::find_if(imagesEmbryoBoxes[frameID].begin(), imagesEmbryoBoxes[frameID].end(),
		[embryoId](const EmbryoBox& box)
	{
		return box.id == embryoId;
	});

	output.leftKeyFrame = output.rightKeyFrame = frameID;
	output.leftEmbryoId = output.centerEmbryoID = output.rightEmbryoId = std::distance(imagesEmbryoBoxes[frameID].begin(), embryoItCurrFrame);
	output.embryoTrackID = embryoId;
	output.centerKeyFrame = frameID;
	
	for (unsigned int leftIndex = frameID; leftIndex > 0; --leftIndex)
	{
		const auto& embryoIt = std::find_if(imagesEmbryoBoxes[leftIndex].begin(), imagesEmbryoBoxes[leftIndex].end(),
											[embryoId](const EmbryoBox& box)
										{
											return box.id == embryoId;
										});

		if (embryoIt != imagesEmbryoBoxes[leftIndex].end())
		{
			output.leftKeyFrame = leftIndex;
			output.leftEmbryoId = std::distance(imagesEmbryoBoxes[leftIndex].begin(), embryoIt);
			if (embryoIt->isKeyFrame)
			{
				break;
			}
		}
	}

	for (unsigned int rightIndex = frameID; rightIndex < imagesEmbryoBoxes.size(); ++rightIndex)
	{
		const auto& embryoIt = std::find_if(imagesEmbryoBoxes[rightIndex].begin(), imagesEmbryoBoxes[rightIndex].end(),
			[embryoId](const EmbryoBox& box)
		{
			return box.id == embryoId;
		});

		if (embryoIt != imagesEmbryoBoxes[rightIndex].end())
		{
			output.rightKeyFrame = rightIndex;
			output.rightEmbryoId = std::distance(imagesEmbryoBoxes[rightIndex].begin(), embryoIt);
			if (embryoIt->isKeyFrame)
			{
				break;
			}
		}
	}

	return output;
}

int shortestDistanceAxis(const int from, const int to)
{
	int simplePath = to - from;
	int hardPath = 360 - std::abs(simplePath);

	if (simplePath > 0)
	{
		hardPath *= -1;
	}

	if (std::abs(simplePath) > std::abs(hardPath))
	{
		return hardPath;
	}

	return simplePath;
}

QVector3D shortestDistance(const QVector3D& from, const QVector3D& to)
{
	return QVector3D(
		shortestDistanceAxis(from.x(), to.x()),
		shortestDistanceAxis(from.y(), to.y()),
		shortestDistanceAxis(from.z(), to.z())
	);
}


void setRotationsBetweenFoundKeyFrames(
	std::vector<std::vector<EmbryoBox>>& imagesEmbryoBoxes,
	const ClosestKeyFrameOutput& closestKeyFrameOutput
)
{
	QVector3D leftRot = imagesEmbryoBoxes[closestKeyFrameOutput.leftKeyFrame][closestKeyFrameOutput.leftEmbryoId].rotation;
	QVector3D centerRot = imagesEmbryoBoxes[closestKeyFrameOutput.centerKeyFrame][closestKeyFrameOutput.centerEmbryoID].rotation;
	QVector3D rightRot = imagesEmbryoBoxes[closestKeyFrameOutput.rightKeyFrame][closestKeyFrameOutput.rightEmbryoId].rotation;
	QVector3D deltaLeft = shortestDistance(leftRot, centerRot);

	//QVector3D deltaLeft = centerRot - leftRot;




	QVector3D deltaLeftStep = QVector3D{ 0,0,0 };
	QVector3D leftCurrentRotation = leftRot;

	if (closestKeyFrameOutput.centerKeyFrame - closestKeyFrameOutput.leftKeyFrame != 0)
	{
		deltaLeftStep = deltaLeft / (closestKeyFrameOutput.centerKeyFrame - closestKeyFrameOutput.leftKeyFrame);
	}

	//QVector3D deltaRight = rightRot - centerRot;
	QVector3D deltaRight = shortestDistance(centerRot, rightRot);
	QVector3D deltaRightStep = QVector3D{ 0, 0, 0 };
	QVector3D rightCurrentRotation = centerRot;

	if (closestKeyFrameOutput.rightKeyFrame - closestKeyFrameOutput.centerKeyFrame != 0)
	{
		deltaRightStep = deltaRight / (closestKeyFrameOutput.rightKeyFrame - closestKeyFrameOutput.centerKeyFrame);
	}

	for (unsigned int i = closestKeyFrameOutput.leftKeyFrame; i < closestKeyFrameOutput.centerKeyFrame; ++i)
	{ 
		
		auto embryoIt = std::find_if(imagesEmbryoBoxes[i].begin(), imagesEmbryoBoxes[i].end(),
			[&closestKeyFrameOutput](const EmbryoBox& box)
		{
			return box.id == closestKeyFrameOutput.embryoTrackID;
		});
		if (embryoIt != imagesEmbryoBoxes[i].end())
		{
			embryoIt->rotation = leftCurrentRotation;
		}
        leftCurrentRotation += deltaLeftStep;
	}

	for (unsigned int i = closestKeyFrameOutput.centerKeyFrame; i < closestKeyFrameOutput.rightKeyFrame; ++i)
	{
		auto embryoIt = std::find_if(imagesEmbryoBoxes[i].begin(), imagesEmbryoBoxes[i].end(),
			[&closestKeyFrameOutput](const EmbryoBox& box)
		{
			return box.id == closestKeyFrameOutput.embryoTrackID;
		});
		if (embryoIt != imagesEmbryoBoxes[i].end())
		{
			embryoIt->rotation = rightCurrentRotation;
		}
        rightCurrentRotation += deltaRightStep;
	}
}
