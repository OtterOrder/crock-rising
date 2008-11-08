#include <windows.h>
#include "System.h"

//******************************************************************

int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow )
{
	return System::GetInstance()->MainLoop();

}
