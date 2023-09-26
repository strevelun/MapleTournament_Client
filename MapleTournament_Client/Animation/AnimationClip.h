#pragma once

#include <string>
#include <vector>
#include "../Setting.h"

class Graphics;

class AnimationClip
{
private:
	std::wstring m_strName;
	std::vector<tAnimationFrame*> m_vecFrame;
	bool m_isLoop = false;
	bool m_isEnd = false;

	int m_curFrameIdx = 0;
	int m_clipSize = 0;

	float m_frameTime = 0.0f;
	float m_playTime = 0.0f;

	bool m_anyState = false;

public:
	AnimationClip(const std::wstring& m_strName, int _clipSize);
	~AnimationClip();

	void Update();
	void Render(Graphics* _pGraphics, const D2D1_RECT_F& _destRect);

	void SetLoop(bool _isLoop) { m_isLoop = _isLoop; }
	void SetPlayTime(float _playTime) { m_playTime = _playTime; }
	void SetAnyState(bool _anyState) { m_anyState = _anyState; }

	void Reset();

	bool IstAnyState() const { return m_anyState; }
	bool IsEnd() const { return m_isEnd; }

	void AddFrame(tAnimationFrame* _pFrame);
};

