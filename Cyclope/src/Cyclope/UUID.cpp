#include "Cpch.h"
#include "UUID.h"

#include <random>

namespace Cyclope {

	static std::random_device s_randomDevice;
	static std::mt19937_64 s_Engine(s_randomDevice());
	static std::uniform_int_distribution<uint64_t> s_uniformDist;

	UUID::UUID() 
		: m_uuid(s_uniformDist(s_Engine))
	{

	}

	UUID::UUID(uint64_t uuid)
	: m_uuid(uuid) {

	}

}