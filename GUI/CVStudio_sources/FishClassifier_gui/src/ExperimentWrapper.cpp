//=======================================================================================================================
//Copyright
//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "ExperimentWrapper.h"

#include <QFile>
#include <QColor>
#include <QDateTime>
#include <fstream>
#include <boost/dll/runtime_symbol_info.hpp>

#include <QDir>

namespace
{

	QString generateExperimentJSON(const QString& path)
	{
		std::string program_location = boost::dll::program_location().parent_path().string();
		std::replace(program_location.begin(), program_location.end(), '\\', '/');

#if  WITH_ONNXRUNTIME
		QFile defaultConfigPath(QString::fromStdString(program_location + "/DefaultConfigGPU.json"));
#else 
		QFile defaultConfigPath(QString::fromStdString(program_location + "/DefaultConfigCPU.json"));
#endif //WITH_ONNXRUNTIME

		defaultConfigPath.open(QFile::ReadOnly);
		return QString::fromLatin1(defaultConfigPath.readAll()).arg(path, path + "/images").arg(QString::fromStdString(program_location));
	}

} // namespace

ExperimentWrapper::ExperimentWrapper(QObject* parent) : QObject(parent)
{
}

ExperimentWrapper::~ExperimentWrapper()
{
}


void ExperimentWrapper::runExperiment(const QString& path)
{
	
	try
	{
	
		const QString& jsonQString = generateExperimentJSON(path);

		std::string temp = jsonQString.toStdString();

		const nlohmann::json defaultSettings = nlohmann::json::parse(jsonQString.toStdString());

		const nlohmann::json updatedConfig = updateDefaultConfig(defaultSettings, this->additionalSettings);

		EmbryoExperimentRunner experimentRunner(updatedConfig);

		const int numFilesInFolder = experimentRunner.getPaths().size();

		std::vector <std::pair<QString, QColor>> outputClasses = loadClasses(updatedConfig["classes"]);
		emit(this->sendClasses(outputClasses));
		float currentFileIdx = 0;

		while (!experimentRunner.isInitialOver())
		{
			const auto& imagePathPair = experimentRunner.getNextImageJsonPair();
			const std::pair<cv::Mat, std::string>& image2jsonPair = experimentRunner.runStep(imagePathPair);
			if (experimentRunner.isInitialOver()){
				break;
			}

			nlohmann::json result = nlohmann::json::parse(image2jsonPair.second);
			result["classes"] = updatedConfig["classes"];
			result["labeler"] = "FishLabeler";
			result["source_type"] = "folder";
			result["version"] = "default";
			std::string resultString = result.dump(4);
			emit this->imageProcessed(imagePathPair.second, resultString);
			currentFileIdx++;
			emit this->sendProgressPercent(static_cast<int>(currentFileIdx * 100.0f / numFilesInFolder + 0.5f));

		}

		emit(this->clearAnnovis());

		currentFileIdx = 0;
		const auto& matPathes = experimentRunner.matPathes();
		for (const auto& imagePath : matPathes)
		{
			const std::pair<cv::Mat, std::string>& imgPathPairSecondRun = { cv::Mat(), imagePath };
			const std::pair <cv::Mat, std::string>& image2jsonPair = experimentRunner.runStep(imgPathPairSecondRun);
			nlohmann::json result = nlohmann::json::parse(image2jsonPair.second);
			result["classes"] = updatedConfig["classes"];
			result["labeler"] = "FishLabeler";
			result["source_type"] = "folder";
			result["version"] = "default";
			std::string resultString = result.dump(4);
			emit this->imageProcessed(imgPathPairSecondRun.second, resultString);
			currentFileIdx++;
			emit this->sendProgressPercent(static_cast<int>(currentFileIdx * 100.0f / numFilesInFolder + 0.5f));
		}


		saveExperimentConfig(defaultSettings, this->additionalSettings, path);
		emit(experimentStopped());

	}
	catch (std::exception & s) {
		emit onError(QString::fromStdString(s.what()));
	
	}

}


std::vector <std::pair<QString, QColor>> ExperimentWrapper::loadClasses(const nlohmann::json& json)
{
	std::pair <QString, QColor> outputClass;

	std::vector<std::pair<QString, QColor> > outputClasses;
	nlohmann::json classes = json;
	for (const auto& embryoClass : classes)
	{
		const std::string clsName = embryoClass["name"];
		std::vector <int> color;
		for (const auto& c : embryoClass["color"])
		{
			color.push_back(static_cast<int> (c));
		}
		QColor clsColor = QColor(color[2], color[1], color[0]);
		outputClass = { QString::fromStdString(clsName), clsColor };
		outputClasses.push_back(outputClass);
	}
	return outputClasses;
}


void ExperimentWrapper::runTestStandalone()
{
	try
	{
		QString path("F:/EMBRYOS_TO_PROCESS/EVALUATION/200629_Dorsomorphin-titration_pt2/10uM/Experiment_01/");

		const QString& jsonQString = generateExperimentJSON(path);

		std::string jsonString = jsonQString.toStdString();

		nlohmann::json json = nlohmann::json::parse(jsonString);

		EmbryoExperimentRunner experimentRunner(json);
		experimentRunner.runStandAlone();
	}
	catch (std::exception & s)
	{
		std::cout << s.what() << std::endl;
	}
}


void ExperimentWrapper::recieveUserSettings(const QString& json)
{
	additionalSettings = nlohmann::json::parse(json.toStdString());

	emit userSettingsApplied();
}

nlohmann::json ExperimentWrapper::updateDefaultConfig(const nlohmann::json& defaultSettings, const nlohmann::json& additionalSettings)
{
	if (additionalSettings.empty())
	{
		return defaultSettings;
	}

	int embryoMinSize = 0;
	int embryoMaxSize = 0;

	if (additionalSettings["MinMaxSize"].get<bool>())
	{
		embryoMinSize = additionalSettings["EmbryoMinSize"].get<int>();
		embryoMaxSize = additionalSettings["EmbryoMaxSize"].get<int>();
	}
	else
	{
		int averageDiameter = additionalSettings["AverageDiameter"].get<int>();
		int averageDiameterInPixels = averageDiameter / additionalSettings["MicrometersPerPixel"].get<int>();
		int diameterDeviation = additionalSettings["DiameterDeviation"].get<int>();

		embryoMaxSize = averageDiameterInPixels * (1 + diameterDeviation / 100.0);
		embryoMinSize = averageDiameterInPixels * (1 - diameterDeviation / 100.0);
	}

	if (embryoMaxSize < embryoMinSize || embryoMaxSize < 0 || embryoMinSize < 0)
	{
		throw std::exception("wrong min max settings");
	}

	const float embryoMeanSize = (embryoMaxSize + embryoMinSize) / 2.0f;

	const float fx = 100.0f / float(embryoMeanSize);
	const float fy = fx;
	

	nlohmann::json outputConfig = defaultSettings;
	

	outputConfig["Algorithm"]["tracker"]["detector"]["fx"] = fx;
	outputConfig["Algorithm"]["tracker"]["detector"]["fy"] = fy;

	int minRadius = int(embryoMinSize * fx / 2);
	outputConfig["Algorithm"]["tracker"]["detector"]["hough"]["minRadius"]= int(embryoMinSize*fx/2);
	outputConfig["Algorithm"]["tracker"]["detector"]["hough"]["maxRadius"] = int(embryoMaxSize*fx/2);

	outputConfig["MicroscopePattern"] = additionalSettings["MicroscopePattern"].get<std::string>();
	outputConfig["StartPoint"] = additionalSettings["StartPoint"].get<std::string>();
	outputConfig["StopPoint"] = additionalSettings["StopPoint"].get<std::string>();



	return outputConfig;
}



void ExperimentWrapper::saveExperimentConfig(const nlohmann::json& globalConfig, const nlohmann::json& additionalConfig, const QString& pathToSave)
{
	QDateTime time = QDateTime::currentDateTime();
	QString timeString = time.toString("yyyy-MM-dd_hh-mm-ss") ;

#if WITH_ONNXRUNTIME
	std::string arch = "GPU";
#else
	std::string arch = "CPU";
#endif // WITH_ONNXRUNTIME

	const QString pathToSaveWithTime = pathToSave + "/" + timeString;

	QDir workingFolder(pathToSave);
	workingFolder.mkdir(timeString);

	std::string pTs = pathToSaveWithTime.toStdString() +"/DefaultConfig" + arch + ".json" ;
	std::ofstream globalConfigFile(pTs);
	globalConfigFile << globalConfig.dump(4);

	if (!additionalSettings.empty())
	{
		std::string apTs = pathToSaveWithTime.toStdString() + "/AdditionalSettings.json";
		std::ofstream additionalConfigFile(apTs);
		additionalConfigFile << additionalSettings.dump(4);
	}

}


