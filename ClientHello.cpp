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

ClientHello::ClientHello(const std::vector<char>& Buffer)
{
	Deserialize(Buffer);
}

std::string ClientHello::ToString()
{
	std::string TLSHeader = ITLSMessage::ToString();

	return "";
}

void ClientHello::Deserialize(const std::vector<char>& Buffer)
{
	size_t Offset = sizeof(TLSPlaintextHeader) + sizeof(HandshakeHeader);

	const ClientHelloHeader* CHHeader =
		reinterpret_cast<const ClientHelloHeader*>(&Buffer[Offset]);

	// Build ClientHello Header
	this->Version = CHHeader->legacy_version;
	memcpy(this->random, CHHeader->random, sizeof(CHHeader->random));
	//RetClientHello->session_id_length = CHHeader->SessionIdLength;

	Offset += sizeof(ClientHelloHeader);

	SerializationHelper::DeserializeVec<unsigned char, unsigned char>(Buffer, Offset, this->session_id);
	SerializationHelper::DeserializeVec<unsigned short, unsigned short>(Buffer, Offset, this->cipher_suites);
	SerializationHelper::DeserializeVec<unsigned char, unsigned char>(Buffer, Offset, this->compression_methods);

	GetExtensions(Buffer, Offset);
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


		// Handle extension here
		switch (ExtType)
		{
		case ExtensionType::server_name:
			extensions.push_back(std::make_shared<ServerNameExtension>(Buffer, Offset));
			break;
		case ExtensionType::supported_groups:
			extensions.push_back(std::make_shared<SupportedGroups>(Buffer, Offset));
			break;
		case ExtensionType::supported_versions:
			extensions.push_back(std::make_shared<ClientSupportedVersions>(Buffer, Offset));
			break;
		case ExtensionType::ec_points_format:
			extensions.push_back(std::make_shared<ECPointFormats>(Buffer, Offset));
			break;
		case ExtensionType::application_layer_protocol_negotiation:
			extensions.push_back(std::make_shared<AppLayerProtocolNegotiation>(Buffer, Offset));
			break;
		case ExtensionType::status_request:
			extensions.push_back(std::make_shared<StatusRequest>(Buffer, Offset));
			break;
		case ExtensionType::signature_algorithms:
			extensions.push_back(std::make_shared<SignatureHashAlgorithms>(Buffer, Offset));
			break;
		case ExtensionType::key_share:
			extensions.push_back(std::make_shared<KeyShare>(Buffer, Offset));
			break;
		case ExtensionType::psk_key_exchange_modes:
			extensions.push_back(std::make_shared<PskKeyExchangeModes>(Buffer, Offset));
			break;
		case ExtensionType::extended_master_secret:
		case ExtensionType::renegotiation_info:
		case ExtensionType::max_fragment_length:
	
		case ExtensionType::use_srtp:
		//	break;
		case ExtensionType::heartbeat:
		//	break;
		
		//	break;
		case ExtensionType::signed_certificate_timestamp:
		//	break;
		case ExtensionType::client_certificate_type:
		//	break;
		case ExtensionType::server_certificate_type:
		//	break;
		case ExtensionType::padding:
		//	break;
		case ExtensionType::pre_shared_key:
		//	break;
		case ExtensionType::early_data:
			//break;
		
		//	break;
		case ExtensionType::cookie:
		//	break;
		
		//	break;
		case ExtensionType::certificate_authorities:
		//	break;
		case ExtensionType::oid_filters:
			//break;
		case ExtensionType::post_handshake_auth:
		//	break;
		case ExtensionType::signature_algorithms_cert:
		//	break;
		
		//	break;
		case ExtensionType::compress_certificate:
		//	break;
		default:
			Offset += ExtLength;
			break;

		}

		
		ExtensionsLength -= (ExtLength + sizeof(ExtensionType) + sizeof(ExtLength));
	}
}


void ClientHello::Serialize(std::vector<char>& Buffer)
{
}
