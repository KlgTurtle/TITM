#pragma once
#include <string>
#include <vector>

#pragma pack(push, 1)

enum class ContentType : unsigned char
{
	change_cipher_spec = 20,
	alert = 21,
	handshake = 22,
	application_data = 23
};

struct ProtocolVersion 
{
	unsigned char major;
	unsigned char minor;
};

struct TLSPlaintextHeader 
{
	ContentType type;
	ProtocolVersion version;
	unsigned short length;
};

//uint16 ProtocolVersion;
//opaque Random[32];

//uint8 CipherSuite[2];    /* Cryptographic suite selector */




enum class ExtensionType : unsigned short
{
	server_name = 0,
	max_fragment_length = 1,
	status_request = 5,
	supported_groups = 10,
	signature_algorithms = 13,
	use_srtp = 14,
	heartbeat = 15,
	application_layer_protocol_negotiation = 16,
	signed_certificate_timestamp = 18,
	client_certificate_type = 19,
	server_certificate_type = 20,
	padding = 21,
	pre_shared_key = 41,
	early_data = 42,
	supported_versions = 43,
	cookie = 44,
	psk_key_exchange_modes = 45,
	certificate_authorities = 47,
	oid_filters = 48,
	post_handshake_auth = 49,
	signature_algorithms_cert = 50,
	key_share = 51
};
enum class HandshakeType : unsigned char
{
	hello_request = 0,
	client_hello = 1,
	server_hello = 2,
	certificate = 11,
	server_key_exchange = 12,
	certificate_request = 13,
	server_hello_done = 14,
	certificate_verify = 15,
	client_key_exchange = 16,
	finished = 20
};

struct HandshakeHeader
{
	HandshakeType msg_type;    /* handshake type */
	unsigned char length[3];   /* bytes in message */
} ;

struct ClientHelloHeader 
{
	ProtocolVersion legacy_version;
	unsigned char random[32];
	unsigned char SessionIdLength;
} ;


#pragma pack(pop)

struct ITLSMessage
{
	TLSPlaintextHeader TLSHeader;
	virtual std::string ToString();
};
/*  struct {
          ProtocolVersion legacy_version = 0x0303;  
		  Random random;
		  opaque legacy_session_id<0..32>;
		  CipherSuite cipher_suites<2..2 ^ 16 - 2>;
		  opaque legacy_compression_methods<1..2 ^ 8 - 1>;
		  Extension extensions<8..2 ^ 16 - 1>;
	  } ClientHello; */
struct IExtension
{

};

struct ClientHello : public ITLSMessage
{
	ProtocolVersion Version;
	unsigned char random[32];
	unsigned char session_id_length;
	std::vector<unsigned char> session_id;  // between 0 and 32 bytes
	unsigned short cipher_suites_length;
	std::vector<unsigned short> cipher_suites;  // 2 elements min
	unsigned char compression_methods_length;
	std::vector<unsigned char> compression_methods;
	unsigned short extensions_length;
	std::vector<IExtension*>   extensions;

	virtual std::string ToString();
} ;





