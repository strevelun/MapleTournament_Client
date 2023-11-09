#include "ObjectManager.h"
#include "../Obj/Obj.h"

ObjectManager* ObjectManager::m_pInst = nullptr;

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

bool ObjectManager::Init()
{
	return true;
}

void ObjectManager::AddObj(Obj* _pObj)
{
	m_vecObj.push_back(_pObj);
}

void ObjectManager::AddSkill(Skill* _pSkill, eSkillName _name)
{
	if (_name == eSkillName::None || _name == eSkillName::NumOfSkills) return;

	m_arrSkill[(size_t)_name] = _pSkill;
}

Obj* ObjectManager::FindObj(const std::wstring& _strName)
{
	std::vector<Obj*>::iterator iter = m_vecObj.begin();
	std::vector<Obj*>::iterator iterEnd = m_vecObj.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == _strName)
			return (*iter);
	}
	return nullptr;
}

Skill* ObjectManager::FindSkill(eSkillName _type)
{
	return m_arrSkill[(size_t)_type];
}

void ObjectManager::KillObj(const std::wstring& _strName)
{
	Obj* pObj = FindObj(_strName);
	if (!pObj) return;

	pObj->SetAlive(false);
}