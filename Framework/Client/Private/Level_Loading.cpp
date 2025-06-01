#include "Level_Loading.h"

#include "Loader.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_MapEdit.h"
#include "Level_Stage1.h"
#include "Level_Stage2.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	if (FAILED(Ready_GameObjects()))
		return E_FAIL;

	if (FAILED(Ready_LoadingThread()))
		return E_FAIL;

	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Player"), TEXT("Com_Transform")));
	pPlayerTransform->Set_State(STATE::POSITION, _float3(12488.f, 12488.f, 12488.f));

	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	if (true == m_pLoader->isFinished())
	{
		m_pGameInstance->Clear_Colliders();

		CLevel* pNewLevel = { nullptr };
		
		switch (m_eNextLevelID)
		{
		case LEVEL::LEVEL_LOGO:
			pNewLevel = CLevel_Logo::Create(m_pGraphic_Device);
			break;
		case LEVEL::LEVEL_MAPEDIT:
			pNewLevel = CLevel_MapEdit::Create(m_pGraphic_Device);
			break;
		case LEVEL::LEVEL_STAGE1:
			pNewLevel = CLevel_Stage1::Create(m_pGraphic_Device);
			break;
		case LEVEL::LEVEL_STAGE2:
			pNewLevel = CLevel_Stage2::Create(m_pGraphic_Device);
			break;
		}

		if (FAILED(m_pGameInstance->Open_Level(ENUM_CLASS(m_eNextLevelID), pNewLevel)))
			return;
	}
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Show_LoadingText();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_GameObjects()
{
	return S_OK;
}

HRESULT CLevel_Loading::Ready_LoadingThread()
{
	m_pLoader = CLoader::Create(m_pGraphic_Device, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}