#include "UnknownExtension.h"

UnknownExtension::UnknownExtension(const std::vector<char>& Buffer, size_t & Offset, 
	ExtensionType _ExtType, unsigned short _ExtLength, bool bIsEmpty) :
	ExtType(_ExtType), ExtLength(_ExtLength)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void UnknownExtension::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeFixedVec<char>(ExtensionData, ExtLength, Buffer, Offset);
}

void UnknownExtension::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::DeserializeFixedVec<char>(Buffer, Offset, ExtensionData, ExtLength);
}
