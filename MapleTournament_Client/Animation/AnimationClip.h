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
	std::vector<tAnimationFrame*>* m_pVecFrame = nullptr;

	AnimationClip* m_pNextClip = nullptr;

	int m_loopCountMax = 1;
	int m_curLoopCount = 0;

	bool m_isLoop = false;
	bool m_isEnd = false;

	int m_curFrameIdx =0;
	int m_clipSize = 0;

	float m_frameTime = 0.0f;
	float m_playTime = 0.0f;

	bool m_anyState = false;
	bool    m_isFlip = false;

public:
	AnimationClip(Bitmap* _pBitmap, std::vector<tAnimationFrame*>* _pVecFrame);
	AnimationClip(const AnimationClip& _clip);
	~AnimationClip();

	void Update();
	void Render(float _xpos, float _ypos, float _ratio);

	void SetLoopCount(int _loopCount) { m_loopCountMax = _loopCount; }
	void SetLoop(bool _isLoop) { m_isLoop = _isLoop; }
	void SetPlayTime(float _playTime) { m_playTime = _playTime; }
	/// <summary>
	/// 현재 클립이 재생되는 도중에 다른 클립으로 전환 가능여부
	/// </summary>
	void SetAnyState(bool _anyState) { m_anyState = _anyState; }
	void SetFlip(bool _isFlip) { m_isFlip = _isFlip; }
	void SetNextClip(AnimationClip* _pClip) { m_pNextClip = _pClip; }

	AnimationClip* GetNextClip() const { return m_pNextClip; }

	void Reset();

	bool IstAnyState() const { return m_anyState; }
	bool IsEnd() const { return m_isEnd; }
};

