#include "TcpSocket.h"
#include <string>
TcpSocket::TcpSocket()
{
	m_Socket = INVALID_SOCKET;
}

TcpSocket::TcpSocket(SOCKET Socket)
{
	m_Socket = Socket;
}

TcpSocket::~TcpSocket()
{
	/*if (m_Socket != INVALID_SOCKET)
	{
		closesocket(m_Socket);
	}*/
}

unsigned long TcpSocket::GetPendingBytes()
{
	unsigned long PendingBytes;
	int Rc = ioctlsocket(m_Socket, FIONREAD, &PendingBytes);
	if (Rc != 0)
	{
		throw std::runtime_error("TcpSocket::GetPendingBytes::ioctlsocket failed, returned: " + std::to_string(Rc));
	}

	return PendingBytes;
}

int TcpSocket::SendBuffer(const std::vector<char>& Data)
{
	return send(m_Socket, Data.data(), Data.size(), 0);
}

void TcpSocket::SendBufferAll(const std::vector<char>& Data)
{
	int sent = 0;

	while (sent < Data.size())
	{
		sent += SendBuffer(std::vector<char>(Data.begin() + sent, Data.end()));
	}
}

bool TcpSocket::ReceiveBuffer(std::vector<char>& Data)
{
	bool bReceivedSomething = false;
	unsigned long PendingBytes = GetPendingBytes();

	if (PendingBytes > 0)
	{
		Data.resize(GetPendingBytes());
		int NumReceived = recv(m_Socket, Data.data(), Data.size(), 0);
		if (NumReceived == SOCKET_ERROR)
		{
			throw std::runtime_error("TcpSocket::ReceiveBuffer::recv failed, WSALastErr: " + WSAGetLastError());
		}
		else if (NumReceived > 0) 
		{
			if (NumReceived < Data.size())
			{
				Data.resize(NumReceived);
			}
			bReceivedSomething = true;
		}
		else
		{
			bReceivedSomething = false;
		}
	}
	else
	{
		bReceivedSomething = false;
	}

	return bReceivedSomething;
}




bool TcpSocket::IsConnected()
{
	return false;
}

void TcpSocket::SetBlocking(bool IsBlocking)
{
}

void TcpSocket::Shutdown()
{
	shutdown(m_Socket, SD_BOTH);
	closesocket(m_Socket);
	m_Socket = INVALID_SOCKET;
}
