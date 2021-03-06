#pragma once
#include "ITLSExtension.h"
#include <string>
#include <vector>

enum class SignatureScheme : unsigned short 
{
	/* RSASSA-PKCS1-v1_5 algorithms */
	rsa_pkcs1_sha256 = 0x0401,
	rsa_pkcs1_sha384 = 0x0501,
	rsa_pkcs1_sha512 = 0x0601,

	/* ECDSA algorithms */
	ecdsa_secp256r1_sha256 = 0x0403,
	ecdsa_secp384r1_sha384 = 0x0503,
	ecdsa_secp521r1_sha512 = 0x0603,

	/* RSASSA-PSS algorithms with public key OID rsaEncryption */
	rsa_pss_rsae_sha256 = 0x0804,
	rsa_pss_rsae_sha384 = 0x0805,
	rsa_pss_rsae_sha512 = 0x0806,

	/* EdDSA algorithms */
	ed25519 = 0x0807,
	ed448 = 0x0808,

	/* RSASSA-PSS algorithms with public key OID RSASSA-PSS */
	rsa_pss_pss_sha256 = 0x0809,
	rsa_pss_pss_sha384 = 0x080a,
	rsa_pss_pss_sha512 = 0x080b,

	/* Legacy algorithms */
	rsa_pkcs1_sha1 = 0x0201,
	ecdsa_sha1 = 0x0203,


} ;

struct SignatureHashAlgorithms : public ITLSExtension
{
	SignatureHashAlgorithms(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::signature_algorithms; }

	std::vector<SignatureScheme> SignatureSchemeList;
};