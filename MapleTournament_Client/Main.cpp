#include "GameApp.h"
#include "Constants.h"

#pragma comment( lib, "ws2_32.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
	int ret = 0;
	GameApp app(hInstance, L"MapleTournament");

	if (!app.Init(nCmdShow, L"������ ��ʸ�Ʈ", ScreenWidth, ScreenHeight)) return -1;
	ret = app.Run();

	return ret;
}

