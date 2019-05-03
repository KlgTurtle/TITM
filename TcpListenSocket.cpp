#include "TcpListenSocket.h"
#include <string>
TcpListenSocket::TcpListenSocket(const std::string& BindAddress, const std::string& BindPort)
	: m_BindAddress(BindAddress), m_BindPort(BindPort)
{
	
}

void TcpListenSocket::Listen()
{
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	int iResult = getaddrinfo(m_BindAddress.c_str(), m_BindPort.c_str(), &hints, &result);
	if (iResult != 0)
	{
		throw std::runtime_error("getaddrinfo failed with error: " + std::to_string(iResult));
	}

	// Create a SOCKET for connecting to server
	m_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_Socket == INVALID_SOCKET) 
	{
		int WSALastErr = WSAGetLastError();
		freeaddrinfo(result);
		throw std::runtime_error("socket failed with error: " + std::to_string(WSALastErr));
	}

	// Setup the TCP listening socket
	iResult = bind(m_Socket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		int WSALastErr = WSAGetLastError();
		freeaddrinfo(result);
		closesocket(m_Socket);
		throw std::runtime_error("bind failed with error: " + std::to_string(WSALastErr));
	}

	iResult = listen(m_Socket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) 
	{
		int WSALastErr = WSAGetLastError();
		closesocket(m_Socket);
		throw std::runtime_error("listen failed with error: " + WSALastErr);
	}
}

TcpSocket TcpListenSocket::Accept()
{
	SOCKET AcceptedSocket = accept(m_Socket, NULL, NULL);

	return TcpSocket(AcceptedSocket);
}
