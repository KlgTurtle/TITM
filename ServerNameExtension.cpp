#include <WinSock2.h>
#include "ServerNameExtension.h"

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

	unsigned short ListLengthBE = htons(ListLength);
	memcpy(&Buffer[Offset], &ListLengthBE, sizeof(ListLengthBE));
	Offset += sizeof(ListLengthBE);

	for each (const std::string& sName in ServerNamesList)
	{
		unsigned char sNameType = 0x00;
		memcpy(&Buffer[Offset], &sNameType, sizeof(sNameType));
		Offset += sizeof(sNameType);

		unsigned short SrvNameLen = sName.length();
		unsigned short SrvNameLenBE = htons(SrvNameLen);
		memcpy(&Buffer[Offset], &SrvNameLenBE, sizeof(SrvNameLenBE));
		Offset += sizeof(SrvNameLenBE);

		memcpy(&Buffer[Offset], sName.data(), SrvNameLen);
		Offset += SrvNameLen;
	}
}

void ServerNameExtension::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	const unsigned short* ListLengthBE = reinterpret_cast<const unsigned short*>(&Buffer[Offset]);
	unsigned short ListLength = ntohs(*ListLengthBE);
	Offset += sizeof(ListLength);

	int SignedListLength = ListLength;

	while (SignedListLength > 0)
	{
		Offset += 1;  // Skip type, its FFU - currently only host_name is supported.
		const unsigned short* SrvNameLenBE = reinterpret_cast<const unsigned short*>(&Buffer[Offset]);
		unsigned short SrvNameLen = ntohs(*SrvNameLenBE);
		Offset += sizeof(SrvNameLen);

		std::string ServerNameStr(&Buffer[Offset], &Buffer[Offset + SrvNameLen]);
		ServerNamesList.push_back(ServerNameStr);

		Offset += SrvNameLen;
		SignedListLength -= (1 + sizeof(SrvNameLen) + SrvNameLen);
	}

	std::vector<char> buffy;
	buffy.resize(1000);
	size_t a = 0;
	Serialize(buffy, a);


}
