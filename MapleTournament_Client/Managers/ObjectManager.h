#pragma once

#include <vector>
#include <array>
#include <string>

#include "../Defines.h"
#include "../Setting.h"

class MyPlayer;
class Obj;
class Skill;

class ObjectManager
{
private:
	MyPlayer* m_pMyPlayer;
	std::vector<Obj*> m_vecObj;
	std::array<Skill*, (size_t)eSkillType::NumOfSkills> m_arrSkill;

public:
	bool Init();
	void SetMyPlayer(MyPlayer* _pMyPlayer) { m_pMyPlayer = _pMyPlayer; }
	MyPlayer* GetMyPlayer() const { return m_pMyPlayer; }

	void AddObj(Obj* _pObj);
	void AddSkill(Skill* _pSkill, eSkillType _type);

	Obj* FindObj(const std::wstring& _strName);
	void KillObj(const std::wstring& _strName);

	SINGLETON(ObjectManager)
};

