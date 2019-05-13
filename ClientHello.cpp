#include <WinSock2.h>
#include "TLS.h"
#include "ServerNameExtension.h"
#include "SupportedGroups.h"
#include "ClientHello.h"
#include "SerializationHelper.h"
#include "ClientSupportedVersions.h"

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

	// Build ClientHello Session ID
	this->session_id.clear();
	this->session_id.insert(this->session_id.begin(), &Buffer[Offset],
		&Buffer[Offset + CHHeader->SessionIdLength]);

	Offset += CHHeader->SessionIdLength;

	// Build CipherSuites
	unsigned short CipherSuitesLength = SerializationHelper::DeserializeUnsignedShort(Buffer, Offset);

	for (size_t i = 0; i < CipherSuitesLength / sizeof(unsigned short); ++i)
	{
		unsigned short CipherSuite = SerializationHelper::DeserializeUnsignedShort(Buffer, Offset);
		this->cipher_suites.push_back(CipherSuite);
	}


	unsigned char compression_methods_length = Buffer[Offset];
	Offset += sizeof(compression_methods_length);
	this->compression_methods.clear();
	this->compression_methods.insert(this->compression_methods.begin(),
		&Buffer[Offset], &Buffer[Offset + compression_methods_length]);

	Offset += compression_methods_length;

	// Build Extensions



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
		case ExtensionType::extended_master_secret:
		case ExtensionType::renegotiation_info:
		case ExtensionType::max_fragment_length:
		//	break;
		case ExtensionType::status_request:
		//	break;
		
		//	break;
		case ExtensionType::signature_algorithms:
		//	break;
		case ExtensionType::use_srtp:
		//	break;
		case ExtensionType::heartbeat:
		//	break;
		case ExtensionType::application_layer_protocol_negotiation:
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
		case ExtensionType::psk_key_exchange_modes:
		//	break;
		case ExtensionType::certificate_authorities:
		//	break;
		case ExtensionType::oid_filters:
			//break;
		case ExtensionType::post_handshake_auth:
		//	break;
		case ExtensionType::signature_algorithms_cert:
		//	break;
		case ExtensionType::key_share:
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
