#include "ClientSupportedVersions.h"
#include "SerializationHelper.h"

ClientSupportedVersions::ClientSupportedVersions(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

void ClientSupportedVersions::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper<unsigned short>::SerializeVec<unsigned char>(SupportedVersionsList, Buffer, Offset);
}

void ClientSupportedVersions::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SupportedVersionsList = SerializationHelper<unsigned short>::DeserializeVec<unsigned char>(Buffer, Offset);
} 
