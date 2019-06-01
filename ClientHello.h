#pragma once
#include <vector>
#include <string>
#include <memory>
#include "ITLSMessage.h"

struct ClientHello : public ITLSHandshakeMessage
{
public:
	ClientHello() {}
	ClientHello(const std::vector<char>& Buffer, size_t& Offset);

	ClientHelloHeader CHHeader;
	std::vector<unsigned char> session_id;  // between 0 and 32 bytes
	std::vector<unsigned short> cipher_suites;  // 2 elements min				
	std::vector<unsigned char> compression_methods;
	std::vector<std::shared_ptr<ITLSExtension>>   extensions;

	virtual std::string ToString();

	virtual HandshakeType GetHandshakeType() { return HandshakeType::client_hello; }
protected:
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	//virtual void GetExtensions(const std::vector<char>& Buffer, size_t& Offset);
};