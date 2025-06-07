#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
namespace Client
{
	enum class ITEM_TYPE { ARTEFACT, STONE, POTION, SKILLBOOK, ITEM_TYPE_END };

	enum class ITEM_RARITY { NORMAL, RARE, EPIC, LEGENDARY, ITEM_RARITY_END };

	enum class ITEM_INFO { ITEM_TYPE, ITEM_VALUE, ITEM_COUNT, ITEM_INFO_END };

	enum class ITEM_EFFECT { VALUE_TYPE, SPAWN_TYPE, SKILLBOOK_TYPE, ITEM_EFFECT_END };

	struct ItemData
	{
	public:
		_int			m_iItemID;
		_int			m_iItemTextureID;
		_wstring		m_szName;
		_wstring		m_szDescription;
		ITEM_TYPE		m_eType;
		ITEM_RARITY		m_eRarity;
		_int			m_iItemValue;		//효과 관련 수치 값 기능 별로 다르게 적용 예정
		_int			m_iARTEFACT_Value;		//(아티팩트 맥스 레벨, 포션 효과 값, 석판은 효과 인덱스 값)

	public:
		ItemData(_int iItemID, _int iItemTextureID, ITEM_TYPE eType, ITEM_RARITY eRarity,
			_wstring szName, _wstring szDescription, _int iItemValue, _int iARTEFACT_Value = 99) :
			m_iItemID(iItemID), m_iItemTextureID(iItemTextureID), m_eType(eType), m_eRarity(eRarity),
			m_szName(szName), m_szDescription(szDescription), m_iItemValue(iItemValue), m_iARTEFACT_Value(iARTEFACT_Value){
		};
	};

	struct Item_Effect
	{
		struct TypeValue
		{
			STAT_INFO		m_eStat;					//올릴 스탯 타입
			_float			m_fStat_Value1;				//올릴 스탯량
			_float			m_fStat_Value2;				//1별마다 올라갈 스탯량
		};

		_int				m_iIndex;					//효과 번호
		ITEM_EFFECT			m_eType;					//효과 타입
		vector<TypeValue>	m_vecValue;					//올라갈 스탯 배열
		
		_wstring			m_szEffectTag;					//부여할 아이템 효과(망치, 행성, 스킬북

	public:
		Item_Effect(_int iIndex, ITEM_EFFECT eType, vector<TypeValue> vecValue, _wstring m_szEffectTag = TEXT("")) : m_iIndex(iIndex), m_eType(eType), m_vecValue(vecValue), m_szEffectTag(m_szEffectTag) {};
	};

	struct Slate
	{
		struct PosValue										//넣어줄 값들을 모두 소유
		{
			_int m_iX;
			_int m_iY;
			_int m_iValue;
			_int m_iPos;
		};

		_int				m_iSlateIndex;				//석판 번호
		_bool				m_bRelative;				//false 고정 좌표, true 유동 좌표
		_bool				m_bRotation;				//false 회전 불가, true 회전 가능

		vector<PosValue>	m_vecGardeValue;

	public:
		Slate(_int iSlateIndex, _bool bRelative, _bool bRotation, vector<PosValue> vecGardeValue) :
			m_iSlateIndex(iSlateIndex), m_bRelative(bRelative), m_bRotation(bRotation), m_vecGardeValue(vecGardeValue) {
		};
	};

	struct Potion
	{
		_int			m_iPotionIndex;
		_int			m_iPotionType;
		_int			m_iValue;
		_uint			m_iMaxPotion;
		
	public:
		Potion(_int m_iPotionIndex, _int m_iPotionType, _int m_iValue, _int m_iMaxPotion) :
			m_iPotionIndex(m_iPotionIndex), m_iPotionType(m_iPotionType), m_iValue(m_iValue), m_iMaxPotion(m_iMaxPotion) {};
	};


	const vector<ItemData> g_ItemDataBase
	{
		ItemData(0, 0, ITEM_TYPE::ARTEFACT, ITEM_RARITY::LEGENDARY, TEXT("아그마 투영검"), TEXT("공격력"), 7, 0),
		ItemData(1, 1, ITEM_TYPE::ARTEFACT, ITEM_RARITY::RARE,TEXT("금빛 망토"), TEXT("대쉬 횟수"), 2, 1),
		ItemData(2, 2, ITEM_TYPE::ARTEFACT, ITEM_RARITY::LEGENDARY,TEXT("스타 루비"), TEXT("최대 HP"), 2, 2),
		ItemData(3, 3,ITEM_TYPE::POTION, ITEM_RARITY::NORMAL,TEXT("재생의 포션"), TEXT("HP 20% 회복"), 0),
		ItemData(4, 4,ITEM_TYPE::POTION, ITEM_RARITY::RARE,TEXT("큰 재샙의 포션"), TEXT("HP 50% 회복"), 1),
		ItemData(5, 5,ITEM_TYPE::POTION, ITEM_RARITY::RARE,TEXT("마나 재생의 포션"), TEXT("MP 30% 회복"), 2),
		ItemData(6, 6,ITEM_TYPE::ARTEFACT, ITEM_RARITY::NORMAL,TEXT("배고니아 향 주머니"), TEXT("공격력"), 1, 3),
		ItemData(7, 7,ITEM_TYPE::ARTEFACT, ITEM_RARITY::NORMAL,TEXT("방패술 교본"), TEXT("공격력"), 1, 4),
		ItemData(8, 8,ITEM_TYPE::STONE,	  ITEM_RARITY::NORMAL,TEXT("건조"), TEXT(""), 0),
		ItemData(9, 9,ITEM_TYPE::STONE,   ITEM_RARITY::NORMAL,TEXT("근사"), TEXT(""), 1),
		ItemData(10, 10,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("기사도"), TEXT(""), 2),
		ItemData(11, 11,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("도래"), TEXT(""), 3),
		ItemData(12, 12,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("시선"), TEXT(""), 4),
		ItemData(13, 13,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("악수"), TEXT(""), 5),
		ItemData(14, 14,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("운명"), TEXT(""), 6),
		ItemData(15, 15,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("재치"), TEXT(""), 7),
		ItemData(16, 16,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("착취"), TEXT(""), 8),
		ItemData(17, 17,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("화합"), TEXT(""), 9),
		ItemData(18, 18,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("환호"), TEXT(""), 10),
		ItemData(19, 19,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("희망"), TEXT(""), 11),
		ItemData(20, 20,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("경쟁"), TEXT(""), 12),
		ItemData(21, 21,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("고동"), TEXT(""), 13),
		ItemData(22, 22,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("미래"), TEXT(""), 14),
		ItemData(23, 23,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("분배"), TEXT(""), 15),
		ItemData(24, 24,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("삼두"), TEXT(""), 16),
		ItemData(25, 25,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("수확"), TEXT(""), 17),
		ItemData(26, 26,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("쌍성"), TEXT(""), 18),
		ItemData(27, 27,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("양육"), TEXT(""), 19),
		ItemData(28, 28,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("열망"), TEXT(""), 20),
		ItemData(29, 29,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("응집"), TEXT(""), 21),
		ItemData(30, 30,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("입구"), TEXT(""), 22),
		ItemData(31, 31,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("장난"), TEXT(""), 23),
		ItemData(32, 32,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("적재"), TEXT(""), 24),
		ItemData(33, 33,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("전이"), TEXT(""), 25),
		ItemData(34, 34,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("전진"), TEXT(""), 26),
		ItemData(35, 35,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("정의"), TEXT(""), 27),
		ItemData(36, 36,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("준비"), TEXT(""), 28),
		ItemData(37, 37,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("파도"), TEXT(""), 29),
		ItemData(38, 38,ITEM_TYPE::STONE, ITEM_RARITY::RARE,TEXT("헌정"), TEXT(""), 30),
		ItemData(39, 39,ITEM_TYPE::STONE, ITEM_RARITY::EPIC,TEXT("권능"), TEXT(""), 31),
		ItemData(40, 40,ITEM_TYPE::STONE, ITEM_RARITY::EPIC,TEXT("기반"), TEXT(""), 32),
		ItemData(41, 41,ITEM_TYPE::STONE, ITEM_RARITY::EPIC,TEXT("동시성"), TEXT(""), 33),
		ItemData(42, 42,ITEM_TYPE::STONE, ITEM_RARITY::EPIC,TEXT("맹세"), TEXT(""), 34),
		ItemData(43, 43,ITEM_TYPE::STONE, ITEM_RARITY::EPIC,TEXT("반항"), TEXT(""), 35),
		ItemData(44, 44,ITEM_TYPE::STONE, ITEM_RARITY::EPIC,TEXT("차양"), TEXT(""), 36),
		ItemData(45, 45,ITEM_TYPE::STONE, ITEM_RARITY::LEGENDARY,TEXT("경계"), TEXT(""), 37),
		ItemData(46, 46,ITEM_TYPE::STONE, ITEM_RARITY::LEGENDARY,TEXT("가시"), TEXT(""), 38),
		ItemData(47, 47,ITEM_TYPE::STONE, ITEM_RARITY::LEGENDARY,TEXT("광휘"), TEXT(""), 39),
		ItemData(48, 48,ITEM_TYPE::STONE, ITEM_RARITY::LEGENDARY,TEXT("기적"), TEXT(""), 40),
		ItemData(49, 49,ITEM_TYPE::STONE, ITEM_RARITY::LEGENDARY,TEXT("백일몽"), TEXT(""), 41),
		ItemData(50, 50,ITEM_TYPE::STONE, ITEM_RARITY::LEGENDARY,TEXT("압축"), TEXT(""), 42),
		ItemData(51, 51,ITEM_TYPE::STONE, ITEM_RARITY::LEGENDARY,TEXT("과거"), TEXT(""), 43),
		ItemData(52, 52,ITEM_TYPE::STONE, ITEM_RARITY::LEGENDARY,TEXT("단절"), TEXT(""), 44),
		ItemData(53, 54,ITEM_TYPE::ARTEFACT, ITEM_RARITY::NORMAL,TEXT("부정한 붕대"), TEXT("치명타 확률, 공격력"), 3, 5),
		ItemData(54, 55,ITEM_TYPE::ARTEFACT, ITEM_RARITY::NORMAL,TEXT("압박 밴드"), TEXT("대시 회복 속도"), 2, 6),
		ItemData(55, 56,ITEM_TYPE::ARTEFACT, ITEM_RARITY::RARE,TEXT("열망의 부적"), TEXT("치명타 확률"), 3, 7),
		ItemData(56, 56,ITEM_TYPE::ARTEFACT, ITEM_RARITY::RARE,TEXT("힘의 부적"), TEXT("공격력"), 3, 8),
		ItemData(57, 57,ITEM_TYPE::ARTEFACT, ITEM_RARITY::RARE,TEXT("은 접시"), TEXT("공격력, 치명타 데미지"), 2, 9),
		ItemData(58, 58,ITEM_TYPE::ARTEFACT, ITEM_RARITY::RARE,TEXT("경고 문서"), TEXT("대시 회복 속도"), 1, 10),
		ItemData(59, 69,ITEM_TYPE::ARTEFACT, ITEM_RARITY::LEGENDARY,TEXT("눈보라 망치"), TEXT("7초 마다 대시 쉬 주변 적에게 망치를 날림"), 6, 11),
		ItemData(60, 60,ITEM_TYPE::ARTEFACT, ITEM_RARITY::RARE,TEXT("노란 행성"), TEXT("노란 행성 소환"), 6, 12),
		ItemData(61, 61,ITEM_TYPE::ARTEFACT, ITEM_RARITY::RARE,TEXT("붉은 행성"), TEXT("붉은 행성 소환"), 6, 13),
		ItemData(62, 62,ITEM_TYPE::ARTEFACT, ITEM_RARITY::RARE,TEXT("푸른 행성"), TEXT("푸른 행성 소환"), 6, 14),
		ItemData(63, 63,ITEM_TYPE::SKILLBOOK, ITEM_RARITY::RARE,TEXT("아이스 볼트"), TEXT("아이스 볼트 마법 획득"), 1, 15),
		ItemData(64, 64,ITEM_TYPE::SKILLBOOK, ITEM_RARITY::NORMAL,TEXT("라이트닝 볼트"), TEXT("라이트닝 볼트 마법 획득"), 1, 16),
	};
	
	const vector<Item_Effect> g_ItemEffect
	{
		Item_Effect(0, ITEM_EFFECT::VALUE_TYPE, {{STAT_INFO::CULDAMAGE, 2, 2}}),
		Item_Effect(1, ITEM_EFFECT::VALUE_TYPE, {{STAT_INFO::MAXDASH, 1, 1}}),
		Item_Effect(2, ITEM_EFFECT::VALUE_TYPE, {{STAT_INFO::MAXHP, 10, 15}}),
		Item_Effect(3, ITEM_EFFECT::VALUE_TYPE, {{STAT_INFO::CULDAMAGE, 5, 10}}),
		Item_Effect(4, ITEM_EFFECT::VALUE_TYPE, {{STAT_INFO::CULDEF, 5, 10}}),
		Item_Effect(5, ITEM_EFFECT::VALUE_TYPE, {{STAT_INFO::CULCRITICAL, 1, 3}, {STAT_INFO::CULDAMAGE, 1, 2}}),
		Item_Effect(6, ITEM_EFFECT::VALUE_TYPE, {{STAT_INFO::REGENDASH, 1.2f, 1.2f}}),
		Item_Effect(7, ITEM_EFFECT::VALUE_TYPE, {{STAT_INFO::CULCRITICAL, 3, 5}}),
		Item_Effect(8, ITEM_EFFECT::VALUE_TYPE, {{STAT_INFO::CULDAMAGE, 2, 2}}),
		Item_Effect(9, ITEM_EFFECT::VALUE_TYPE, {{STAT_INFO::CULDAMAGE, 2, 5}, {STAT_INFO::CRITICALDAMAGE, 2, 5}}),
		Item_Effect(10, ITEM_EFFECT::VALUE_TYPE, {{STAT_INFO::REGENDASH, 2.f, 2.f}}),
		Item_Effect(11, ITEM_EFFECT::SPAWN_TYPE, {{STAT_INFO::STAT_END, 0, 0}}, TEXT("Snow Hamer")),
		Item_Effect(12, ITEM_EFFECT::SPAWN_TYPE, {{STAT_INFO::STAT_END, 0, 0}}, TEXT("Yellow Planet")),
		Item_Effect(13, ITEM_EFFECT::SPAWN_TYPE, {{STAT_INFO::STAT_END, 0, 0}}, TEXT("Red Planet")),
		Item_Effect(14, ITEM_EFFECT::SPAWN_TYPE, {{STAT_INFO::STAT_END, 0, 0}}, TEXT("Bule Planet")),
		Item_Effect(15, ITEM_EFFECT::SKILLBOOK_TYPE, {{STAT_INFO::STAT_END, 0, 0}}, TEXT("Ice Bolt")),
		Item_Effect(16, ITEM_EFFECT::SKILLBOOK_TYPE, {{STAT_INFO::STAT_END, 0, 0}}, TEXT("Lightning Bolt")),
	};

	const vector<Slate> g_SlateDataBase
	{
		//	0	1	2	3	4	5			//			(-1, -2)				
		//	6	7	8	9	10	11			//			(-1, -1)	(0, -1)		(1, -1)		(2. -1)	
		//	12	13	14	15	16	17			//			(-1, 0)		(0, 0)		(1, 0)			
		//	18	19	20	21	22	23			//(-2, 1)	(-1, 1)		(0, 1)		(1, 1) 
		//	24	25	26	27	28	29			//									(1, 2)

		//인덱스, 상대 or 고정, 회전 가능 여부, (X, Y, 값)
		Slate(0, true, true,	{{0,-1,1,0},{0, 1,1,0}}),
		Slate(1, true, true,	{{0,1,1,0},{1,0,1,0} }),
		Slate(2, true, true,	{{-1, -2,1,0}}),
		Slate(3, true, true,	{{-2,0,-2,0},{-1, 0,-1,0}, {1, 0,1,0}, {2, 0,1,0}}),
		Slate(4, true, true,	{{-1,-1,1,0},{1,1,-1,0} }),
		Slate(5, true, true,	{{0, -1,1,0},{0, 1,1,0}}),
		Slate(6, true, true,	{{0,-2,2,0},{0, 2,2,0}}),
		Slate(7, true, false,	{{0,1,1,0} }),
		Slate(8, true, true,	{{0, -2,2,0},{0, 2,-2,0}}),
		Slate(9, true, true,	{{-1, 0,-1,0},{0, -1,-1,0},{1, 0,1,0}, {0, 1,1,0}} ),	
		Slate(10, true, false,	{{0,1,1,0} }),
		Slate(11, true, true,	{{1, 0,1,0}}),
		Slate(12, true, true,	{{-1,-1,-1,0},{0, -1,-1,0}, {0, 1,2,0}}),
		Slate(13, true, true,	{{0,-2,2,0} }),
		Slate(14, true, true,	{{-1, 0,1,0},{-1, -1,1,0},{0, -1,1,0}, {1, -1,1,0}}),
		Slate(15, true, false,	{{-1,0,1,0},{1, 0,1,0},{0,-1,1,0},{0, 1,1,0}}),
		Slate(16, true, false,	{{-1,0,1,0},{1, 0,1,0},{0,-1,1,0} }),
		Slate(17, true, true,	{{0, -1,2,0},{-0, 1, 2,0}}),
		Slate(18, true, true,	{{0, -2,2,0},{-0, 2, 2,0}}),
		Slate(19, true, true,	{{-1,-1,1,0},{0,-1,1,0},{1,-1,1,0} }),
		Slate(20, true, false,	{{0, -1,2,0}}),
		Slate(21, true, true,	{{0,-1,3,0},{-3, 1,-1,0}, {-2, 1,-1,0}, {-1, 1,-1,0}, {0,1,-1,0}, { 3, 1,-1,0 }, {2, 1,-1,0}, {1, 0,-1,0}}),
		Slate(22, true, false,	{{-1,-1,1,0},{0,-1,2,0},{1,-1,1,0} }),
		Slate(23, true, true,	{{-1, -1,1,0},{0, -1,1,0},{1, -1,1,0}, {-1, 0,-1,0},{1, 0,-1,0}}),
		Slate(24, true, true,	{{-1,-2,1,0}, {-1, -1,1,0}, {0, -2,1,0}, {0, -1,1,0}}),	//
		Slate(25, true, true,	{{-4,0,1,0},{-3,0,1,0},{-2,0,1,0},{-1,0,1,0}, {4,0,1,0},{3,0,1,0},{2,0,1,0},{1,0,1,0}, {5,0,1,0},{-5,0,1,0},{0,-4,-1,0},{0,-3,-1,0},{0,-2,-1,0},{0,-1,-1,0},{0,4,-1,0},{0,3,-1,0},{0,2,-1,0},{0,1,-1,0} }),
		Slate(26, true, true,	{{0, -3,1,0},{0, -2,-1,0},{0, -1,-1,0}}),
		Slate(27, false, false, {{0,0,1,5},{0,0,1,11},{0,0,1,17},{0,0,1,23},{0,0,1,29}}),
		Slate(28, true, true, {{1,1,1,0}, {-1,-1,1,0} }),
		Slate(29, true, true, {{0,-1,-1,0},{1, 0,-1,0},{1, -1,2,0}}),
		Slate(30, true, true, {{-1,-1,1,0},{1, -1,1,0}, {1, 1,1,0}, {-1, 1,1,0}}),
		Slate(31, true, true, {{0,-1,3,0}}),
		Slate(32, true, false, {{-4,0,1,0},{-3,0,1,0},{-2,0,1,0},{-1,0,1,0}, {4,0,1,0},{3,0,1,0},{2,0,1,0},{1,0,1,0}, {5,0,1,0},{-5,0,1,0}}),
		Slate(33, true, false, {{0,-3,1,0},{0, -2,1,0},{0, -1,1,0}}),
		Slate(34, true, true, {{0,-2,2,0},{0,-1,1,0},{0,1,1,0},{-1,0,1,0},{1,0,1,0} }),
		Slate(35, true, true, {{2, -2,1,0},{1, -1,1,0},{-1,1,1,0}}),
		Slate(36, false, false, {{0,0,1,24},{0,0,1,25},{0,0,1,26}, {0,0,1,27},{0,0,1,28},{0,0,1,29} }),//
		Slate(37, true, false, {{-4,0,1,0},{-3,0,1,0},{-2,0,1,0},{-1,0,1,0}, {4,0,1,0},{3,0,1,0},{2,0,1,0},{1,0,1,0}, {5,0,1,0},{-5,0,1,0}, {-4,-3,1,0},{-3,-3,1,0},{-2,-3,1,0},{-1,-3,1,0}, {4,-3,1,0},{3,-3,1,0},{2,-3,1,0},{1,-3,1,0}, {5,-3,1,0},{-5,-3,1,0},{0,-3,1,0}}),
		Slate(38, true, false, {{-1, -1,1,0},{0, -1,1,0},{1, -1,1,0}, {1, 0,1,0}, {1, 1,1,0}, {0, 1,1,0}, {-1, 1,1,0}, {-1, -1,1,0}, {-1, 0,1,0}}),
		Slate(39, true, true, {{-4,0,1,0},{-3,0,1,0},{-2,0,1,0},{-1,0,1,0}, {4,0,1,0},{3,0,1,0},{2,0,1,0},{1,0,1,0}, {5,0,1,0},{-5,0,1,0},{0,-1,2,0},{0,1,2,0} }),
		Slate(40, true, true, {{-4,0,1,0},{-3,0,1,0},{-2,0,1,0},{-1,0,1,0}, {4,0,1,0},{3,0,1,0},{2,0,1,0},{1,0,1,0}, {5,0,1,0},{-5,0,1,0},{0,-1,1,0}, { 0,-2,1,0 },{0,-3,1,0} }),
		Slate(41, true, true, {{-1, -1,1,0},{-1, -2,1,0},{1, -1,1,0}, {1, -2,1,0},{-1, -1,1,0},{-1, -2,1,0},{1, -1,1,0},{1, -2,1,0}}),
		Slate(42, true, true, {{0,-3,1,0},{0, -2,2,0},{0, -1,3,0}}),
		Slate(43, true, true, {{-1,-1,1,0},{0,-1,1,0},{1,-1,1,0},{1,0,1,0} }),
		Slate(44, true, false, {{0, -1,3,0},{0, 1,3,0},{1, 0,-1,0},{-1, 0,1,0}}),
	};

	const vector<Potion> g_PotionDataBase
	{
		Potion(0, 1, 20, 5),	//HP 20퍼 회복 최대 5개
		Potion(1, 1, 50, 5),	//HP 50퍼 회복 최대 5개
		Potion(2, 2, 30, 5)		//MP 30퍼 회복 최대 3개
	};
}
