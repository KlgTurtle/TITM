#pragma once
#include <ws2tcpip.h>
#include <windows.h>
#include <vector>
#include "TcpSocket.h"

class TcpClientSocket : public TcpSocket
{

public:
	virtual ~TcpClientSocket() {}
	TcpClientSocket(const std::string& TargetAddress, const std::string& TargetPort);
	virtual void Connect();
protected:
	std::string m_TargetAddress;
	std::string m_TargetPort;

};