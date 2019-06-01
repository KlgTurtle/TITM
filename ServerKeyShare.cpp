#include "ServerKeyShare.h"

ServerKeyShare::ServerKeyShare(const std::vector<char>& Buffer, size_t & Offset, bool bIsEmpty)
{
	Deserialize(Buffer, Offset);
}

void ServerKeyShare::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::Serialize<NamedGroup>(this->SelectedKeyShare.Group, Buffer, Offset);
	SerializationHelper::SerializeVec<unsigned char, unsigned short>(this->SelectedKeyShare.KeyExchangeData, 
																		Buffer, Offset);
}

void ServerKeyShare::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	this->SelectedKeyShare.Group = SerializationHelper::Deserialize<NamedGroup>(Buffer, Offset);
	SerializationHelper::DeserializeVec<unsigned char, unsigned short>(Buffer, Offset, this->SelectedKeyShare.KeyExchangeData);
}
