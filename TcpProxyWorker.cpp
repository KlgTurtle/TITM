#include "TcpProxyWorker.h"
#include "TcpSocketSelector.h"
#include <string>
#include <sstream>
#include <fstream>

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

					InvokeUserRoutine(DataBuffer, TcpProxyDirection::SrcToDest);
					m_TargetSocket.SendBufferAll(DataBuffer);
				}

				if (m_TargetSocket.ReceiveBuffer(DataBuffer))
				{
					if (m_Parms.bPrintDst)
					{
						m_Parms.Printer->Print(DataBuffer);
					}

					InvokeUserRoutine(DataBuffer, TcpProxyDirection::DestToSrc);
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

void TcpProxyWorker::AttemptToReadMore(int AdditionalBytesRequested, std::vector<char>& DataBuffer)
{
}

void TcpProxyWorker::WriteDataBufferToFile(std::vector<char>& DataBuffer, const std::string & OutputFile)
{
	std::ofstream file(OutputFile, std::ios::binary | std::ios::trunc);
	file.write(DataBuffer.data(), DataBuffer.size());
	file.close();
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

			WriteDataBufferToFile(DataBuffer, TempOutput);

			std::string PsParameters = " " +
				std::to_string(InvocationNumber) + " " +
			 	std::to_string(static_cast<unsigned int>(Direction)) + " " +
				TempOutput;

			std::string PsExecuteCmd = "powershell.exe -windowstyle hidden -file " + m_Parms.ScriptPath + PsParameters;

			int AdditionalBytesRequested = system(PsExecuteCmd.c_str());
							
			if (AdditionalBytesRequested == 0)
			{
				UpdateBufferFromScriptOutput(TempOutput, DataBuffer);
				bInvokeAgain = false;
			}
			else if (AdditionalBytesRequested > 0)
			{
				AttemptToReadMore(AdditionalBytesRequested, DataBuffer);
			}
			else
			{
				throw std::runtime_error("Illegal return value received from user powershell script: " + m_Parms.ScriptPath);
			}

			DeleteFile(TempOutput.c_str());

			++InvocationNumber;
		}

		//system("start powershell.exe " + );
	}
}
