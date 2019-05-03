#pragma once
#include "TcpSocket.h"

#include <vector>
class TcpSocketSelector
{
public:
	TcpSocketSelector(std::vector<TcpSocket> Sockets);
	virtual ~TcpSocketSelector() {}
	virtual bool SelectForRead();
protected:


	std::vector<TcpSocket> m_Sockets;

};