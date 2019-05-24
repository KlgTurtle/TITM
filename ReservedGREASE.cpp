#include "ReservedGREASE.h"

ReservedGREASE::ReservedGREASE(const std::vector<char>& Buffer, size_t & Offset, 
	ExtensionType _GREASEValue, unsigned short _GREASELength, bool bIsEmpty) :
	GREASEValue(_GREASEValue), GREASELength(_GREASELength)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void ReservedGREASE::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeFixedVec<char>(GREASEData, GREASEData.size(), Buffer, Offset);
}

void ReservedGREASE::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::DeserializeFixedVec<char>(Buffer, Offset, GREASEData, GREASELength);
}
