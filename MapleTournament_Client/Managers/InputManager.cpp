#include "InputManager.h"
#include "../Obj/UI/Mouse.h"

InputManager* InputManager::m_pInst = nullptr;

InputManager::InputManager() :
	m_hWnd(nullptr), m_pMouse(nullptr)
{
}

InputManager::~InputManager()
{
 
}

bool InputManager::Init(HWND _hWnd)
{
	m_hWnd = _hWnd;
	m_pMouse = new Mouse(_hWnd);

	return true;
}

void InputManager::Update()
{
	
	m_pMouse->Update();
	ClearVecKeyInput();
}

void InputManager::RegisterVKKey(int _vkKey)
{
}

void InputManager::AddChar(WPARAM _input)
{
	m_vecKeyInput.push_back(_input);
}
