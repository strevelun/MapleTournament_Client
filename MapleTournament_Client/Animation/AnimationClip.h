#pragma once

#include <string>
#include <vector>
#include "../Setting.h"

class Graphics;
class Bitmap;

class AnimationClip
{
private:
	Bitmap* m_pBitmap = nullptr;
	std::vector<tAnimationFrame*> m_vecFrame;
	bool m_isLoop = false;
	bool m_isEnd = false;

	int m_curFrameIdx = 0;
	int m_clipSize = 0;

	float m_frameTime = 0.0f;
	float m_playTime = 0.0f;

	bool m_anyState = false;
	bool    m_isFlip = false;

public:
	AnimationClip(Bitmap* _pBitmap, int _clipSize);
	~AnimationClip();

	void Update();
	void Render(float _xpos, float _ypos, float _ratio);

	void SetLoop(bool _isLoop) { m_isLoop = _isLoop; }
	void SetPlayTime(float _playTime) { m_playTime = _playTime; }
	void SetAnyState(bool _anyState) { m_anyState = _anyState; }
	void SetFlip(bool _isFlip) { m_isFlip = _isFlip; }

	void Reset();

	bool IstAnyState() const { return m_anyState; }
	bool IsEnd() const { return m_isEnd; }

	void AddFrame(tAnimationFrame* _pFrame);
};

