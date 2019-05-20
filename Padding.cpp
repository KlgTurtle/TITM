#include "Padding.h"

Padding::Padding(const std::vector<char>& Buffer, size_t & Offset, unsigned short _PaddingSize) : 
	PaddingSize(_PaddingSize)
{
	Deserialize(Buffer, Offset);
}

void Padding::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeFixedVec<unsigned char>(PaddingData, PaddingSize, Buffer, Offset);
}

void Padding::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	size_t OrigOffset = Offset;
	SerializationHelper::DeserializeFixedVec<unsigned char>(Buffer, Offset, PaddingData, PaddingSize);

	/*std::vector<char>& obj = const_cast<std::vector<char>&>(Buffer);
	PaddingData[5] = 'F';
	Serialize(obj, OrigOffset);*/
}
