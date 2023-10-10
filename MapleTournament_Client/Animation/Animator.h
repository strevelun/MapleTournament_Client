#pragma once

#include <map>
#include <string>
#include <d2d1.h>

class Graphics;
class AnimationClip;

class Animator
{
private:
	std::map<std::wstring, AnimationClip*> m_mapClip;

	AnimationClip* m_pDefaultClip = nullptr;
	AnimationClip* m_pCurClip = nullptr;
	AnimationClip* m_pNextClip = nullptr;

public:
	Animator(AnimationClip* _pDefaultClip);
	~Animator();

	void Update();
	void Render(Graphics* _pGraphics, UINT _xpos, UINT _ypos, float _ratio);

	void AddClip(const std::wstring& _stateName, AnimationClip* _pClip);
	void SetNextClip(const std::wstring& _stateName);
};

