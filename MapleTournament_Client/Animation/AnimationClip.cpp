#include "AnimationClip.h"
#include "../GraphicCore/Graphics.h"
#include "../Timer.h"
#include "../Debug.h"
#include "../Bitmap.h"

AnimationClip::AnimationClip(Bitmap* _pBitmap, std::vector<tAnimationFrame*>* _pVecFrame) :
	m_pBitmap(_pBitmap), m_pVecFrame(_pVecFrame)
{
	m_clipSize = _pVecFrame->size();
}

AnimationClip::AnimationClip(const AnimationClip& _clip) :
	m_pBitmap(_clip.m_pBitmap), m_isLoop(_clip.m_isLoop), m_isEnd(_clip.m_isEnd), m_curFrameIdx(_clip.m_curFrameIdx),
	m_clipSize(_clip.m_clipSize), m_frameTime(_clip.m_frameTime), m_playTime(_clip.m_playTime), m_anyState(_clip.m_anyState),
	m_isFlip(_clip.m_isFlip), m_pNextClip(_clip.m_pNextClip)
{
}

AnimationClip::~AnimationClip()
{
}

void AnimationClip::Update()
{
	if (m_isEnd) return;

	m_frameTime += Timer::GetInst()->GetDeltaTime();
	//Debug::Log(std::to_string(m_frameTime));
	if (m_frameTime >= m_playTime / m_clipSize)
	{
		//Debug::Log(std::to_string(m_frameTime) + " >= " + std::to_string(m_playTime / m_clipSize));
		m_curFrameIdx++;
		if (!m_isLoop && m_curFrameIdx >= m_clipSize)
		{
			if (m_curLoopCount >= m_loopCountMax - 1)
			{
				m_isEnd = true;
				return;
			}
			m_curLoopCount++;
		}
		m_curFrameIdx = m_curFrameIdx % m_clipSize;
		m_frameTime = 0.0f;
	}
}

void AnimationClip::Render(float _xpos, float _ypos, float _ratio)
{
	if (m_isEnd) return;

	ID2D1HwndRenderTarget* pRenderTarget = Graphics::GetInst()->GetRenderTarget();

	float pivotX = ((*m_pVecFrame)[m_curFrameIdx]->pivotX * (*m_pVecFrame)[m_curFrameIdx]->size.width);
	float pivotY = ((*m_pVecFrame)[m_curFrameIdx]->pivotY * (*m_pVecFrame)[m_curFrameIdx]->size.height);
	float adjustedX = _xpos / _ratio;
	float adjustedY = _ypos / _ratio;

	D2D1_RECT_F renderRect = 
	{
		(adjustedX - pivotX) * _ratio,
		(adjustedY - pivotY) * _ratio,
		(adjustedX + (*m_pVecFrame)[m_curFrameIdx]->size.width - pivotX) * _ratio,
		(adjustedY + (*m_pVecFrame)[m_curFrameIdx]->size.height - pivotY) * _ratio
	};

	if (m_isFlip) pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(-1.0f, 1.0f, D2D1::Point2F(_xpos, _ypos)));

	pRenderTarget->DrawBitmap(m_pBitmap->GetBitmap(), renderRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, (*m_pVecFrame)[m_curFrameIdx]->rect);

	if (m_isFlip) pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void AnimationClip::Reset()
{
	m_curLoopCount = 0;
	m_frameTime = 0.0f;
	m_curFrameIdx = 0;
	m_isEnd = false;
}
