#include "SupportedGroups.h"
#include "SerializationHelper.h"

SupportedGroups::SupportedGroups(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

void SupportedGroups::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeVec<NamedGroup, unsigned short>(SupportedGroupsList, Buffer, Offset);
}

void SupportedGroups::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
    SerializationHelper::DeserializeVec<NamedGroup, unsigned short>(Buffer, Offset, SupportedGroupsList);
}

