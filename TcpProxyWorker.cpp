#include "TcpProxyWorker.h"
#include "TcpSocketSelector.h"
TcpProxyWorker::TcpProxyWorker(const TITMParms& Parms, TcpSocket SourceSocket, const std::string & TargetAddress, 
	const std::string & TargetPort) : m_Parms(Parms),
	m_SourceSocket(SourceSocket), m_TargetSocket(TargetAddress, TargetPort), m_bStopped(false)
{

}

void TcpProxyWorker::Start()
{
	try
	{
		m_TargetSocket.Connect();

		std::vector<char> DataBuffer;

		TcpSocketSelector SocketSelector({ m_SourceSocket , m_TargetSocket });
		while (!m_bStopped)
		{
			bool bHaveData = SocketSelector.SelectForRead();
			if (bHaveData)
			{
				if (m_SourceSocket.ReceiveBuffer(DataBuffer))
				{
					if (m_Parms.bPrintSrc)
					{
						m_Parms.Printer->Print(DataBuffer);
					}
					m_TargetSocket.SendBufferAll(DataBuffer);
				}

				if (m_TargetSocket.ReceiveBuffer(DataBuffer))
				{
					if (m_Parms.bPrintDst)
					{
						m_Parms.Printer->Print(DataBuffer);
					}
					m_SourceSocket.SendBufferAll(DataBuffer);
				}
			}
		}
	}
	catch (std::runtime_error& e)
	{
		printf(e.what());
	}
}

void TcpProxyWorker::Stop()
{
}
