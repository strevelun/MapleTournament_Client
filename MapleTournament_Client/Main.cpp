#include "GameApp.h"
#include "Constants.h"

#include <ctime>

#pragma comment( lib, "ws2_32.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
	srand((unsigned int)time(0));

	GameApp app(hInstance, L"MapleTournament");

	if (!app.Init(nCmdShow, L"메이플 토너먼트", ScreenWidth, ScreenHeight)) return -1;

	return app.Run();
}

