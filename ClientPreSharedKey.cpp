#include "ClientPreSharedKey.h"

ClientPreSharedKey::ClientPreSharedKey(const std::vector<char>& Buffer, size_t & Offset, bool bIsEmpty)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void ClientPreSharedKey::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	// Leave space for the total identities length
	size_t IdentitiesLengthOffset = Offset;
	Offset += sizeof(unsigned short);

	for each (auto CurrIdentity in offeredPsks.identities)
	{
		SerializationHelper::SerializeVec<char, unsigned short>(CurrIdentity.identity, Buffer, Offset);
		SerializationHelper::Serialize<unsigned int>(CurrIdentity.obfuscated_ticket_age, Buffer, Offset);
	}

	// Now go back and update the total identities length we left out at the start
	unsigned short IdentitiesLength = Offset - IdentitiesLengthOffset - sizeof(unsigned short);
	SerializationHelper::Serialize<unsigned short>(IdentitiesLength, Buffer, IdentitiesLengthOffset);

	SerializationHelper::SerializeVec<char, unsigned short>(offeredPsks.binders, Buffer, Offset);
}

void ClientPreSharedKey::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	offeredPsks.identities.clear();
	offeredPsks.binders.clear();

	unsigned short IdentitiesLength = SerializationHelper::Deserialize<unsigned short>(Buffer, Offset);
	size_t IdentitiesEndOffset = Offset + IdentitiesLength;

	while (Offset < IdentitiesEndOffset)
	{
		PskIdentity CurrIdentity;

		SerializationHelper::DeserializeVec<char, unsigned short>(Buffer, Offset, CurrIdentity.identity);
		CurrIdentity.obfuscated_ticket_age = SerializationHelper::Deserialize<unsigned int>(Buffer, Offset);
		offeredPsks.identities.push_back(CurrIdentity);
	}

	SerializationHelper::DeserializeVec<char, unsigned short>(Buffer, Offset, offeredPsks.binders);
}
