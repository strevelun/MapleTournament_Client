#pragma once

#include <queue>
#include <functional>

#include "Defines.h"

class InstructionQueue
{
private:
	std::queue<std::function<void()>> m_instruction;

public:
	bool Init();
	void Push(std::function<void()> _instFunc);
	void Update();

	SINGLETON(InstructionQueue)
};

