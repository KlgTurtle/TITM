#pragma once
#include "ITLSExtension.h"
#include <vector>

enum class CertificateStatusType : unsigned char
{
	ocsp = 1
};

struct StatusRequest : public ITLSExtension
{
	StatusRequest(const std::vector<char>& Buffer, size_t& Offset);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);

	CertificateStatusType StatusType;
	std::vector<unsigned char> ResponderIDList;
	std::vector<unsigned char> RequestExtensions;
};