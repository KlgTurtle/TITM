#pragma once

#ifdef DEBUG_PRINTS
#define dbgprintf(x) printf(std::string(x).c_str())
#else
#define dbgprintf
#endif