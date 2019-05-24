#include <WinSock2.h>
#include "ServerNameExtension.h"
#include "SerializationHelper.h"

ServerNameExtension::ServerNameExtension(const std::vector<char>& Buffer, size_t & Offset, bool bIsEmpty)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void ServerNameExtension::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	unsigned short ListLength = 0;
	for each (const std::string& sName in ServerNamesList)
	{
		ListLength += sName.length() + sizeof(unsigned short) + 1;
	}

	SerializationHelper::Serialize<unsigned short>(ListLength, Buffer, Offset);

	for each (const std::string& sName in ServerNamesList)
	{
		unsigned char sNameType = 0x00;
		SerializationHelper::Serialize<unsigned char>(sNameType, Buffer, Offset);
		SerializationHelper::SerializeVec<unsigned char, unsigned short>(std::vector<unsigned char>(sName.begin(), sName.end()),
			Buffer, Offset);
	}
}

void ServerNameExtension::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	unsigned short ListLength = SerializationHelper::Deserialize<unsigned short>(Buffer, Offset);

	int SignedListLength = ListLength;

	while (SignedListLength > 0)
	{
		unsigned char sNameType = SerializationHelper::Deserialize<unsigned char>(Buffer, Offset);

		std::vector<unsigned char> ServerNameStr;
		SerializationHelper::DeserializeVec<unsigned char, unsigned short>(Buffer, Offset, ServerNameStr);
		ServerNamesList.push_back(std::string(ServerNameStr.begin(), ServerNameStr.end()));
		SignedListLength -= (sizeof(sNameType) + sizeof(unsigned short) + ServerNameStr.size());
	}

}
