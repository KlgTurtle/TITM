#pragma once
#include <vector>

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
	renegotiation_info = 65281
};

struct ITLSExtension
{
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset) = 0;
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset) = 0;
};

