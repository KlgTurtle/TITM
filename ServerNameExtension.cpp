#include <WinSock2.h>
#include "ServerNameExtension.h"
#include "SerializationHelper.h"

ServerNameExtension::ServerNameExtension(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

void ServerNameExtension::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	unsigned short ListLength = 0;
	for each (const std::string& sName in ServerNamesList)
	{
		ListLength += sName.length() + sizeof(unsigned short) + 1;
	}

	SerializationHelper::SerializeUnsignedShort(ListLength, Buffer, Offset);

	for each (const std::string& sName in ServerNamesList)
	{
		unsigned char sNameType = 0x00;
		SerializationHelper::SerializeUnsignedChar(sNameType, Buffer, Offset);

		unsigned short SrvNameLen = sName.length();
		SerializationHelper::SerializeUnsignedShort(SrvNameLen, Buffer, Offset);	
		SerializationHelper::SerializeString(sName, Buffer, Offset);

	}
}

void ServerNameExtension::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	unsigned short ListLength = SerializationHelper::DeserializeUnsignedShort(Buffer, Offset);

	int SignedListLength = ListLength;

	while (SignedListLength > 0)
	{
		unsigned char sNameType = SerializationHelper::DeserializeUnsignedChar(Buffer, Offset);
		unsigned short SrvNameLen = SerializationHelper::DeserializeUnsignedShort(Buffer, Offset);

		std::string ServerNameStr = SerializationHelper::DeserializeString(Buffer, Offset, SrvNameLen);		
		ServerNamesList.push_back(ServerNameStr);

		SignedListLength -= (sizeof(sNameType) + sizeof(SrvNameLen) + SrvNameLen);
	}

}
