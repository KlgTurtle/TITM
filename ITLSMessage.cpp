#include "TLS.h"
#include "ITLSMessage.h"
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
#include "ClientKeyShare.h"
#include "PskKeyExchangeModes.h"
#include "CompressCertificate.h"
#include "Padding.h"
#include "ReservedGREASE.h"
#include "ExtendedMasterSecret.h"
#include "RenegotiationInfo.h"
#include "SessionTicket.h"
#include "SignedCertificateTimestamp.h"
#include "UnknownExtension.h"
#include <memory>

std::string ITLSMessage::ToString()
{
	std::string ContentTypeStr = "";
	switch (TLSHeader.type)
	{
	case ContentType::change_cipher_spec:
		ContentTypeStr = "Change Cipher Spec";
		break;
	case	ContentType::alert:
		ContentTypeStr = "Alert";
		break;
	case	ContentType::handshake:
		ContentTypeStr = "Handshake";
		break;
	case	ContentType::application_data:
		ContentTypeStr = "Application Data";
		break;
	default:
		ContentTypeStr = "*Unknown*";
		break;
	}

	std::string VersionStr = "";
	if (TLSHeader.version.major == 3 && TLSHeader.version.minor >= 1)
	{
		VersionStr = "TLSv1." + std::to_string(TLSHeader.version.minor - 1);
	}
	else
	{
		VersionStr += "SSLv" + std::to_string(TLSHeader.version.major) + ".0";
	}

	std::string LengthStr = std::to_string(TLSHeader.length);

	return
		"Content Type:" + ContentTypeStr + "\n"
		"Version:     " + VersionStr + "\n" +
		"Length:      " + LengthStr;
}

void ITLSMessage::SerializePlaintextHeader(std::vector<char>& Buffer, size_t & Offset)
{
	this->TLSHeader.Serialize(Buffer, Offset);
}

void ITLSMessage::DeserializePlaintextHeader(const std::vector<char>& Buffer, size_t & Offset)
{
	this->TLSHeader.Deserialize(Buffer, Offset);
}

void ITLSHandshakeMessage::SerializeHandshakeHeader(std::vector<char>& Buffer, size_t & Offset)
{
	this->HSHeader.Serialize(Buffer, Offset);
}

void ITLSHandshakeMessage::DeserializeHandshakeHeader(const std::vector<char>& Buffer, size_t & Offset)
{
	this->HSHeader.Deserialize(Buffer, Offset);
}

void ITLSHandshakeMessage::GetExtensions(const std::vector<char>& Buffer, size_t & Offset, std::vector<std::shared_ptr<ITLSExtension>>& outExtensions)
{
	outExtensions.clear();
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
			outExtensions.push_back(std::make_shared<ServerNameExtension>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::supported_groups:
			outExtensions.push_back(std::make_shared<SupportedGroups>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::supported_versions:
			outExtensions.push_back(std::make_shared<ClientSupportedVersions>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::ec_points_format:
			outExtensions.push_back(std::make_shared<ECPointFormats>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::application_layer_protocol_negotiation:
			outExtensions.push_back(std::make_shared<AppLayerProtocolNegotiation>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::status_request:
			outExtensions.push_back(std::make_shared<StatusRequest>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::signature_algorithms:
			outExtensions.push_back(std::make_shared<SignatureHashAlgorithms>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::key_share:
			outExtensions.push_back(std::make_shared<ClientKeyShare>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::psk_key_exchange_modes:
			outExtensions.push_back(std::make_shared<PskKeyExchangeModes>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::compress_certificate:
			outExtensions.push_back(std::make_shared<CompressCertificate>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::padding:
			outExtensions.push_back(std::make_shared<Padding>(Buffer, Offset, ExtLength, bIsExtEmpty));
			break;
		case ExtensionType::extended_master_secret:
			outExtensions.push_back(std::make_shared<ExtendedMasterSecret>(Buffer, Offset, ExtLength, bIsExtEmpty));
			break;
		case ExtensionType::renegotiation_info:
			outExtensions.push_back(std::make_shared<RenegotiationInfo>(Buffer, Offset, bIsExtEmpty));
			break;
		case ExtensionType::session_ticket:
			outExtensions.push_back(std::make_shared<SessionTicket>(Buffer, Offset, ExtLength, bIsExtEmpty));
			break;
		case ExtensionType::signed_certificate_timestamp:
			outExtensions.push_back(std::make_shared<SignedCertificateTimestamp>(Buffer, Offset, ExtLength, bIsExtEmpty));
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
			outExtensions.push_back(std::make_shared<ReservedGREASE>(Buffer, Offset, ExtType, ExtLength, bIsExtEmpty));
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
			outExtensions.push_back(std::make_shared<UnknownExtension>(Buffer, Offset, ExtType, ExtLength, bIsExtEmpty));
			break;

		}


		ExtensionsLength -= (ExtLength + sizeof(ExtensionType) + sizeof(ExtLength));
	}
}
