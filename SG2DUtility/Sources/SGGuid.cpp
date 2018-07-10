#include "../Headers/SGGuid.h"

std::unordered_map<std::string, unsigned int> SG::SGGuid::guids;

SG::SGGuid::SGGuid()
{
	auto target = guids.find("");

	if (target == guids.end())
	{
		myID = guids.size();
		guids[""] = guids.size();
	}
	else
	{
		myID = guids[""];
	}
}

SG::SGGuid::SGGuid(std::string identifier)
{
	auto target = guids.find(identifier);

	if (target == guids.end())
	{
		myID = guids.size();
		guids[identifier] = guids.size();
	}
	else
	{
		myID = guids[identifier];
	}
}

SG::SGGuid::~SGGuid()
{
}

bool SG::SGGuid::operator==(const SGGuid & other) const
{
	return myID == other.myID;
}

std::size_t SG::SGGuid::operator()(const SGGuid & other) const
{
	return myID;
}
