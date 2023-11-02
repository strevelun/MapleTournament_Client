#include "Timer.h"
#include <iostream>
#pragma comment(lib, "Winmm.lib")

Timer* Timer::m_pInst = nullptr;

Timer::Timer()
{
}

Timer::~Timer()
{
}

bool Timer::Init(unsigned int _frameLimit)
{
	m_frameLimit = _frameLimit;
	m_tick = timeGetTime();
	return true;
}

bool Timer::Update()
{
	DWORD tick = timeGetTime();

	m_deltaTime = (float)(tick - m_tick) / 1000;
	m_time += m_deltaTime;
	m_tick = tick;

	m_elapsedTime += m_deltaTime;
	if (m_frameLimit > 0)
		if (m_time < (1.0f / m_frameLimit))
		{
			return false;
		}

	m_time = 0;
	m_frame++;

	if (m_elapsedTime >= 1.0f)
	{
		m_fps = m_frame / m_elapsedTime;
		m_frame = 0;
		m_elapsedTime = 0;
	}
	return true;
}
