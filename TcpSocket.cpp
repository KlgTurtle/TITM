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

bool TcpSocket::ReceiveBufferWithMax(std::vector<char>& Data, int MaxSize)
{
	unsigned long BytesToReceive = GetPendingBytes();

	if (BytesToReceive == 0)
	{
		return false;
	}

	if (BytesToReceive > MaxSize)
	{
		BytesToReceive = MaxSize;
	}

	int NumReceived = ReceiveBufferInternal(Data, BytesToReceive);
	return (NumReceived > 0);
}

bool TcpSocket::ReceiveBufferExact(std::vector<char>& Data, int ExactSizeRequired)
{
	unsigned long PendingBytes = GetPendingBytes();
	if (PendingBytes < ExactSizeRequired)
	{
		return false;
	}
	else
	{
		int NumReceived = ReceiveBufferInternal(Data, ExactSizeRequired);
		return (NumReceived == ExactSizeRequired);
	}
}



int TcpSocket::ReceiveBufferInternal(std::vector<char>& Data, int LengthToReceive)
{
	Data.resize(LengthToReceive);
	int NumReceived = recv(m_Socket, Data.data(), Data.size(), 0);
	if (NumReceived == SOCKET_ERROR)
	{
		throw std::runtime_error("TcpSocket::ReceiveBuffer::recv failed, WSALastErr: " + WSAGetLastError());
	}
	else if ((NumReceived > 0) && (NumReceived < Data.size()))
	{
		Data.resize(NumReceived);
	}

	return NumReceived;
}

bool TcpSocket::ReceiveBuffer(std::vector<char>& Data)
{
	bool bReceivedSomething = false;
	unsigned long PendingBytes = GetPendingBytes();

	if (PendingBytes > 0)
	{
		int NumReceived = ReceiveBufferInternal(Data, PendingBytes);
		if (NumReceived > 0)
		{
			bReceivedSomething = true;
		}
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
