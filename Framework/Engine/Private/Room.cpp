#include "Room.h"

CRoom::CRoom(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{
}

CRoom::CRoom(const CRoom& Prototype)
    : CGameObject { Prototype }
    , m_bIsActive { Prototype.m_bIsActive }
    , m_bIsVisited { Prototype.m_bIsVisited }
{
}

HRESULT CRoom::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CRoom::Initialize(void* pArg)
{
    return S_OK;
}

void CRoom::Priority_Update(_float fTimeDelta)
{
}

void CRoom::Update(_float fTimeDelta)
{
}

void CRoom::Late_Update(_float fTimeDelta)
{
}

HRESULT CRoom::Render()
{
    return S_OK;
}

void CRoom::Enter()
{
    m_bIsActive = true;
    m_bIsVisited = true;
}

void CRoom::Exit()
{
    m_bIsActive = false;
}

void CRoom::Activate()
{
    m_bIsActive = true;
}

void CRoom::Deactivate()
{
    m_bIsActive = false;
}

CRoom* CRoom::Clone(void* pArg)
{
    return nullptr;
}

void CRoom::Free()
{
    __super::Free();
}
