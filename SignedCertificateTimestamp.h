#pragma once
#include "ITLSExtension.h"
#include <vector>

// That's the structure from the RFC of the SCT entry... but for now this is an opaque structure here.
//struct {
//	Version sct_version;
//	LogID id;
//	uint64 timestamp;
//	CtExtensions extensions;
//	digitally - signed struct {
//		Version sct_version;
//		SignatureType signature_type = certificate_timestamp;
//		uint64 timestamp;
//		LogEntryType entry_type;
//		select(entry_type) {
//                   case x509_entry: ASN.1Cert;
//				   case precert_entry: PreCert;
//		} signed_entry;
//		CtExtensions extensions;
//	};
//} SignedCertificateTimestamp;

struct SignedCertificateTimestamp : public ITLSExtension
{
	SignedCertificateTimestamp(const std::vector<char>& Buffer, size_t& Offset, unsigned short _ExtLength, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::signed_certificate_timestamp; }

	std::vector<char> OpaqueSCT;
	size_t ExtensionLength;
};