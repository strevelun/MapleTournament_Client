#pragma once

#include <vector>
#include <array>
#include <string>

#include "../Defines.h"
#include "../Setting.h"

class Player;
class Obj;
class Skill;

class ObjectManager
{
private:
	std::vector<Obj*> m_vecObj;
	std::array<Skill*, (size_t)eSkillName::NumOfSkills> m_arrSkill;

public:
	bool Init();

	void AddObj(Obj* _pObj);
	void AddSkill(Skill* _pSkill, eSkillName _name);

	Obj* FindObj(const std::wstring& _strName);
	Skill* FindSkill(eSkillName _type);
	void KillObj(const std::wstring& _strName);
	void ClearAll() { m_vecObj.clear(); }

	SINGLETON(ObjectManager)
};

