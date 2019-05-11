#pragma once
#include "TcpClientSocket.h"
#include "TcpListenSocket.h"
#include "TcpProxy.h"
#include "TcpSocket.h"
#include <memory>
#include "TITMParms.h"
#include "TLSState.h"


	enum class TcpProxyDirection
	{
		SrcToDest = 0,
		DestToSrc = 1
	};



class TcpProxyWorker
{
public:
	TcpProxyWorker(const TITMParms& Parms, TcpSocket SourceSocket, const std::string& TargetAddress, const std::string& TargetPort);
	virtual ~TcpProxyWorker() {}
	virtual void Start();
	virtual void Stop();
	

protected:
	virtual void InvokeUserRoutine(std::vector<char>& DataBuffer, TcpProxyDirection Direction);
	virtual std::string GetTempFilePath();
	virtual void UpdateBufferFromScriptOutput(const std::string& OutputFile, std::vector<char>& DataBuffer);
	virtual bool AttemptToReadMore(int AdditionalBytesRequested, std::vector<char>& DataBuffer, const TcpProxyDirection& Direction);
	virtual void WriteDataBufferToFile(std::vector<char>& DataBuffer, const std::string& OutputFile);
	virtual void DoParamDefinedActions(std::vector<char>& DataBuffer, const TcpProxyDirection& Direction);
	TcpSocket       m_SourceSocket;
	TcpClientSocket m_TargetSocket;
	bool            m_bStopped;
	TITMParms       m_Parms;

	TLSState m_SourceTLSState;
	TLSState m_TargetTLSState;
	
};