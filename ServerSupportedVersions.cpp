#include "ServerSupportedVersions.h"

ServerSupportedVersions::ServerSupportedVersions(const std::vector<char>& Buffer, size_t & Offset, bool bIsEmpty)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void ServerSupportedVersions::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeStruct(reinterpret_cast<char*>(&ChosenVersion), sizeof(ChosenVersion),
		Buffer, Offset);
}

void ServerSupportedVersions::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::DeserializeStruct(Buffer, Offset,
		reinterpret_cast<char*>(&ChosenVersion), sizeof(ChosenVersion));
}
