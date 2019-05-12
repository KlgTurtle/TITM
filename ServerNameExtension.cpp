#include <WinSock2.h>
#include "ServerNameExtension.h"

ServerNameExtension::ServerNameExtension(const std::vector<char>& Buffer, size_t & Offset)
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

		SignedListLength -= (1 + sizeof(SrvNameLen) + SrvNameLen);
	}
	
}
