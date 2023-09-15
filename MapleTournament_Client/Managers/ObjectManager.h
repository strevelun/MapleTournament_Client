#pragma once

#include <vector>

#include "../Defines.h"

class MyPlayer;
class Obj;

class ObjectManager
{
private:
	MyPlayer* m_pMyPlayer;
	std::vector<Obj*> m_vecObj;

public:
	bool Init();
	void SetMyPlayer(MyPlayer* _pMyPlayer) { m_pMyPlayer = _pMyPlayer; }
	MyPlayer* GetMyPlayer() const { return m_pMyPlayer; }

	void AddObj(Obj* _pObj);

	SINGLETON(ObjectManager)
};

