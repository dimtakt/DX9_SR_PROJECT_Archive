#pragma once
#include "../Default/framework.h"
#include "Engine_Defines.h"
#include <process.h>

namespace Client
{
	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;

	enum class LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_TOWN, LEVEL_STAGE1, LEVEL_STAGE2, LEVEL_STAGE3, LEVEL_STAGE4, LEVEL_BOSS1, LEVEL_SHELTER, LEVEL_BOSS2, LEVEL_MAPEDIT, LEVEL_END };
	enum class OBJECT_TYPE { TREE, ROCK, TYPE_END };
	enum class STAT_INFO { MAXHP, CULHP, MAXMP, CULMP, CULDEF, CULDAMAGE, CULCRITICAL, CRITICALDAMAGE, MAXDASH, CULDASH, REGENDASH, EXP, MAXSTATPOINT, CULSTATPOINT, LEVEL, GOLD, DICE, FURYREADY, LEVELUPPOINT, STAT_END };

	enum class ROOM_INFO { EVENT_NORMAL,  EVENT_SHOP, EVENT_HP, EVENT_EXP,  EVENT_ARTEFACT, EVENT_STONE, EVENT_ENDPOTAL, EVENT_BOSS, EVENT_END };
	enum class EVENT_TYPE { PLAYERTSTATCHANGE, UICHANGE, ROOMCHANGE, EXP, GOLD, FROZENHAMMER, GETITEM, EVENT_END };
	enum class POTAL_TYPE { LEFT, RIGHT, UP, DOWN, END_POTAL, STAGE_POTAL, BOSS_POTAL, POTAL_END };

	enum class DAMAGE { NORMAL, SPECIAL, DASH, PLANET, FROZENHAMMER, ICEBOLT, DAMAGE_END };

	enum class MONSTER_TYPE { MOLE, OINK, LASERGHOST, ERMA, ERMA_BODY, ERMA_HAND_L, ERMA_HAND_R, ERMA_HEAD, ASKARD, ASKARD_TENTACLE, ASKARD_TENTACLE_EX };

	enum class MONSTER_TYPE_A { MONSTER_DEFAULT, MONSTER_MOLE_A, MONSTER_OINK_A, MONSTER_LASERGHOST_D, MONSTER_BOSS_ERMA, MONSTER_BOSS_ERMA_BODY, MONSTER_BOSS_ERMA_HAND_L, MONSTER_BOSS_ERMA_HAND_R, MONSTER_BOSS_ERMA_HEAD, ERMA_HEAD, MONSTER_BOSS_ASKARD, MONSTER_BOSS_ASKARD_TENTACLE
	};

	// SOUND_BGM은 0으로 고정
	enum class CHANNELID { SOUND_BGM, SOUND_BACKGROUND, SOUND_PLAYER, SOUND_MONSTER, SOUND_EFFECT, SOUND_PLAYER_EFFECT, SOUND_MONSTER_EFFECT, SOUND_MONSTER_LONG_EFFECT, SOUND_MONSTER_LONG_EFFECT2, SOUND_MONSTER_LONG_EFFECT3, SOUND_OBJECT, SOUND_PLAYER_WALK, SOUND_UI, ITEM_EFFECT, ITEM_EFFECT2, ITEM_EFFECT3,  SOUND_END };

	enum class PARTICLE_TYPE { RAIN, FIRE, DUST, LIGHT, PARTICLE_END};

	struct UI_DEPTH {
		//KEY_GUIDE
		static constexpr float KEY_GUIDE = 0.f;

		//HUD
		static constexpr float HUD_DASH = 0.23f;
		static constexpr float HUD_BUFF = 0.23f;
		static constexpr float HUD_BUTTON = 0.23f;
		static constexpr float HUD_EXP = 0.23f;
		static constexpr float HUD_STATUS = 0.23f;
		static constexpr float HUD_WALLET = 0.23f;
		static constexpr float HUD_Qyick_Slot = 0.23f;
		
		//EVENT
		static constexpr float EVENT_AZ = 0.2f;

		//BOSSHP
		static constexpr float BOSSHP = 0.23f;

		//FILED
		static constexpr float FILED_HP = 0.3f;
		static constexpr float FILED_HP_FRAME = 0.4f;
		static constexpr float FILED_FONT = 0.29;
		static constexpr float FILED_ITEM = 0.31;
		static constexpr float FILED_CHAT = 0.31;


		//INVEN
		static constexpr float INVEN_SLOT_SELETE = 0.1f;
		static constexpr float INVENTORY = 0.2f;
		static constexpr float INVEN_SLOT = 0.2f;

		//Gahcah
		static constexpr float GACHA = 0.2f;
		static constexpr float GACHA_SLOT = 0.2f;

		//CHATERMAP
		static constexpr float CHATERMAP = 0.26f;
		static constexpr float CHATERMAP_BUTTON = 0.24f;
		static constexpr float CHATERMAP_UP = 0.245f;
		static constexpr float CHATERMAP_PLAYER = 0.235f;
		static constexpr float CHATERMAP_LINE = 0.242f;

		//PLAYER_STAUTS
		static constexpr float PLAYER_STAUTS = 0.2f;

		//PLAYER_STAUTS
		static constexpr float TALENT = 0.2f;
		static constexpr float TALENT_Selete = 0.19f;

		//Minimap
		static constexpr float MiniMap = 0.18f;
		static constexpr float MINIMAP_BUTTON = 0.17f;

		//Tooltip
		static constexpr float TOOLTIP = 0.f;

		//Loding
		static constexpr float LODING = 0.2f;

	};

	inline _float Lerp(_float a, _float b, _float t)
	{
		return a + (b - a) * t;
	}

	inline _float3 Lerp(const _float3& a, const _float3& b, _float t)
	{
		return {
			a.x + (b.x - a.x) * t,
			a.y + (b.y - a.y) * t,
			a.z + (b.z - a.z) * t
		};
	}

	inline _float EaseInOutSine(_float t)
	{
		return -(cosf(D3DX_PI * t) - 1.f) * 0.5f;
	}
};

extern short g_ScrollValue;
extern HWND g_hWnd;
extern HCURSOR g_hCursor;
extern float g_fBGMVolume;
extern float g_fEFFECTVolume;
extern float g_fWALKVolume;
extern float g_fUIVolume;
using namespace Client;
