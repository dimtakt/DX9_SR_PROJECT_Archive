#include "Particle.h"
#include "GameInstance.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{

}

CParticle::CParticle(const CParticle& Prototype)
	: CGameObject{ Prototype }
{

}


HRESULT CParticle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle::Initialize(void* pArg)
{
	return S_OK;
}

void CParticle::Priority_Update(_float fTimeDelta)
{

}

void CParticle::Update(_float fTimeDelta)
{
}

void CParticle::Late_Update(_float fTimeDelta)
{
}

HRESULT CParticle::Render()
{
	return S_OK;
}


void CParticle::Free()
{
	__super::Free();
}