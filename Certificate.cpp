#include "Certificate.h"

Certificate::Certificate(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

std::string Certificate::ToString()
{
	return std::string();
}

void Certificate::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	DeserializePlaintextHeader(Buffer, Offset);
	DeserializeHandshakeHeader(Buffer, Offset);


}

void Certificate::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
}
