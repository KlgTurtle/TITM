#pragma once
#include "ITLSExtension.h"
#include <vector>


struct SessionTicket : public ITLSExtension
{
	SessionTicket(const std::vector<char>& Buffer, size_t& Offset, unsigned short _ExtLength, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::session_ticket; }

	size_t ExtensionLength;
	std::vector<char> TicketData;
};