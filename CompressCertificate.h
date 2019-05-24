#pragma once
#include "ITLSExtension.h"
#include <vector>

enum class CertificateCompressionAlgorithm : unsigned short
{
	zlib = 1,
	brotli = 2,
	zstd = 3
} ;

struct CompressCertificate : public ITLSExtension
{
	CompressCertificate(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::compress_certificate; }

	std::vector<CertificateCompressionAlgorithm> CompressionAlgorithmList;
};