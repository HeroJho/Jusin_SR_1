#pragma once

namespace Client
{
	static const unsigned int g_iWinSizeX = 1280;
	static const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_LOAIDNG, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };
}

extern HWND g_hWnd;

using namespace Client;