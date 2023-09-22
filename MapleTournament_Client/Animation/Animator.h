#pragma once

#include <map>
#include <string>

class Graphics;
class AnimationClip;

class Animator
{
private:
	std::map<std::wstring, AnimationClip*> m_mapClip;

	AnimationClip* m_pDefaultClip = nullptr;
	AnimationClip* m_pCurClip = nullptr;

public:
	Animator(AnimationClip* _pDefaultClip);
	~Animator();

	void Update();
	void Render(Graphics* _pGraphics);

	void AddClip(const std::wstring& _stateName, AnimationClip* _pClip);
};

