#pragma once
#include <vector>
#include "SerializationHelper.h"

enum class ExtensionType : unsigned short
{
	server_name = 0,
	max_fragment_length = 1,
	status_request = 5,
	supported_groups = 10,
	ec_points_format = 11,
	signature_algorithms = 13,
	use_srtp = 14,
	heartbeat = 15,
	application_layer_protocol_negotiation = 16,
	signed_certificate_timestamp = 18,
	client_certificate_type = 19,
	server_certificate_type = 20,
	padding = 21,
	extended_master_secret = 23,
	compress_certificate = 27,
	session_ticket = 35,
	pre_shared_key = 41,
	early_data = 42,
	supported_versions = 43,
	cookie = 44,
	psk_key_exchange_modes = 45,
	certificate_authorities = 47,
	oid_filters = 48,
	post_handshake_auth = 49,
	signature_algorithms_cert = 50,
	key_share = 51,
	GREASE_Reserved1 = 0x0A0A, 
	GREASE_Reserved2 = 0x1A1A,
	GREASE_Reserved3 = 0x2A2A, 
	GREASE_Reserved4 = 0x3A3A, 
	GREASE_Reserved5 = 0x4A4A, 
	GREASE_Reserved6 = 0x5A5A, 
	GREASE_Reserved7 = 0x6A6A, 
	GREASE_Reserved8 = 0x7A7A, 
	GREASE_Reserved9 = 0x8A8A, 
	GREASE_Reserved10 = 0x9A9A, 
	GREASE_Reserved11 = 0xAAAA, 
	GREASE_Reserved12 = 0xBABA, 
	GREASE_Reserved13 = 0xCACA, 
	GREASE_Reserved14 = 0xDADA, 
	GREASE_Reserved15 = 0xEAEA, 
	GREASE_Reserved16 = 0xFAFA,
	renegotiation_info = 65281
};

struct ITLSExtension
{
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset) = 0;
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset) = 0;
	virtual ExtensionType GetType() = 0;

};

