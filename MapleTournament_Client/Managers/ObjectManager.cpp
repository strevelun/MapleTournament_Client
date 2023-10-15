#include "ObjectManager.h"

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
	m_arrSkill[(size_t)_type] = _pSkill;
}
