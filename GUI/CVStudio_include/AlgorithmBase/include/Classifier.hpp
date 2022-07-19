#include "SingleImageAlgorithm.hpp"

class Classifier : public SingleImageAlgorithm
{

protected:

	std::vector<float> m_features;

public:

	Classifier(const std::string &name, const std::string type, const std::string &engine);
	virtual ~Classifier();

	void run(const CVUtils::Image &input);

	virtual void classify(const CVUtils::Image &input) = 0;

	std::vector<float> getClassifications() const;
};