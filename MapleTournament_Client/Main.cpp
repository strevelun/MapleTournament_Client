#include "GameApp.h"
#include <stdexcept>

#pragma comment( lib, "ws2_32.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
	int ret = 0;

	GameApp app(hInstance, nCmdShow, L"MapleTournament", L"메이플 토너먼트", 1920, 1080);

	try {
		app.Init();
		ret = app.Run();
	}
	catch (const std::wstring& e)
	{
		MessageBox(nullptr, e.c_str(), L"ERROR!", MB_OK);
		return -1;
	}

	return ret;
}

