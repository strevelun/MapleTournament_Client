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

void ObjectManager::AddSkill(Skill* _pSkill, eSkillType _type)
{
	if (_type == eSkillType::None || _type == eSkillType::NumOfSkills) return;
	if (m_arrSkill[(size_t)_type]) delete m_arrSkill[(size_t)_type];

	m_arrSkill[(size_t)_type] = _pSkill;
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

Skill* ObjectManager::FindSkill(eSkillType _type)
{
	return nullptr;
}

void ObjectManager::KillObj(const std::wstring& _strName)
{
	Obj* pObj = FindObj(_strName);
	if (!pObj) return;

	pObj->SetAlive(false);
}