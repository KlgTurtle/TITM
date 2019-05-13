#include <WinSock2.h>
#include "SerializationHelper.h"

unsigned short SerializationHelper::DeserializeUnsignedShort(const std::vector<char>& Buffer, size_t & Offset)
{
	const unsigned short* UShortBE = reinterpret_cast<const unsigned short*>(&Buffer[Offset]);
	unsigned short UShort = ntohs(*UShortBE);
	Offset += sizeof(UShort);

	return UShort;
}

unsigned int SerializationHelper::DeserializeUnsignedInt(const std::vector<char>& Buffer, size_t & Offset)
{
	const unsigned int* UIntBE = reinterpret_cast<const unsigned int*>(&Buffer[Offset]);
	unsigned int UInt = ntohs(*UIntBE);
	Offset += sizeof(UInt);

	return UInt;
}

unsigned char SerializationHelper::DeserializeUnsignedChar(const std::vector<char>& Buffer, size_t & Offset)
{
	unsigned char UChar = Buffer[Offset];
	Offset += sizeof(UChar);
	return UChar;
}

std::string SerializationHelper::DeserializeString(const std::vector<char>& Buffer, size_t & Offset, const size_t & Length)
{
	std::string Str(&Buffer[Offset], &Buffer[Offset + Length]);
	Offset += Length;
	return Str;
}

void SerializationHelper::SerializeUnsignedShort(unsigned short UShort, std::vector<char>& Buffer, size_t & Offset)
{
	unsigned short UShortBE = htons(UShort);
	memcpy(&Buffer[Offset], &UShortBE, sizeof(UShortBE));
	Offset += sizeof(UShortBE);
}

void SerializationHelper::SerializeUnsignedInt(unsigned int UInt, std::vector<char>& Buffer, size_t & Offset)
{
	unsigned int UIntBE = htons(UInt);
	memcpy(&Buffer[Offset], &UIntBE, sizeof(UIntBE));
	Offset += sizeof(UIntBE);
}

void SerializationHelper::SerializeUnsignedChar(unsigned char UChar, std::vector<char>& Buffer, size_t & Offset)
{
	Buffer[Offset] = UChar;
	Offset += sizeof(UChar);
}

void SerializationHelper::SerializeString(const std::string & Str, std::vector<char>& Buffer, size_t & Offset)
{
	std::memcpy(&Buffer[Offset], Str.data(), Str.length());
	Offset += Str.length();
}
