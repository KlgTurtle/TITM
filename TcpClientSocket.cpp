#include "TcpClientSocket.h"
#include <string>
TcpClientSocket::TcpClientSocket(const std::string & TargetAddress, const std::string & TargetPort)
	: m_TargetAddress(TargetAddress), m_TargetPort(TargetPort)
{

}

void TcpClientSocket::Connect()
{
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	int iResult = getaddrinfo(m_TargetAddress.c_str(), m_TargetPort.c_str(), &hints, &result);
	if (iResult != 0) 
	{
		throw std::runtime_error("getaddrinfo failed with error: " + std::to_string(iResult));
	}

	// Attempt to connect to an address until one succeeds
	for (addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next) 
	{

		// Create a SOCKET for connecting to server
		m_Socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (m_Socket == INVALID_SOCKET) 
		{
			throw std::runtime_error("socket failed with error: " + std::to_string(WSAGetLastError()));
		}

		// Connect to server.
		iResult = connect(m_Socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) 
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (m_Socket == INVALID_SOCKET)
	{
		throw std::runtime_error("Failed to connect to target: " + m_TargetAddress + ":" + m_TargetPort);
	}
}
