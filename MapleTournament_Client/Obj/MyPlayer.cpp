#include "MyPlayer.h"

MyPlayer::MyPlayer(const wchar_t* _pNickname)
	: Player(_pNickname)
{
}

MyPlayer::~MyPlayer()
{
}

void MyPlayer::Update()
{
	Player::Update();
}

void MyPlayer::Render()
{
	Player::Render();
}
