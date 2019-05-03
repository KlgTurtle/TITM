#include "HexPrinter.h"
#include <algorithm>
HexPrinter::HexPrinter(FILE * Dest) : m_Dest(Dest), m_bInterlocked(false)
{
	
}

HexPrinter::~HexPrinter()
{
	if (m_bInterlocked)
	{
		DeleteCriticalSection(&m_LockHandle);
	}
}

HexPrinter::HexPrinter(FILE* Dest, bool bInterlocked) : m_Dest(Dest), m_bInterlocked(bInterlocked)
{
	if (m_bInterlocked)
	{
		InitializeCriticalSection(&m_LockHandle);
	}
}

void HexPrinter::Print(std::vector<char> Buffer)
{
	if (m_bInterlocked)
	{
		EnterCriticalSection(&m_LockHandle);
	}

	size_t LineOffset = 0;
	size_t TotalLines = Buffer.size() / BytesPerLine;
	if (TotalLines == 0 && Buffer.size() > 0)
	{
		TotalLines = 1;
	}

	for (size_t LineNum = 0; LineNum < TotalLines; ++LineNum)
	{
		size_t NumOfBytesInThisLine = std::min(BytesPerLine, Buffer.size() - LineOffset);
		printf("%08X  ", LineOffset);
		for (size_t i = 0; i < NumOfBytesInThisLine; ++i)
		{
			printf("%02X ", static_cast<unsigned char>(Buffer[LineOffset + i]));
		}
		for (size_t i = 0; i < BytesPerLine - NumOfBytesInThisLine; ++i)
		{
			printf("   ");
		}
		printf("   ");
		for (size_t i = 0; i < NumOfBytesInThisLine; ++i)
		{
			if ((Buffer[LineOffset + i] > 31) && (Buffer[LineOffset + i] < 127))
			{
				printf("%c", Buffer[LineOffset + i]);
			}
			else
			{
				printf(".");
			}
		}
		printf("\n");

		LineOffset += NumOfBytesInThisLine;
	}

	if (m_bInterlocked)
	{
		LeaveCriticalSection(&m_LockHandle);
	}
}
