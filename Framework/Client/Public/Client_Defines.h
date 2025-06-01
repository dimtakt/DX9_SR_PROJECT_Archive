#pragma once
#include "Client_Defines_Item.h"
#include "Client_Defines_Map.h"
#include "Client_Struct.h"
#include "../Default/framework.h"
#include <process.h>

namespace Client
{
	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;

	enum class LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_STAGE1,LEVEL_STAGE2, LEVEL_MAPEDIT, LEVEL_SHARED, LEVEL_END };
	enum class OBJECT_TYPE { TREE, ROCK, TYPE_END };
	enum class STAT_INFO { MAXHP, CULHP, MAXMP, CULMP, CULDEF, CULDAMAGE, CULCRITICAL, CRITICALDAMAGE, MAXDASH, CULDASH, REGENDASH, EXP, MAXSTATPOINT, CULSTATPOINT, LEVEL, GOLD, DICE, STAT_END };
	enum class EVENT_TYPE { PLAYERTSTATCHANGE, UICHANGE, EVENT_END };

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
		static constexpr float CHATERMAP_UP = 0.25f;
		static constexpr float CHATERMAP_Player = 0.24f;
	};

	typedef struct tagStatChangeEvent : public EVENTDATA
	{
		STAT_INFO	eStatType;
		float		fValue;
	}STATCHANGE;

	typedef struct tagActionEvent : public EVENTDATA
	{
		_wstring strActionName;
	};

};

extern short g_ScrollValue;
extern HWND g_hWnd;
extern HCURSOR g_hCursor;
using namespace Client;
