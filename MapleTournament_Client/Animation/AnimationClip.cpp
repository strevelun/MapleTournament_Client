#include "AnimationClip.h"
#include "../GraphicCore/Graphics.h"
#include "../Timer.h"

AnimationClip::AnimationClip(const std::wstring& _strName, int _clipSize) :
	m_strName(_strName), m_clipSize(_clipSize)
{
	m_vecFrame.reserve(_clipSize);
}

AnimationClip::~AnimationClip()
{
	std::vector<tAnimationFrame*>::iterator iter = m_vecFrame.begin();
	std::vector<tAnimationFrame*>::iterator iterEnd = m_vecFrame.end();

	(*iter)->bitmap->Release();

	for (; iter != iterEnd; iter++)
		delete *iter;
}

void AnimationClip::Update()
{
	m_frameTime += Timer::GetInst()->GetDeltaTime();
	if (m_frameTime >= m_playTime / m_clipSize)
	{
		m_curFrameIdx++;
		if (!m_isLoop && m_curFrameIdx >= m_clipSize) 
			m_isEnd = true;
		m_curFrameIdx = m_curFrameIdx % m_clipSize;
		m_frameTime = 0.0f;
	}
}

void AnimationClip::Render(Graphics* _pGraphics, const D2D1_RECT_F& _destRect)
{
	_pGraphics->DrawBitmap(m_vecFrame[m_curFrameIdx]->bitmap, _destRect, m_vecFrame[m_curFrameIdx]->rect);
}

void AnimationClip::Reset()
{
	m_frameTime = 0.0f;
	m_curFrameIdx = 0.0f;
}

void AnimationClip::AddFrame(tAnimationFrame* _pFrame)
{
	m_vecFrame.push_back(_pFrame);
}
