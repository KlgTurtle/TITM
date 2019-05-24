#include "RenegotiationInfo.h"

RenegotiationInfo::RenegotiationInfo(const std::vector<char>& Buffer, size_t & Offset, bool bIsEmpty)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void RenegotiationInfo::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeVec<char, unsigned char>(RenegotiatedConnection, Buffer, Offset);
}

void RenegotiationInfo::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::DeserializeVec<char, unsigned char>(Buffer, Offset, RenegotiatedConnection);
}
