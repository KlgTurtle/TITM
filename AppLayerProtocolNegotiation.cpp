#include "AppLayerProtocolNegotiation.h"
#include "SerializationHelper.h"

AppLayerProtocolNegotiation::AppLayerProtocolNegotiation(const std::vector<char>& Buffer, size_t & Offset, bool bIsEmpty)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void AppLayerProtocolNegotiation::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	unsigned short ListLength = 0;
	for each (auto ProtocolName in ProtocolNameList)
	{
		ListLength += ProtocolName.length() + sizeof(unsigned char);
	}

	SerializationHelper::Serialize<unsigned short>(ListLength, Buffer, Offset);

	for each (auto ProtocolName in ProtocolNameList)
	{
		std::vector<unsigned char> ProtocolNameVec(ProtocolName.begin(), ProtocolName.end());
		SerializationHelper::SerializeVec<unsigned char, unsigned char>(ProtocolNameVec, Buffer, Offset);
	}
}

void AppLayerProtocolNegotiation::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	ProtocolNameList.clear();

	unsigned short ListLength = SerializationHelper::Deserialize<unsigned short>(Buffer, Offset);

	while (ListLength > 0)
	{
		std::vector<unsigned char> ProtocolNameVec;
		SerializationHelper::DeserializeVec<unsigned char, unsigned char>(Buffer, Offset, ProtocolNameVec);
		ProtocolNameList.push_back(std::string(ProtocolNameVec.begin(), ProtocolNameVec.end()));
		ListLength -= (ProtocolNameVec.size() + sizeof(unsigned char));
	}
}
