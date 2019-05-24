#pragma once
#include "ITLSExtension.h"
#include <string>
#include <vector>

struct AppLayerProtocolNegotiation : public ITLSExtension
{
	AppLayerProtocolNegotiation(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() {	return ExtensionType::application_layer_protocol_negotiation;	}
	std::vector<std::string> ProtocolNameList;
};