#pragma once

#include <process.h>
#include "../Default/framework.h"

#include "Client_Enum.h"

namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { STATIC, LOADING, LOGO, EDITOR, GAMEPLAY, TUTORIAL, END };
}

using namespace Client;

extern HWND g_hWnd;