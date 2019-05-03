#include "TcpProxy.h"
#include <string>
#include <memory>
#include "TcpProxyWorker.h"
TcpProxy::TcpProxy(const TITMParms& Parms,const std::string & ListenPort, const std::string & TargetAddress, 
	const std::string & TargetPort)
	: m_Parms(Parms), m_ListenPort(ListenPort), m_TargetAddress(TargetAddress), m_TargetPort(TargetPort),
	m_SourceListenSocket("127.0.0.1", ListenPort),
	m_bStopped(false)
{

}

void TcpProxy::Start()
{
	//	m_TargetSocket(TargetAddress, TargetPort),
	m_SourceListenSocket.Listen();
	while (!m_bStopped)
	{
		TcpSocket SourceSocket = m_SourceListenSocket.Accept();

		std::shared_ptr<TcpProxyWorker>* ProxyWorker = new std::shared_ptr<TcpProxyWorker>();

		*ProxyWorker = std::make_shared<TcpProxyWorker>(m_Parms, SourceSocket, m_TargetAddress, m_TargetPort);


		// Start worker
		HANDLE hThread = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			TcpProxy::StartWorker,       // thread function name
			ProxyWorker,          // argument to thread function 
			0,                      // use default creation flags 
			NULL);   // returns the thread identifier 

		m_WorkerThreads.push_back(hThread);
		m_WorkerObjects.push_back(*ProxyWorker);

	}

	for (size_t i = 0; i < m_WorkerObjects.size(); ++i)
	{
		m_WorkerObjects[i]->Stop();
	}

	// Wait 30 seconds for all threads to end
	WaitForMultipleObjects(m_WorkerThreads.size(), m_WorkerThreads.data(), TRUE, 30 * 1000);
}

void TcpProxy::Stop()
{
	m_bStopped = true;
}

DWORD WINAPI TcpProxy::StartWorker(LPVOID pParam)
{
	std::shared_ptr<TcpProxyWorker>* pProxyWorker = static_cast<std::shared_ptr<TcpProxyWorker>*>(pParam);

	std::shared_ptr<TcpProxyWorker> ProxyWorker = *pProxyWorker;
	delete pProxyWorker; pProxyWorker = NULL;

	ProxyWorker->Start();

	return 0;
}