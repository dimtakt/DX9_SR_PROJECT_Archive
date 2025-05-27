#pragma once

#include "../Default/framework.h"
#include <process.h>

namespace Client
{
	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;

	enum class LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_STAGE1, LEVEL_MAPEDIT, LEVEL_SHARED, LEVEL_END };
	enum class OBJECT_TYPE { TREE, ROCK, TYPE_END };
}

extern HWND g_hWnd;
extern HCURSOR g_hCursor;
using namespace Client;