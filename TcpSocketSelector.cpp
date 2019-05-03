#include "TcpSocketSelector.h"
#include <string>
TcpSocketSelector::TcpSocketSelector(std::vector<TcpSocket> Sockets)
{
	m_Sockets = Sockets;
}

bool TcpSocketSelector::SelectForRead()
{
	FD_SET ReadSet;
	FD_ZERO(&ReadSet);

	for (size_t i = 0; i < m_Sockets.size(); ++i)
	{
		FD_SET(m_Sockets[i].Get(), &ReadSet);
	}

	if ((select(0, &ReadSet, NULL, NULL, NULL)) == SOCKET_ERROR)
	{
		throw std::runtime_error("select() returned with error " + std::to_string(WSAGetLastError()));
	}

	return true;
}
