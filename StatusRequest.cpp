#include "StatusRequest.h"
#include "SerializationHelper.h"

StatusRequest::StatusRequest(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

void StatusRequest::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::Serialize<CertificateStatusType>(StatusType, Buffer, Offset);
	SerializationHelper::SerializeVec<unsigned char, unsigned short>(ResponderIDList, Buffer, Offset);
	SerializationHelper::SerializeVec<unsigned char, unsigned short>(RequestExtensions, Buffer, Offset);
}

void StatusRequest::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	StatusType = SerializationHelper::Deserialize<CertificateStatusType>(Buffer, Offset);

	ResponderIDList.clear();
	RequestExtensions.clear();

	SerializationHelper::DeserializeVec<unsigned char, unsigned short>(Buffer, Offset, ResponderIDList);
	SerializationHelper::DeserializeVec<unsigned char, unsigned short>(Buffer, Offset, RequestExtensions);
}
