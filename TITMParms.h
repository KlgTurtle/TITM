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