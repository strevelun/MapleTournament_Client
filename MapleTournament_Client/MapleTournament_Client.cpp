#include "App.h"
#include "Setting.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (App::GetInst()->Init())
		return 0;

	int ret = App::GetInst()->Run();
	App::DestroyInst();
	return ret;
}

