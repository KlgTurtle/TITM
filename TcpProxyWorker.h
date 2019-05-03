#pragma once
#include "TcpClientSocket.h"
#include "TcpListenSocket.h"
#include "TcpProxy.h"
#include "TcpSocket.h"
#include <memory>
#include "TITMParms.h"


class TcpProxyWorker
{
public:
	TcpProxyWorker(const TITMParms& Parms, TcpSocket SourceSocket, const std::string& TargetAddress, const std::string& TargetPort);
	virtual ~TcpProxyWorker() {}
	virtual void Start();
	virtual void Stop();

protected:
	TcpSocket       m_SourceSocket;
	TcpClientSocket m_TargetSocket;
	bool            m_bStopped;
	TITMParms       m_Parms;
};