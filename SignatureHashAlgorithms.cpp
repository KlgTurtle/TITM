#include "SignatureHashAlgorithms.h"
#include "SerializationHelper.h"

SignatureHashAlgorithms::SignatureHashAlgorithms(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

void SignatureHashAlgorithms::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeVec<SignatureScheme, unsigned short>(SignatureSchemeList, Buffer, Offset);
}

void SignatureHashAlgorithms::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SignatureSchemeList.clear();
	SerializationHelper::DeserializeVec<SignatureScheme, unsigned short>(Buffer, Offset, SignatureSchemeList);
}
