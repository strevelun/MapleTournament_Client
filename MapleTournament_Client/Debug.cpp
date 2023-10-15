#include "Debug.h"
#include <Windows.h>

void Debug::Log(std::string _log)
{
	_log += "\n";
	OutputDebugStringA(_log.c_str());
}

void Debug::Log(std::wstring _log)
{
	_log += L"\n";
	OutputDebugStringW(_log.c_str());
}
