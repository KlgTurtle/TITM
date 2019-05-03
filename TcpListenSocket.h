#pragma once
#include "TcpSocket.h"
class TcpListenSocket : public TcpSocket
{
public:
	TcpListenSocket(const std::string& BindAddress, const std::string& BindPort);
	virtual ~TcpListenSocket() {}
	TcpSocket Accept();
	void Listen();
protected:
	std::string m_BindAddress;
	std::string m_BindPort;
};