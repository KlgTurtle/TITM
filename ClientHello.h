#pragma once
#include <vector>
#include <string>
#include <memory>
#include "ITLSMessage.h"

struct ClientHello : public ITLSMessage
{
public:
	ClientHello() {}
	ClientHello(const std::vector<char>& Buffer, size_t& Offset);

	ClientHelloHeader CHHeader;
	//ProtocolVersion Version;
	//unsigned char random[32];
	//unsigned char session_id_length;
	std::vector<unsigned char> session_id;  // between 0 and 32 bytes
											//unsigned short cipher_suites_length;
	std::vector<unsigned short> cipher_suites;  // 2 elements min
												//unsigned char compression_methods_length;
	std::vector<unsigned char> compression_methods;
	//unsigned short extensions_length;
	std::vector<std::shared_ptr<ITLSExtension>>   extensions;

	virtual std::string ToString();
protected:
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void GetExtensions(const std::vector<char>& Buffer, size_t& Offset);
};