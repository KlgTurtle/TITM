#pragma once

#include "TcpProxy.h"

class ITcpProxyManipulator
{
public:
	ITcpProxyManipulator(TcpSocket SourceSocket, TcpSocket TargetSocket);
	virtual ~ITcpProxyManipulator() {}
	virtual void ManipulateSourceToDest(std::vector<char>& Buffer) = 0;
	virtual void ManipulateDestToSource(std::vector<char>& Buffer) = 0;
protected:
	TcpSocket m_SourceSocket;
	TcpSocket m_TargetSocket;
};