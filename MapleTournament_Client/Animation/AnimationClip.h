#pragma once

#include <vector>
#include "../Setting.h"

class Graphics;

class AnimationClip
{
private:
	std::vector<tAnimationFrame*> m_vecFrame;
	bool m_isLoop = false;

public:
	AnimationClip();
	~AnimationClip();

	void Update();
	void Render(Graphics* _pGraphics);

	void SetLoop(bool _isLoop) { m_isLoop = _isLoop; }
};

