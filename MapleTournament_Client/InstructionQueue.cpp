#include "InstructionQueue.h"

InstructionQueue* InstructionQueue::m_pInst = nullptr;

InstructionQueue::InstructionQueue()
{
}

InstructionQueue::~InstructionQueue()
{
}

bool InstructionQueue::Init()
{
	return true;
}

void InstructionQueue::Push(std::function<void()> _instFunc)
{
	m_instruction.push(_instFunc);
}

void InstructionQueue::Update()
{
	while (!m_instruction.empty())
	{
		auto& inst = m_instruction.front();
		inst();
		m_instruction.pop();
	}
}
