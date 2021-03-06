#include "ClientKeyShare.h"
#include "SerializationHelper.h"

ClientKeyShare::ClientKeyShare(const std::vector<char>& Buffer, size_t & Offset, bool bIsEmpty)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void ClientKeyShare::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	unsigned short KeySharesLength = 0;

	for each (auto KeyShare in KeyShareList)
	{
		KeySharesLength += sizeof(KeyShare.Group) + sizeof(unsigned short) + KeyShare.KeyExchangeData.size();
	}

	SerializationHelper::Serialize<unsigned short>(KeySharesLength, Buffer, Offset);

	for each (auto KeyShare in KeyShareList)
	{
		SerializationHelper::Serialize<NamedGroup>(KeyShare.Group, Buffer, Offset);
		SerializationHelper::SerializeVec<unsigned char, unsigned short>(KeyShare.KeyExchangeData, Buffer, Offset);
	}
}

void ClientKeyShare::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	KeyShareList.clear();

	unsigned short KeySharesLength = SerializationHelper::Deserialize<unsigned short>(Buffer, Offset);
	
	size_t ExtensionEndOffset = Offset + KeySharesLength;

	while (Offset < ExtensionEndOffset)
	{
		KeyShareEntry KeyShare;
		KeyShare.Group = SerializationHelper::Deserialize<NamedGroup>(Buffer, Offset);
		SerializationHelper::DeserializeVec<unsigned char, unsigned short>(Buffer, Offset, KeyShare.KeyExchangeData);

		KeyShareList.push_back(KeyShare);
	}
}
