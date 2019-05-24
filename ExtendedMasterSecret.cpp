#include "ExtendedMasterSecret.h"

ExtendedMasterSecret::ExtendedMasterSecret(const std::vector<char>& Buffer, size_t & Offset, 
	unsigned short _ExtLength, bool bIsEmpty) : ExtLength(_ExtLength)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void ExtendedMasterSecret::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeFixedVec<char>(Data, Data.size(), Buffer, Offset);
}

void ExtendedMasterSecret::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::DeserializeFixedVec<char>(Buffer, Offset, Data, ExtLength);
}
