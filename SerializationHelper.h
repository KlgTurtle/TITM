#pragma once
#include <vector>
#include <string>

class SerializationHelper
{
public:
	static unsigned short DeserializeUnsignedShort(const std::vector<char>& Buffer, size_t& Offset);
	static unsigned int DeserializeUnsignedInt(const std::vector<char>& Buffer, size_t& Offset);
	static unsigned char DeserializeUnsignedChar(const std::vector<char>& Buffer, size_t& Offset);
	static std::string DeserializeString(const std::vector<char>& Buffer, size_t& Offset, const size_t& Length);

	static void SerializeUnsignedShort(unsigned short UShort, std::vector<char>& Buffer, size_t& Offset);
	static void SerializeUnsignedInt(unsigned int UInt, std::vector<char>& Buffer, size_t& Offset);
	static void SerializeUnsignedChar(unsigned char UChar, std::vector<char>& Buffer, size_t& Offset);
	static void SerializeString(const std::string& Str, std::vector<char>& Buffer, size_t& Offset);
};