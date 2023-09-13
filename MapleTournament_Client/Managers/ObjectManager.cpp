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
