#include "PlayerStats.h"

CPlayerStats::CPlayerStats(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CPlayerStats::CPlayerStats(const CPlayerStats& Prototype)
	: CComponent{ Prototype }
{
}

HRESULT CPlayerStats::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerStats::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	PLAYERSTAT_DESC* pDesc = static_cast<PLAYERSTAT_DESC*>(pArg);

	tPlayerStats = *pDesc;

	return S_OK;
}

CPlayerStats* CPlayerStats::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayerStats* pInstance = new CPlayerStats(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayerStats"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CPlayerStats::Clone(void* pArg)
{
	CPlayerStats* pInstance = new CPlayerStats(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to cloned : CPlayerStats"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerStats::Free()
{
	__super::Free();
}
