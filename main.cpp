#undef UNICODE

#define WIN32_LEAN_AND_MEAN


#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "TcpListenSocket.h"
#include "TcpProxy.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
int InitWinsock()
{
	WSADATA wsaData;

	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	else
	{
		return 0;
	}


}



TITMParms ParseParameters(int argc, char** argv)
{
	TITMParms retParms;
	for (size_t i = 0; i < argc - 1; ++i)
	{
		std::string parm = argv[1];

		if (parm == "-PrintSrc")
		{
			retParms.bPrintSrc = true;
		}
		else if (parm == "-PrintDst")
		{
			retParms.bPrintDst = true;
		}
		else if (parm == "-PrintBoth")
		{
			retParms.bPrintDst = retParms.bPrintSrc = true;
		}
	}

	return retParms;


}

int __cdecl main(int argc, char **argv)
{
	TITMParms Parms = ParseParameters(argc, argv);
	InitWinsock();
	
	Parms.ScriptPath = "c:\\temp\\MyScript.ps1";
	//TcpProxy TITMProxy("1234", "www.israelnationalnews.com", "80");
	TcpProxy TITMProxy(Parms, "443", "104.17.239.85", "443");

	try
	{
		TITMProxy.Start();
	}
	catch (std::runtime_error& e)
	{
		printf(e.what());
	}
	

	TITMProxy.Stop();





	WSACleanup();

	return 0;
}