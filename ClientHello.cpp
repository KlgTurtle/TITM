#include <WinSock2.h>
#include "TLS.h"
#include "ServerNameExtension.h"
#include "SupportedGroups.h"
#include "ClientHello.h"
#include "SerializationHelper.h"
#include "ClientSupportedVersions.h"
#include "ECPointFormats.h"
#include "AppLayerProtocolNegotiation.h"
#include "StatusRequest.h"
#include "SignatureHashAlgorithms.h"
#include "KeyShare.h"
#include "PskKeyExchangeModes.h"
#include "CompressCertificate.h"
#include "Padding.h"
#include "ReservedGREASE.h"
#include "ExtendedMasterSecret.h"
#include "RenegotiationInfo.h"
#include "SessionTicket.h"
#include "SignedCertificateTimestamp.h"
#include "UnknownExtension.h"

ClientHello::ClientHello(const std::vector<char>& Buffer, size_t& Offset)
{
	Deserialize(Buffer, Offset);
}

std::string ClientHello::ToString()
{
	std::string TLSHeader = ITLSMessage::ToString();

	return "";
}

void ClientHello::Deserialize(const std::vector<char>& Buffer, size_t& Offset)
{
	DeserializePlaintextHeader(Buffer, Offset);
	DeserializeHandshakeHeader(Buffer, Offset);

	SerializationHelper::DeserializeStruct(Buffer, Offset, reinterpret_cast<char*>(&CHHeader), sizeof(CHHeader));
	SerializationHelper::DeserializeVec<unsigned char, unsigned char>(Buffer, Offset, this->session_id);
	SerializationHelper::DeserializeVec<unsigned short, unsigned short>(Buffer, Offset, this->cipher_suites);
	SerializationHelper::DeserializeVec<unsigned char, unsigned char>(Buffer, Offset, this->compression_methods);

	GetExtensions(Buffer, Offset);

	//std::vector<char> stambuf;
	//stambuf.resize(Buffer.size() - sizeof(TLSPlaintextHeader) - sizeof(HandshakeHeader));
	//size_t offy = 0;
	//Serialize(stambuf, offy);

	//size_t offy1 = 0;
	//extensions.clear();
	//ClientHello other;
	//other.Deserialize(stambuf, offy1);
	//int z = 1;
}


void ClientHello::GetExtensions(const std::vector<char>& Buffer, size_t& Offset)
{
	const unsigned short* ExtensionsLengthBE = reinterpret_cast<const unsigned short*>(&Buffer[Offset]);
	unsigned short ExtensionsLength1 = ntohs(*ExtensionsLengthBE);
	int ExtensionsLength = ExtensionsLength1; // just so it is signed.

	Offset += sizeof(ExtensionsLength1);

	while (ExtensionsLength > 0)
	{
		ExtensionType ExtType =
			static_cast<ExtensionType>(ntohs(*reinterpret_cast<const unsigned short*>(&Buffer[Offset])));
		Offset += sizeof(ExtensionType);

		const unsigned short* ExtLengthBE = reinterpret_cast<const unsigned short*>(&Buffer[Offset]);
		unsigned short ExtLength = ntohs(*ExtLengthBE);
		Offset += sizeof(ExtLength);
		bool bIsExtEmpty = (ExtLength == 0);

		// Handle extension here
		switch (ExtType)
		{
		case ExtensionType::server_name:
			extensions.push_back(std::make_shared<ServerNameExtension>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::supported_groups:
			extensions.push_back(std::make_shared<SupportedGroups>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::supported_versions:
			extensions.push_back(std::make_shared<ClientSupportedVersions>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::ec_points_format:
			extensions.push_back(std::make_shared<ECPointFormats>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::application_layer_protocol_negotiation:
			extensions.push_back(std::make_shared<AppLayerProtocolNegotiation>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::status_request:
			extensions.push_back(std::make_shared<StatusRequest>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::signature_algorithms:
			extensions.push_back(std::make_shared<SignatureHashAlgorithms>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::key_share:
			extensions.push_back(std::make_shared<KeyShare>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::psk_key_exchange_modes:
			extensions.push_back(std::make_shared<PskKeyExchangeModes>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::compress_certificate:
			extensions.push_back(std::make_shared<CompressCertificate>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::padding:
			extensions.push_back(std::make_shared<Padding>(Buffer, Offset, ExtLength, bIsExtEmpty));
			break;
		case ExtensionType::extended_master_secret:
			extensions.push_back(std::make_shared<ExtendedMasterSecret>(Buffer, Offset, ExtLength, bIsExtEmpty));
			break;
		case ExtensionType::renegotiation_info:
			extensions.push_back(std::make_shared<RenegotiationInfo>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::session_ticket:
			extensions.push_back(std::make_shared<SessionTicket>(Buffer, Offset, ExtLength, bIsExtEmpty));
			break;
		case ExtensionType::signed_certificate_timestamp:
			extensions.push_back(std::make_shared<SignedCertificateTimestamp>(Buffer, Offset, ExtLength, bIsExtEmpty));
			break;
		case ExtensionType::GREASE_Reserved1:
		case ExtensionType::GREASE_Reserved2:
		case ExtensionType::GREASE_Reserved3:
		case ExtensionType::GREASE_Reserved4:
		case ExtensionType::GREASE_Reserved5:
		case ExtensionType::GREASE_Reserved6:
		case ExtensionType::GREASE_Reserved7:
		case ExtensionType::GREASE_Reserved8:
		case ExtensionType::GREASE_Reserved9:
		case ExtensionType::GREASE_Reserved10:
		case ExtensionType::GREASE_Reserved11:
		case ExtensionType::GREASE_Reserved12:
		case ExtensionType::GREASE_Reserved13:
		case ExtensionType::GREASE_Reserved14:
		case ExtensionType::GREASE_Reserved15:
		case ExtensionType::GREASE_Reserved16:
			extensions.push_back(std::make_shared<ReservedGREASE>(Buffer, Offset, ExtType, ExtLength, bIsExtEmpty));
			break;
		case ExtensionType::max_fragment_length:
		case ExtensionType::use_srtp:
		case ExtensionType::heartbeat:
		case ExtensionType::client_certificate_type:
		case ExtensionType::server_certificate_type:
		case ExtensionType::pre_shared_key:
		case ExtensionType::early_data:
		case ExtensionType::cookie:
		case ExtensionType::certificate_authorities:
		case ExtensionType::oid_filters:
		case ExtensionType::post_handshake_auth:
		case ExtensionType::signature_algorithms_cert:
		default:
			extensions.push_back(std::make_shared<UnknownExtension>(Buffer, Offset, ExtType, ExtLength, bIsExtEmpty));
			break;

		}

		
		ExtensionsLength -= (ExtLength + sizeof(ExtensionType) + sizeof(ExtLength));
	}
}


void ClientHello::Serialize(std::vector<char>& Buffer, size_t& Offset)
{
	SerializationHelper::SerializeStruct(reinterpret_cast<char*>(&CHHeader), sizeof(CHHeader), Buffer, Offset);
	SerializationHelper::SerializeVec<unsigned char, unsigned char>(this->session_id, Buffer, Offset);
	SerializationHelper::SerializeVec<unsigned short, unsigned short>(this->cipher_suites, Buffer, Offset);
	SerializationHelper::SerializeVec<unsigned char, unsigned char>(this->compression_methods, Buffer, Offset);

	// Leave space for total extensions length - we'll know it later.
	size_t ExtStartOffset = Offset;
	Offset += sizeof(unsigned short);

	for (size_t i = 0; i < extensions.size(); ++i)
	{
		SerializationHelper::Serialize<ExtensionType>(extensions[i]->GetType(), Buffer, Offset);
		size_t ExtLenOffset = Offset;

		// Leave space for this specific extension length
		Offset += sizeof(unsigned short);

		extensions[i]->Serialize(Buffer, Offset);

		// Update the two bytes we left earlier with the total extension length
		unsigned short ExtensionLength = Offset - ExtLenOffset - sizeof(unsigned short);
		SerializationHelper::Serialize<unsigned short>(ExtensionLength, Buffer, ExtLenOffset);
	}

	// Update the two bytes we left (much) earlier with the total length of all extensions
	unsigned short TotalExtensionsLength = Offset - ExtStartOffset - sizeof(unsigned short);
	SerializationHelper::Serialize<unsigned short>(TotalExtensionsLength, Buffer, ExtStartOffset);
}
