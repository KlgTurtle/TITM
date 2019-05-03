#pragma once
#include "TcpListenSocket.h"
#include "TcpClientSocket.h"
#include "TcpSocket.h"
#include "TcpProxyWorker.h"
#include <memory>
#include "TITMParms.h"
class TcpProxyWorker;



class TcpProxy
{
public:
	TcpProxy(const TITMParms& Parms, const std::string& ListenPort, const std::string& TargetAddress, const std::string& TargetPort);
	virtual ~TcpProxy() {}
	virtual void Start();
	virtual void Stop();
	static DWORD WINAPI StartWorker(LPVOID pParam);
protected:
	TcpListenSocket m_SourceListenSocket;
	//TcpSocket       m_SourceSocket;
	//TcpClientSocket m_TargetSocket;
	

	std::string m_ListenPort;
	std::string m_TargetAddress;
	std::string m_TargetPort;
	bool        m_bStopped;
	std::vector<HANDLE> m_WorkerThreads;
	std::vector<std::shared_ptr<TcpProxyWorker>> m_WorkerObjects;

	TITMParms   m_Parms;
};