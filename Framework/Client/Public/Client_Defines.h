#pragma once

#include "../Default/framework.h"
#include <process.h>

namespace Client
{
	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;

	enum class LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_STAGE1,LEVEL_STAGE2, LEVEL_MAPEDIT, LEVEL_SHARED, LEVEL_END };
	enum class OBJECT_TYPE { TREE, ROCK, TYPE_END };

	struct UI_DEPTH {
		//키 가이드
		static constexpr float KEY_GUIDE = 0.15f;

		//HUD
		static constexpr float HUD_DASH = 0.25f;
		static constexpr float HUD_BUFF = 0.25f;
		static constexpr float HUD_BUTTON = 0.25f;
		static constexpr float HUD_EXP = 0.25f;
		static constexpr float HUD_STATUS = 0.25f;
		static constexpr float HUD_WALLET = 0.25f;

		//필드
		static constexpr float FILED_HP = 0.3f;
		static constexpr float FILED_HP_FRAME = 0.4f;

		//인벤
		static constexpr float INVEN_SLOT_SELETE = 0.1f;
		static constexpr float INVENTORY = 0.2f;
		static constexpr float INVEN_SLOT = 0.2f;

		//챕터 지도
		static constexpr float CHATERMAP = 0.26f;
		static constexpr float CHATERMAP_BUTTON = 0.24f;
		static constexpr float CHATERMAP_UP = 0.245f;
		static constexpr float CHATERMAP_PLAYER = 0.235f;
		static constexpr float CHATERMAP_LINE = 0.242f;

		//플레이어 스테이터스 창
		static constexpr float PLAYER_STAUTS = 0.2f;
	};

};

extern short g_ScrollValue;
extern HWND g_hWnd;
extern HCURSOR g_hCursor;
using namespace Client;
