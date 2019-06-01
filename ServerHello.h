#pragma once
#include <vector>
#include <string>
#include <memory>
#include "ITLSMessage.h"

/*struct {
          ProtocolVersion legacy_version = 0x0303;     TLS v1.2
Random random;
opaque legacy_session_id_echo<0..32>;
CipherSuite cipher_suite;
uint8 legacy_compression_method = 0;
Extension extensions<6..2 ^ 16 - 1>;
	  } ServerHello; */


struct ServerHello : public ITLSHandshakeMessage
{
public:
	ServerHello() {}
	ServerHello(const std::vector<char>& Buffer, size_t& Offset);

	ServerHelloHeader SHHeader;
	std::vector<unsigned char> session_id;  // between 0 and 32 bytes
	unsigned short cipher_suite; 		
	unsigned char legacy_compression_method;
	std::vector<std::shared_ptr<ITLSExtension>>   extensions;

	virtual std::string ToString();

	virtual HandshakeType GetHandshakeType() { return HandshakeType::server_hello; }
protected:
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
};