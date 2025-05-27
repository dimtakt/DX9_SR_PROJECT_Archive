#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
namespace Client
{
	enum class ITEM_TYPE { ARTEFACT, STONE, POTION, ITEM_TYPE_END};

	enum class ITEM_RARITY { NORMAL, RARE, EPIC, LEGENDARY, ITEM_RARITY_END};

	struct ItemData
	{
	public:
		_int			m_iItemID;
		_wstring		m_szName;
		_wstring		m_szDescription;
		ITEM_TYPE		m_eType;
		ITEM_RARITY		m_eRarity;
		_int			m_iiValue;		//효과 관련 수치 값 기능 별로 다르게 적용 예정
										//(아티팩트 맥스 레벨, 포션이나 석판은 효과 값으로 사용 예정)

		ItemData(_int iItemID, ITEM_TYPE eType, ITEM_RARITY eRarity, _wstring szName, _wstring szDescription, _int iValue) : m_iItemID(iItemID), m_szName(szName), m_szDescription(szDescription), m_eType(eType), m_eRarity(eRarity), m_iiValue(iValue){};
	};

	const vector<ItemData> g_ItemDataBase = {
		//인덱스 규칙: 아이템 타입, 아이템 효과 인덱스, 이미지 텍스쳐 번호
		//예 : 10302 = (아티팩트 타입)이며 (3번) 효과를 부여한 아이템 중 (2번 째 텍스쳐 이미지)
		ItemDate(10101, ITEM_TYPE::ARTEFACT, ITEM_RARITY::NORMAL, "말라버린 꽃", "어디서든 씩씩하게 자라는 꽃을 정갈하게 말린 것", 3),
		ItemDate(10202, ITEM_TYPE::ARTEFACT, ITEM_RARITY::NORMAL,"부정한 붕대", "춤추는 그을은 연기처럼.", 3),
		ItemDate(10303, ITEM_TYPE::ARTEFACT, ITEM_RARITY::LEGENDARY,"레전더리", "공격력 올려주는 아이템.", 5),
		ItemDate(20101, ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,"건조", "", 0),
		ItemDate(20202, ITEM_TYPE::STONE, ITEM_RARITY::NORMAL,"근사", "", 0),
		ItemDate(20303, ITEM_TYPE::STONE, ITEM_RARITY::EPIC,"비석", "", 0),
		ItemDate(30101, ITEM_TYPE::POTION, ITEM_RARITY::NORMAL,"재생의 포션", "HP 20% 회복", 20),
		ItemDate(30102, ITEM_TYPE::POTION, ITEM_RARITY::RARE,"큰 재샙의 포션", "HP 50% 회복", 50),
		ItemDate(30203, ITEM_TYPE::POTION, ITEM_RARITY::RARE,"마나 재생의 포션", "MP 20% 회복", 50),
	};
}