#include "Animator.h"
#include "AnimationClip.h"
#include "../GraphicCore/Graphics.h"
#include "../Debug.h"

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
	if (m_pNextClip && m_pNextClip->IstAnyState())
	{
		m_pCurClip->Reset();
		m_pCurClip = m_pNextClip;
	}

	m_pCurClip->Update();

	if (m_pCurClip->IsEnd())
	{
		if (m_pNextClip)
		{
			m_pCurClip = m_pNextClip;
			m_pNextClip = nullptr;
		}
		else
			m_pCurClip = m_pDefaultClip; // 유니티는 마지막 프레임에서 멈춘상태로 정지
	}
}

void Animator::Render(UINT _xpos, UINT _ypos, float _ratio)
{
	m_pCurClip->Render(_xpos, _ypos, _ratio);
}

void Animator::AddClip(const std::wstring& _stateName, AnimationClip* _pClip)
{
	m_mapClip.insert(std::make_pair(_stateName, _pClip));
}

void Animator::SetNextClip(const std::wstring& _stateName)
{
	std::map<std::wstring, AnimationClip*>::iterator iter = m_mapClip.find(_stateName);
	if (iter == m_mapClip.end())
	{
		Debug::Log(_stateName + L"클립을 찾을 수 없음!");
		return;
	}
	m_pNextClip = iter->second;
}
