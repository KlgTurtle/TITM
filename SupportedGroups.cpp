#include "SupportedGroups.h"
#include "SerializationHelper.h"

SupportedGroups::SupportedGroups(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

void SupportedGroups::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	unsigned short SupportedGroupsListLen = SupportedGroupsList.size() * sizeof(unsigned short);
	SerializationHelper::SerializeUnsignedShort(SupportedGroupsListLen, Buffer, Offset);
	for each (unsigned short Group in SupportedGroupsList)
	{
		SerializationHelper::SerializeUnsignedShort(Group, Buffer, Offset);
	}
}

void SupportedGroups::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SupportedGroupsList.clear();

	unsigned short SupportedGroupsListLen = SerializationHelper::DeserializeUnsignedShort(Buffer, Offset);
	for (size_t i = 0; i < SupportedGroupsListLen / sizeof(unsigned short); ++i)
	{
		unsigned short Group = SerializationHelper::DeserializeUnsignedShort(Buffer, Offset);
		SupportedGroupsList.push_back(Group);
	}
}
