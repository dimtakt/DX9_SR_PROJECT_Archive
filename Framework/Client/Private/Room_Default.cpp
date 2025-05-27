#include "Room_Default.h"
#include "GameInstance.h"
#include "Land.h"

CRoom_Default::CRoom_Default(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CRoom { pGraphic_Device }
{
}

CRoom_Default::CRoom_Default(const CRoom_Default& Prototype)
	: CRoom { Prototype }
{
}

HRESULT CRoom_Default::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CRoom_Default::Initialize(void* pArg)
{

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRoom_Default::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CRoom_Default::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CRoom_Default::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRoom_Default::Render()
{
	__super::Render();
	return S_OK;
}

HRESULT CRoom_Default::Ready_Components(void* pArg)
{
	return S_OK;
}
HRESULT CRoom_Default::Ready_Objects(void* pArg)
{

	return S_OK;
}
CRoom_Default* CRoom_Default::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRoom_Default* pInstance = new CRoom_Default(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CRoom_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CRoom_Default::Clone(void* pArg)
{
	CRoom_Default* pInstance = new CRoom_Default(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CRoom_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CRoom_Default::Free()
{
	__super::Free();

}
