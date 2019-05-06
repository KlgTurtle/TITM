#pragma once
#include <memory>
#include "HexPrinter.h"
struct TITMParms
{
public:
	TITMParms() : ScriptPath("")
	{
		Printer = std::make_shared<HexPrinter>(stdout, true);
	}
	bool bPrintSrc = false;
	bool bPrintDst = false;
	std::shared_ptr<HexPrinter> Printer;
	std::string                 ScriptPath;
};

// This is because the maximum parameter length for the command line is 8191
static const int MAX_DATA_BUFFER_SIZE = 4000;