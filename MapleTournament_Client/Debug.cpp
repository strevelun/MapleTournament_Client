#include "Debug.h"
#include <Windows.h>

void Debug::Log(std::wstring _log)
{
	_log += L"\n";
	OutputDebugStringW(_log.c_str());
}
