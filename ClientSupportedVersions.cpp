#include "ClientSupportedVersions.h"
#include "SerializationHelper.h"

ClientSupportedVersions::ClientSupportedVersions(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

void ClientSupportedVersions::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	unsigned char SupportedVersionsListLen = SupportedVersionsList.size() * sizeof(unsigned short);
	SerializationHelper::SerializeUnsignedChar(SupportedVersionsListLen, Buffer, Offset);

	for each (unsigned short SupportedVersion in SupportedVersionsList)
	{
		SerializationHelper::SerializeUnsignedShort(SupportedVersion, Buffer, Offset);
	}
}

void ClientSupportedVersions::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	unsigned char SupportedVersionsListLen = SerializationHelper::DeserializeUnsignedChar(Buffer, Offset);

	for (size_t i = 0; i < SupportedVersionsListLen/sizeof(unsigned short); ++i)
	{
		unsigned short SupportedVersion = SerializationHelper::DeserializeUnsignedShort(Buffer, Offset);
		SupportedVersionsList.push_back(SupportedVersion);
	}
}
