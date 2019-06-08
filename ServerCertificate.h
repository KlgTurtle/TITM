#pragma once
#include "ITLSHandshakeMessage.h"

//enum {
//	X509(0),
//	RawPublicKey(2),
//	(255)
//} CertificateType;
//
//struct {
//	select(certificate_type) {
//              case RawPublicKey:
//				  /* From RFC 7250 ASN.1_subjectPublicKeyInfo */
//				  opaque ASN1_subjectPublicKeyInfo<1..2 ^ 24 - 1>;
//
//			  case X509:
//				  opaque cert_data<1..2 ^ 24 - 1>;
//	};
//	Extension extensions<0..2 ^ 16 - 1>;
//} CertificateEntry;
//
//struct {
//	opaque certificate_request_context<0..2 ^ 8 - 1>;
//	CertificateEntry certificate_list<0..2 ^ 24 - 1>;
//} Certificate;


struct CertificateEntry
{
	std::vector<char> cert_data;
	std::vector<std::shared_ptr<ITLSExtension>> extensions;
};

struct ServerCertificate : public ITLSHandshakeMessage
{
public:
	ServerCertificate(const std::vector<char>& Buffer, size_t& Offset);

	virtual std::string ToString();

	virtual HandshakeType GetHandshakeType() { return HandshakeType::certificate; }

	std::vector<std::vector<char>> Certificates;

protected:
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
};