#include "Inventory.h"
#include "GameInstance.h"
#include "Item_Base.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CInventory::CInventory(const CInventory& Prototype) : CUIObject(Prototype), m_eLevel(Prototype.m_eLevel), m_bIsOpen(false)
{
}

HRESULT CInventory::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

	if (FAILED(Ready_ChildPrototype(eLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	m_fSizeX = 1033.f;
	m_fSizeY = 1215.f;
	m_fX = g_iWinSizeX * 0.5;
	m_fY = g_iWinSizeY * 0.5;
	m_fZ = UI_DEPTH::INVENTORY;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();

	if (FAILED(Ready_Children()))
		return E_FAIL;

	m_pGameInstance->Add_UIObject(ENUM_CLASS(m_eLevel), TEXT("UI_Inven"), this);

	CItemObject* pItem = m_pGameInstance->Get_ItemObject(0, true);
	m_vecInventory[0]->Add_Item(static_cast<CItem_Base*>(pItem));

	pItem = m_pGameInstance->Get_ItemObject(1,true);
	m_vecInventory[1]->Add_Item(static_cast<CItem_Base*>(pItem));

	pItem = m_pGameInstance->Get_ItemObject(2, true);
	m_vecInventory[2]->Add_Item(static_cast<CItem_Base*>(pItem));

	pItem = m_pGameInstance->Get_ItemObject(3, true);
	m_vecInventory[3]->Add_Item(static_cast<CItem_Base*>(pItem));

	pItem = m_pGameInstance->Get_ItemObject(4, true);
	m_vecInventory[4]->Add_Item(static_cast<CItem_Base*>(pItem));

	pItem = m_pGameInstance->Get_ItemObject(5, true);
	m_vecInventory[5]->Add_Item(static_cast<CItem_Base*>(pItem));

	pItem = m_pGameInstance->Get_ItemObject(6, true);
	m_vecInventory[6]->Add_Item(static_cast<CItem_Base*>(pItem));

	pItem = m_pGameInstance->Get_ItemObject(7, true);
	m_vecInventory[7]->Add_Item(static_cast<CItem_Base*>(pItem));

	pItem = m_pGameInstance->Get_ItemObject(8, true);
	m_vecInventory[8]->Add_Item(static_cast<CItem_Base*>(pItem));
	return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;
	
	if (!m_bIsOpen)
		return;

	__super::Priority_Update(fTimeDelta);

}

void CInventory::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bIsOpen)
		return;

	Set_Grade();
	__super::Update(fTimeDelta);

}

void CInventory::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOADING) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_LOGO) || m_pGameInstance->Get_CurrentLevel() == ENUM_CLASS(LEVEL::LEVEL_MAPEDIT))
		return;

	if (!m_bIsUpdate)
		return;

	if (!m_bIsOpen)
		return;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CInventory::Render()
{
	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}

void CInventory::UI_Switch()
{
	if (m_bIsOpen)
		Close_UI();
	else
		m_bIsOpen = true;


}

void CInventory::Add_Item_Inven(_uint ItemIndex)
{
	CItemObject* pItem = m_pGameInstance->Get_ItemObject(ItemIndex, true);

	for (size_t i = 0; i < m_vecInventory.size(); ++i)
	{
		if (m_vecInventory[i]->Pop_Item() == nullptr)
			m_vecInventory[i]->Add_Item(static_cast<CItem_Base*>(pItem));
	}
}

void CInventory::Open_UI(_float fX, _float fY)
{
	m_bIsOpen = true;

	m_fX = m_iWinSizeX * 0.5 + fX;
	m_fY = m_iWinSizeY * 0.5 + fY;
	__super::Update_Position();
}

void CInventory::Close_UI()
{
	m_bIsOpen = false;

	m_fX = m_iWinSizeX * 0.5;
	m_fY = m_iWinSizeY * 0.5;
	__super::Update_Position();
}

void CInventory::Push_Item_Slot(CItem_Base* pItem, _uint iCount)
{
	for (_int i = 0; i < m_vecInventory.size(); i++)
	{
		if (m_vecInventory[i]->Pop_Item() == nullptr)
		{
			m_vecInventory[i]->Push_Item(pItem);
			m_vecInventory[i]->Push_Item_Count(iCount);
			return;
		}
	}
}

void CInventory::Set_Grade()
{
	for (size_t i = 0; i < m_vecInventory.size(); ++i)
	{
		if (m_vecInventory[i]->Pop_Item() == nullptr)
			continue;
		if (m_vecInventory[i]->Slot_Info(ITEM_INFO::ITEM_TYPE) != ENUM_CLASS(ITEM_TYPE::STONE))
			continue;

		//석판 효과 번호
		_int iIndex = m_vecInventory[i]->Slot_Info(ITEM_INFO::ITEM_VALUE);		//석판 인덱스 가져오기

		//석판 효과 번호 돌림

		for (size_t j = 0; j < g_SlateDataBase[iIndex].m_vecGardeValue.size(); ++j)	//석판에 입력된 좌표만큼 값넣기
		{
			_int MyX = i % 6;
			_int MyY = i / 6;

			_int YouIndex = 0;
			_int iX = 0;
			_int iY = 0;
		
			CItem_Base* pItem = m_vecInventory[i]->Pop_Item();

			_int iAngle = pItem->Item_Info()->fAngle;
			_int iValue = g_SlateDataBase[iIndex].m_vecGardeValue[j].m_iValue;

			if (g_SlateDataBase[iIndex].m_bRelative)								//석판 좌표 타입 체크
			{
				switch (iAngle)
				{
				case 0:
					iX = g_SlateDataBase[iIndex].m_vecGardeValue[j].m_iX;
					iY = g_SlateDataBase[iIndex].m_vecGardeValue[j].m_iY;
					break;
				case 90:
					iX = -g_SlateDataBase[iIndex].m_vecGardeValue[j].m_iY;
					iY = (g_SlateDataBase[iIndex].m_vecGardeValue[j].m_iX);
					break;
				case 180:
					iX = -g_SlateDataBase[iIndex].m_vecGardeValue[j].m_iX;
					iY = -g_SlateDataBase[iIndex].m_vecGardeValue[j].m_iY;
					break;
				case 270:
					iX = g_SlateDataBase[iIndex].m_vecGardeValue[j].m_iY;
					iY = -g_SlateDataBase[iIndex].m_vecGardeValue[j].m_iX;
					break;
				}
				_int youX = MyX + iX;
				_int youY = MyY + iY;

				if (youX < 0 || youX >= 6 || youY < 0 || youY >= 5)
					continue;
				YouIndex = youX + youY * 6;
				if (YouIndex >= 30 || YouIndex < 0)
					continue;
				m_vecInventory[YouIndex]->Add_GradeCount(iValue);
			}
			else
			{
				_int iValue = g_SlateDataBase[iIndex].m_vecGardeValue[j].m_iValue;
				_int iPos = g_SlateDataBase[iIndex].m_vecGardeValue[j].m_iPos;
				m_vecInventory[iPos]->Add_GradeCount(iValue);
			}

		}
	}
}

HRESULT CInventory::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rect_Window_Inventory"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CInventory::Ready_ChildPrototype(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevel), TEXT("Prototype_GameObject_UI_Inven_Slot"),
		CInven_Slot::Create(m_pGraphic_Device, eLevel))))
		return E_FAIL;

	return S_OK;
}

HRESULT CInventory::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	CInven_Slot::INVEN_SLOT_DESC Desc{};

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			Desc.fX = j;
			Desc.fY = i;
			Desc.iSlotInedx = j + (i * 6);
			pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(m_eLevel), TEXT("Prototype_GameObject_UI_Inven_Slot"), &Desc));
			if (nullptr == pGameObject)
				return E_FAIL;
			Add_Child(pGameObject);
			m_vecInventory.push_back(dynamic_cast<CInven_Slot*>(pGameObject));
			Safe_AddRef(m_vecInventory[Desc.iSlotInedx]);
		}
	}

	return S_OK;
}

CInventory* CInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CInventory* pInstance = new CInventory(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CInventory"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CInventory"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInventory::Free()
{
	for (auto& pItemObject : m_vecInventory)
		Safe_Release(pItemObject);
	m_vecInventory.clear();

	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

