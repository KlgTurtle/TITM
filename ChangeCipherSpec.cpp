#include "ChangeCipherSpec.h"

ChangeCipherSpec::ChangeCipherSpec(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

std::string ChangeCipherSpec::ToString()
{
	return std::string();
}

void ChangeCipherSpec::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializePlaintextHeader(Buffer, Offset);
	SerializationHelper::Serialize<unsigned char>(ChangeCipherSpecMsg, Buffer, Offset);
}

void ChangeCipherSpec::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	DeserializePlaintextHeader(Buffer, Offset);
	ChangeCipherSpecMsg = SerializationHelper::Deserialize<unsigned char>(Buffer, Offset);
}


