#include "SessionTicket.h"

SessionTicket::SessionTicket(const std::vector<char>& Buffer, size_t & Offset, unsigned short _ExtLength, bool bIsEmpty)
	: ExtensionLength(_ExtLength)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void SessionTicket::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeFixedVec<char>(TicketData, ExtensionLength, Buffer, Offset);
}

void SessionTicket::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::DeserializeFixedVec<char>(Buffer, Offset, TicketData, ExtensionLength);
}
