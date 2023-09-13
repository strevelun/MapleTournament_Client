#pragma once

#include <vector>

class MyPlayer;
class Obj;

class ObjectManager
{
private:
	static ObjectManager* m_pInst;

	MyPlayer* m_pMyPlayer;
	std::vector<Obj*> m_vecObj;

	ObjectManager();
	~ObjectManager();

public:
	static ObjectManager* GetInst()
	{
		if (!m_pInst) m_pInst = new ObjectManager;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst) delete m_pInst;
		m_pInst = nullptr;
	}

	bool Init();
	void SetMyPlayer(MyPlayer* _pMyPlayer) { m_pMyPlayer = _pMyPlayer; }
	MyPlayer* GetMyPlayer() const { return m_pMyPlayer; }

	void AddObj(Obj* _pObj);
};

