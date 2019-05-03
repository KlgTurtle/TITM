#pragma once

#include <stdio.h>
#include <vector>
#define NOMINMAX
#include <WinSock2.h>
class HexPrinter
{
public:
	HexPrinter(FILE* Dest);
	HexPrinter(FILE* Dest, bool bInterlocked);
	virtual ~HexPrinter();

	virtual void Print(std::vector<char> Buffer);

protected:
	FILE* m_Dest;
	CRITICAL_SECTION m_LockHandle;
	bool  m_bInterlocked;

	static const size_t BytesPerLine = 16;
};