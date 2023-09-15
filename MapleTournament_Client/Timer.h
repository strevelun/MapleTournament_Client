#pragma once

#include "Defines.h"
#include <Windows.h>

class Timer
{
private:
	DWORD m_tick = 0;
	float m_deltaTime = 0.f;
	unsigned int m_frame = 0, m_frameLimit = 0;
	float m_elapsedTime = 0.f;
	float m_fps = 0.f;
	float m_time = 0.f;

public:
	bool Init(unsigned int _frameLimit);
	bool Update();
	float GetDeltaTime() const { return m_deltaTime; }

	SINGLETON(Timer);
};

