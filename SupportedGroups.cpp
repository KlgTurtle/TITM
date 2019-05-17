#include "SupportedGroups.h"
#include "SerializationHelper.h"

SupportedGroups::SupportedGroups(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

void SupportedGroups::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper<unsigned short>::SerializeVec<unsigned short>(SupportedGroupsList, Buffer, Offset);
}

void SupportedGroups::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SupportedGroupsList = SerializationHelper<unsigned short>::DeserializeVec<unsigned short>(Buffer, Offset);
}

