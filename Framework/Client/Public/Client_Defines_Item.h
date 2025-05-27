#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
namespace Client
{
	enum class ITEM_TYPE { ARTEFACT, STONE, POTION, SKILLBOOK, ITEM_TYPE_END};

	enum class ITEM_RARITY { NORMAL, RARE, EPIC, LEGENDARY, ITEM_RARITY_END};

	struct ItemData
	{
	public:
		_int			m_iItemID;
		_int			m_iItemTextureID;
		_wstring		m_szName;
		_wstring		m_szDescription;
		ITEM_TYPE		m_eType;
		ITEM_RARITY		m_eRarity;
		_int			m_iiValue;		//효과 관련 수치 값 기능 별로 다르게 적용 예정
										//(아티팩트 맥스 레벨, 포션이나 석판은 효과 값으로 사용 예정)

	public:
		ItemData(_int iItemID, _int iItemTextureID,ITEM_TYPE eType, ITEM_RARITY eRarity, _wstring szName, _wstring szDescription, _int iValue) : m_iItemID(iItemID), m_szName(szName), m_szDescription(szDescription), m_eType(eType), m_eRarity(eRarity), m_iiValue(iValue), m_iItemTextureID(iItemTextureID){};
	};

	const vector<ItemData> g_ItemDataBase
	{
		ItemData(0, 0, ITEM_TYPE::ARTEFACT, ITEM_RARITY::NORMAL, TEXT("말라버린 꽃"), TEXT("어디서든 씩씩하게 자라는 꽃을 정갈하게 말린 것"), 3),
		ItemData(1, 1, ITEM_TYPE::ARTEFACT, ITEM_RARITY::NORMAL,TEXT("부정한 붕대"), TEXT("춤추는 그을은 연기처럼."), 3),
		ItemData(2, 2, ITEM_TYPE::ARTEFACT, ITEM_RARITY::LEGENDARY,TEXT("레전더리"), TEXT("공격력 올려주는 아이템."), 5),
		ItemData(3, 3, ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("건조"), TEXT(" "), 0),
		ItemData(4, 4,ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,TEXT("근사"), TEXT(""), 0),
		ItemData(5, 5,ITEM_TYPE::STONE, ITEM_RARITY::EPIC,TEXT("비석"), TEXT(""), 0),
		ItemData(6, 6,ITEM_TYPE::POTION, ITEM_RARITY::NORMAL,TEXT("재생의 포션"), TEXT("HP 20% 회복"), 20),
		ItemData(7, 7,ITEM_TYPE::POTION, ITEM_RARITY::RARE,TEXT("큰 재샙의 포션"), TEXT("HP 50% 회복"), 50),
		ItemData(8, 8,ITEM_TYPE::POTION, ITEM_RARITY::RARE,TEXT("마나 재생의 포션"), TEXT("MP 20% 회복"), 50),
		ItemData(9, 9,ITEM_TYPE::SKILLBOOK, ITEM_RARITY::NORMAL,TEXT("스킬북1"), TEXT("HP 20% 회복"), 0),
		ItemData(10, 10,ITEM_TYPE::SKILLBOOK, ITEM_RARITY::RARE,TEXT("스킬북2"), TEXT("HP 50% 회복"), 0)

	};
}