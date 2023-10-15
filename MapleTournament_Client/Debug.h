#pragma once

#include "Defines.h"

#include <string>

class Debug
{
public:
	static void Log(std::string _log);
	static void Log(std::wstring _log);
};

