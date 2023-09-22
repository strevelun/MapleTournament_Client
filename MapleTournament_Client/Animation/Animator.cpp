#include "Animator.h"
#include "AnimationClip.h"
#include "../GraphicCore/Graphics.h"

Animator::Animator(AnimationClip* _pDefaultClip)
	: m_pDefaultClip(_pDefaultClip),
	m_pCurClip(_pDefaultClip)
{
}

Animator::~Animator()
{
	std::map<std::wstring, AnimationClip*>::iterator iter = m_mapClip.begin();
	std::map<std::wstring, AnimationClip*>::iterator iterEnd = m_mapClip.end();
	for (; iter != iterEnd; iter++)
	{
		delete iter->second;
	}
}

void Animator::Update()
{
	m_pCurClip->Update();
}

void Animator::Render(Graphics* _pGraphics)
{
	m_pCurClip->Render(_pGraphics);
}

void Animator::AddClip(const std::wstring& _stateName, AnimationClip* _pClip)
{
	m_mapClip.insert(std::make_pair(_stateName, _pClip));
}