#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <vector>

class TcpSocket
{

public:
	TcpSocket();
	TcpSocket(SOCKET Socket);
	virtual ~TcpSocket();

	virtual unsigned long GetPendingBytes();
	virtual int SendBuffer(const std::vector<char>& Data);
	virtual void SendBufferAll(const std::vector<char>& Data);
	virtual bool ReceiveBuffer(std::vector<char>& Data);
	virtual bool IsConnected();
	virtual void SetBlocking(bool IsBlocking);
	virtual void Shutdown();

	virtual SOCKET Get() { return m_Socket; }

protected:

	SOCKET m_Socket;	
};