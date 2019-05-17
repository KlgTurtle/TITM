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

	SerializationHelper<unsigned short>::Serialize(ListLength, Buffer, Offset);

	for each (const std::string& sName in ServerNamesList)
	{
		unsigned char sNameType = 0x00;
		SerializationHelper<unsigned char>::Serialize(sNameType, Buffer, Offset);
		SerializationHelper<unsigned char>::SerializeVec<unsigned short>(std::vector<unsigned char>(sName.begin(), sName.end()), 
			Buffer, Offset);
	}
}

void ServerNameExtension::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	unsigned short ListLength = SerializationHelper<unsigned short>::Deserialize(Buffer, Offset);

	int SignedListLength = ListLength;

	while (SignedListLength > 0)
	{
		unsigned char sNameType = SerializationHelper<unsigned char>::Deserialize(Buffer, Offset);

		std::vector<unsigned char> ServerNameStr =
			SerializationHelper<unsigned char>::DeserializeVec<unsigned short>(Buffer, Offset);
		ServerNamesList.push_back(std::string(ServerNameStr.begin(), ServerNameStr.end()));
		SignedListLength -= (sizeof(sNameType) + sizeof(unsigned short) + ServerNameStr.size());
	}

}
