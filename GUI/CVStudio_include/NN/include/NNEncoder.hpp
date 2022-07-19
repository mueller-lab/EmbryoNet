#pragma once

#include "Encoder.hpp"
#include "NetManager.hpp"

class NNEncoder: public Encoder
{
	std::shared_ptr<NetManager> m_netManager;

public:

	NNEncoder(std::shared_ptr<NetManager> netManager);
	~NNEncoder();

	void encode(const CVUtils::Image &input);
};
