#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
namespace Client
{
	enum class ITEM_TYPE { ARTEFACT, STONE, POTION, SKILLBOOK, ITEM_TYPE_END };

	enum class ITEM_RARITY { NORMAL, RARE, EPIC, LEGENDARY, ITEM_RARITY_END };

	enum class ITEM_INFO { ITEM_TYPE, ITEM_VALUE, ITEM_INFO_END };

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
		//(아티팩트 맥스 레벨, 포션 효과 값, 석판은 효과 인덱스 값)

	public:
		ItemData(_int iItemID, _int iItemTextureID, ITEM_TYPE eType, ITEM_RARITY eRarity,
			_wstring szName, _wstring szDescription, _int iItemValue) :
			m_iItemID(iItemID), m_iItemTextureID(iItemTextureID), m_eType(eType), m_eRarity(eRarity),
			m_szName(szName), m_szDescription(szDescription), m_iItemValue(iItemValue) {
		};
	};

	struct PosValue										//넣어줄 값들을 모두 소유
	{
		_int m_iX;
		_int m_iY;
		_int m_iValue;
		_int m_iPos;
	};
	struct Slate
	{
		_int				m_iSlateIndex;				//석판 번호
		_bool				m_bRelative;				//false 고정 좌표, true 유동 좌표
		_bool				m_bRotation;				//false 회전 불가, true 회전 가능

		vector<PosValue>	m_vecGardeValue;

	public:
		Slate(_int iSlateIndex, _bool bRelative, _bool bRotation, vector<PosValue> vecGardeValue) :
			m_iSlateIndex(iSlateIndex), m_bRelative(bRelative), m_bRotation(bRotation), m_vecGardeValue(vecGardeValue) {
		};
	};

	const vector<ItemData> g_ItemDataBase
	{
		ItemData(0, 0, ITEM_TYPE::ARTEFACT, ITEM_RARITY::RARE, TEXT("말라버린 꽃"), TEXT("어디서든 씩씩하게 자라는 꽃을 정갈하게 말린 것"), 3),
		ItemData(1, 1, ITEM_TYPE::SKILLBOOK, ITEM_RARITY::EPIC,TEXT("부정한 붕대"), TEXT("춤추는 그을은 연기처럼."), 3),
		ItemData(2, 2, ITEM_TYPE::ARTEFACT, ITEM_RARITY::LEGENDARY,TEXT("레전더리"), TEXT("공격력 올려주는 아이템."), 5),
		ItemData(3, 3, ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("건조"), TEXT(" "), 0),
		ItemData(4, 4,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("근사"), TEXT(""), 1),
		ItemData(5, 5,ITEM_TYPE::STONE, ITEM_RARITY::EPIC,TEXT("비석"), TEXT(""), 2),
		ItemData(6, 6,ITEM_TYPE::POTION, ITEM_RARITY::NORMAL,TEXT("재생의 포션"), TEXT("HP 20% 회복"), 20),
		ItemData(7, 7,ITEM_TYPE::POTION, ITEM_RARITY::RARE,TEXT("큰 재샙의 포션"), TEXT("HP 50% 회복"), 50),
		ItemData(8, 8,ITEM_TYPE::POTION, ITEM_RARITY::RARE,TEXT("마나 재생의 포션"), TEXT("MP 20% 회복"), 50),
		ItemData(9, 9,ITEM_TYPE::SKILLBOOK, ITEM_RARITY::NORMAL,TEXT("스킬북1"), TEXT("스킬북1"), 5),
		ItemData(10, 10,ITEM_TYPE::SKILLBOOK, ITEM_RARITY::RARE,TEXT("스킬북2"), TEXT("스킬북2"), 5)

	};

	const vector<Slate> g_SlateDataBase
	{
		//	0	1	2	3	4	5			//			(-1, -2)				
		//	6	7	8	9	10	11			//			(-1, -1)	(0, -1)		(1, -1)		(2. -1)	
		//	12	13	14	15	16	17			//			(-1, 0)		(0, 0)		(1, 0)			
		//	18	19	20	21	22	23			//(-2, 1)	(-1, 1)		(0, 1)		(1, 1) 
		//	24	25	26	27	28	29			//									(1, 2)

		//인덱스, 상대 or 고정, 회전 가능 여부, (X, Y, 값)
		Slate(0, true, true, {{-1,-1,1,0},{1, -1,1,0}, {1,1,1,0}, {-1, 1, 1,0}}),
		Slate(1, false, false, {{0,0,1,24},{0,0,1,25},{0,0,1,26}, {0,0,1,27},{0,0,1,28},{0,0,1,29} }),
		Slate(2, true, true, {{-1,-1,1,0},{-2, -2,1,0}})
	};
}