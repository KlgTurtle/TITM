#pragma once

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



#pragma pack(pop)