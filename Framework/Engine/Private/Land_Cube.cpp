#include "Land_Cube.h"
#include "LandObject.h"

CLand_Cube::CLand_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject { pGraphic_Device }
{
}

CLand_Cube::CLand_Cube(const CLand_Cube& Prototype)
	: CLandObject { Prototype }
{
}

HRESULT CLand_Cube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLand_Cube::Initialize(void* pArg)
{
	return S_OK;
}

void CLand_Cube::Priority_Update(_float fTimeDelta)
{
}

void CLand_Cube::Update(_float fTimeDelta)
{
}

void CLand_Cube::Late_Update(_float fTimeDelta)
{
}

HRESULT CLand_Cube::Render()
{
	return S_OK;
}

CGameObject* CLand_Cube::Clone(void* pArg)
{
	return nullptr;
}

void CLand_Cube::Free()
{
	__super::Free();
}
