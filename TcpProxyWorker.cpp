#include "TcpProxyWorker.h"
#include "TcpSocketSelector.h"
#include "TLS.h"
#include "ClientHello.h"
#include "ServerHello.h"
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include "Padding.h"
#include "ServerSupportedVersions.h"

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
				if (m_SourceSocket.ReceiveBufferWithMax(DataBuffer, MAX_DATA_BUFFER_SIZE))
				{
					DoParamDefinedActions(DataBuffer, TcpProxyDirection::SrcToDest);
					m_TargetSocket.SendBufferAll(DataBuffer);
				}

				if (m_TargetSocket.ReceiveBufferWithMax(DataBuffer, MAX_DATA_BUFFER_SIZE))
				{
					DoParamDefinedActions(DataBuffer, TcpProxyDirection::DestToSrc);
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

std::string TcpProxyWorker::GetTempFilePath()
{
	TCHAR lpTempPathBuffer[MAX_PATH];
	TCHAR szTempFileName[MAX_PATH];

	//  Gets the temp path env string (no guarantee it's a valid path).
	DWORD dwRetVal = GetTempPath(MAX_PATH,          // length of the buffer
		lpTempPathBuffer); // buffer for path 
	if (dwRetVal > MAX_PATH || (dwRetVal == 0))
	{
		
		
	}
	//  Generates a temporary file name. 
	UINT uRetVal = GetTempFileName(lpTempPathBuffer, // directory for tmp files
		TEXT("TITM"),     // temp file name prefix 
		0,                // create unique name 
		szTempFileName);  // buffer for name 
	if (uRetVal == 0)
	{
		
	}

	return szTempFileName;
}

void TcpProxyWorker::UpdateBufferFromScriptOutput(const std::string & OutputFile, std::vector<char>& DataBuffer)
{
	std::vector<char> tempold = DataBuffer;
	std::ifstream file(OutputFile, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	DataBuffer.resize(size);
	if (file.read(DataBuffer.data(), size))
	{
		file.close();
	}
	else
	{
		file.close();
		throw std::runtime_error("Failed to read temporary output file: " + OutputFile);
	}
}

bool TcpProxyWorker::AttemptToReadMore(int AdditionalBytesRequested, std::vector<char>& DataBuffer, const TcpProxyDirection & Direction)
{
	bool bSuccess = false;

	std::vector<char> NewDataBuffer;
	if (Direction == TcpProxyDirection::SrcToDest)
	{	
		bSuccess = m_SourceSocket.ReceiveBufferExact(NewDataBuffer, AdditionalBytesRequested);
	}
	else
	{
		bSuccess = m_TargetSocket.ReceiveBufferExact(NewDataBuffer, AdditionalBytesRequested);
	}

	if (bSuccess)
	{
		DataBuffer.insert(DataBuffer.end(), NewDataBuffer.begin(), NewDataBuffer.end());
		if (DataBuffer.size() > MAX_DATA_BUFFER_SIZE)
		{
			DataBuffer = std::vector<char>(DataBuffer.end() - MAX_DATA_BUFFER_SIZE, DataBuffer.end());
		}
	}

	return bSuccess;
}


void TcpProxyWorker::WriteDataBufferToFile(std::vector<char>& DataBuffer, const std::string & OutputFile)
{
	std::ofstream file(OutputFile, std::ios::binary | std::ios::trunc);
	file.write(DataBuffer.data(), DataBuffer.size());
	file.close();
}

void TcpProxyWorker::DoParamDefinedActions(std::vector<char>& DataBuffer, const TcpProxyDirection& Direction)
{
	if ((Direction == TcpProxyDirection::SrcToDest && m_Parms.bPrintSrc) || 
		(Direction == TcpProxyDirection::DestToSrc && m_Parms.bPrintDst))
	{
	//	m_Parms.Printer->PrintRegular(Direction == TcpProxyDirection::SrcToDest ?
		//	"Source->Dest" : "Dest->Source");
		if (m_Parms.bDecodeAsTLS)
		{
			std::vector<std::shared_ptr<ITLSMessage>> TLSMessages;
			(Direction == TcpProxyDirection::SrcToDest) ?
				m_SourceTLSState.Update(DataBuffer, TLSMessages) :
				m_TargetTLSState.Update(DataBuffer, TLSMessages);

			//for each (auto TLSMessage in TLSMessages)
			//{
				//if (TLSMessage->GetType() == ContentType::handshake &&
				//	((ITLSHandshakeMessage*)TLSMessage.get())->GetHandshakeType() == HandshakeType::server_hello)
				//{
				//	std::shared_ptr<ServerHello> SH = std::static_pointer_cast<ServerHello>(TLSMessage);
				//	for (size_t i = 0; i < SH->extensions.size(); ++i)
				//	{
				//		if (SH->extensions[i]->GetType() == ExtensionType::supported_versions)
				//		{
				//			std::shared_ptr<ServerSupportedVersions> ssv = std::static_pointer_cast<ServerSupportedVersions>(SH->extensions[i]);
				//			ssv->ChosenVersion.major = 0xBE;
				//			//SH->extensions.erase(SH->extensions.begin() + i);
				//			break;
				//		}
				//	}
				//	size_t off = 0;

				//	//std::vector<char> db;
				//	//db.clear();
				//	//DataBuffer.resize(0);
				//	//TLSMessage->Serialize(db, off);
				//	//db.resize(off);

				//	std::vector<char> tmp = DataBuffer;
				//	//DataBuffer.erase(DataBuffer.begin(), DataBuffer.begin() + off);
				//	off = 0;
				//	TLSMessage->Serialize(DataBuffer, off);

				//	if (tmp != DataBuffer)
				//	{
				//		int z = 1;
				//	}



				//}
				

				//if (TLSMessage != nullptr)
				//{
				//	m_Parms.Printer->PrintRegular(TLSMessage->ToString());
				//}


				
			//}

			
		}
		else
		{
			
			m_Parms.Printer->Print(DataBuffer);
		}
	}

	//InvokeUserRoutine(DataBuffer, Direction);
}


void TcpProxyWorker::InvokeUserRoutine(std::vector<char>& DataBuffer, TcpProxyDirection Direction)
{
	if (m_Parms.ScriptPath != "")
	{
		size_t InvocationNumber = 1;
		const size_t MaxInvocations = 10;
		bool   bInvokeAgain = true;

		while (bInvokeAgain && (InvocationNumber <= MaxInvocations))
		{
			std::string TempOutput = GetTempFilePath();

			std::stringstream DataBufferHexEncoded;

			DataBufferHexEncoded.clear();

			std::for_each(DataBuffer.begin(), DataBuffer.end(),
				[&DataBufferHexEncoded](const unsigned char &c) 
			{ 
				DataBufferHexEncoded << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(c);
			});
			const std::string PsCommand = "powershell.exe -executionpolicy bypass -windowstyle hidden -noninteractive -nologo -file ";
			//const std::string PsCommand = "powershell.exe -executionpolicy bypass -nologo -file ";
			std::string PsParameters = " " +
				std::to_string(InvocationNumber) + " " +
			 	std::to_string(static_cast<unsigned int>(Direction)) + " " +
				TempOutput + " " +
				DataBufferHexEncoded.str();

			std::string PsExecuteCmd = PsCommand + m_Parms.ScriptPath + PsParameters;

			int AdditionalBytesRequested = system(PsExecuteCmd.c_str());
			
			// Return code logic:
			// Negative: no need to reinvoke, and buffer was not updated
			// Zero:     no need to reinvoke, updated buffer was written to file
			// Positive: need to reinvoke after reading additional bytes, specified by value.
			if (AdditionalBytesRequested < 0)
			{
				bInvokeAgain = false;
			}
			else if (AdditionalBytesRequested == 0)
			{
				UpdateBufferFromScriptOutput(TempOutput, DataBuffer);
				bInvokeAgain = false;
			}
			else 
			{
				bInvokeAgain = AttemptToReadMore(AdditionalBytesRequested, DataBuffer, Direction);
			}
			

			DeleteFile(TempOutput.c_str());

			++InvocationNumber;
		}

		//system("start powershell.exe " + );
	}
}
