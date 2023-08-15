#include "GameApp.h"
#include <stdexcept>

#pragma comment( lib, "ws2_32.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
	int ret = 0;
	GameApp app(hInstance, L"MapleTournament");

	if (!app.Init(nCmdShow, L"메이플 토너먼트", 1920, 1080)) return -1;
	ret = app.Run();

	return ret;
}

