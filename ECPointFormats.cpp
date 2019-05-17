#include "ECPointFormats.h"
#include "SerializationHelper.h"

ECPointFormats::ECPointFormats(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

void ECPointFormats::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeVec<ECPointFormat, unsigned char>(ECPointFormatList, Buffer, Offset);
}

void ECPointFormats::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::DeserializeVec<ECPointFormat, unsigned char>(Buffer, Offset, ECPointFormatList);
}
