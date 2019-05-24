#include "CompressCertificate.h"

CompressCertificate::CompressCertificate(const std::vector<char>& Buffer, size_t & Offset, bool bIsEmpty)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void CompressCertificate::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeVec<CertificateCompressionAlgorithm, unsigned char>
		(CompressionAlgorithmList, Buffer, Offset);
}

void CompressCertificate::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::DeserializeVec<CertificateCompressionAlgorithm, unsigned char>
														(Buffer, Offset, CompressionAlgorithmList);
}


