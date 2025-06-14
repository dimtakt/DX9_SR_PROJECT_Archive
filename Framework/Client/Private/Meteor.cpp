#include "Meteor.h"
#include "GameInstance.h"
#include "Client_Struct.h"
#include "Effect_Factory.h"

CMeteor::CMeteor(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CMeteor::CMeteor(const CMeteor& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CMeteor::Initialize_Prototype()
{
  /*  m_pGameInstance->Item_MaxCool(L"Meteor", m_fMaxCoolTime);*/
    return S_OK;
}

HRESULT CMeteor::Initialize(void* pArg)
{
    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

void CMeteor::Priority_Update(_float fTimeDelta)
{
    if (m_fCurrentCoolTime > 0.f)
        m_fCurrentCoolTime--;
}

void CMeteor::Update(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyDown('R'))
    {
        Create_Bolt();
    }
  /*  m_pGameInstance->Item_CulCool(L"Meteor", m_fCurrentCoolTime);*/
}

void CMeteor::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CMeteor::Render()
{
    return S_OK;
}

void CMeteor::OnEvent(_uint iTypeindex, const EVENTDATA* pData)
{
    if (m_fCurrentCoolTime <= 0.f)
    {
        _float3 vMouseWorld = {};
        _float fY = m_pPlayerTransformCom->Get_State(STATE::POSITION).y;
        m_pGameInstance->Get_IntersectAtY(fY, vMouseWorld);

        _float3 vPlayerPos = m_pPlayerTransformCom->Get_State(STATE::POSITION);
        m_vDirection = vMouseWorld - vPlayerPos;
        D3DXVec3Normalize(&m_vDirection, &m_vDirection);

        _float3 vLook = m_vDirection;
        _float3 vUp = { 0.f, 1.f, 0.f };
        _float3 vRight;

        if (fabsf(D3DXVec3Dot(&vLook, &vUp)) >= 0.99f)
            vUp = { 0.f, 0.f, 1.f };

        D3DXVec3Cross(&vRight, &vUp, &vLook);
        D3DXVec3Normalize(&vRight, &vRight);
        D3DXVec3Cross(&vUp, &vLook, &vRight);
        D3DXVec3Normalize(&vUp, &vUp);

        m_pTransformCom->Set_State(STATE::RIGHT, vRight);
        m_pTransformCom->Set_State(STATE::UP, vUp);
        m_pTransformCom->Set_State(STATE::LOOK, vLook);
        m_pTransformCom->Set_State(STATE::POSITION, vPlayerPos);

        _float4x4 TempMat{};
        D3DXMatrixIdentity(&TempMat);

        _float4x4 matOffset;
        D3DXMatrixIdentity(&matOffset);
        _float3 vForwardOffset = m_vDirection * 0.5f;
        matOffset._41 = vForwardOffset.x;
        matOffset._42 = vForwardOffset.y;
        matOffset._43 = vForwardOffset.z;

        m_pTransformCom->Scaling(1.5f, 1.5f, 1.5f);


        CEffect_Factory::GetInstance()->Create_Effect(
            GAMEOBJ_TYPE::PLAYER_SKILL,
            L"Prototype_Component_Texture_Effect_MeteorStart",
            *m_pTransformCom->Get_WorldMatrix(),
            matOffset,
            false);

        // --- 랜덤 위치 잡아주기
        _float fRandom = 5.f;       // 중심 기준 최대 랜덤 오프셋 
        _float fRanX = m_pGameInstance->Compute_Random(-fRandom, fRandom);
        _float fRanZ = m_pGameInstance->Compute_Random(-fRandom, fRandom);

        _float3 vSpawnPos = {
            vPlayerPos.x + fRanX,
            vPlayerPos.y + 12.f,
            vPlayerPos.z + fRanZ
        };

        _float4x4 matWorld;
        D3DXMatrixIdentity(&matWorld);
        matWorld._41 = vSpawnPos.x;
        matWorld._42 = vSpawnPos.y;
        matWorld._43 = vSpawnPos.z;


        m_pTransformCom->Set_State(STATE::RIGHT, _float3(matWorld.m[0][0], matWorld.m[0][1], matWorld.m[0][2]));
        m_pTransformCom->Set_State(STATE::UP, _float3(matWorld.m[1][0], matWorld.m[1][1], matWorld.m[1][2]));
        m_pTransformCom->Set_State(STATE::LOOK, _float3(matWorld.m[2][0], matWorld.m[2][1], matWorld.m[2][2]));
        m_pTransformCom->Set_State(STATE::POSITION, _float3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]));


        _float4x4 NoOffset;
        D3DXMatrixIdentity(&NoOffset);

        CEffect_Factory::GetInstance()->Create_Effect(
            GAMEOBJ_TYPE::PLAYER_SKILL,
            L"Prototype_Component_Texture_Meteor",
            *m_pTransformCom->Get_WorldMatrix(),
            NoOffset,
            _float3{ 0.f , -1.f, 0.f },
            8.f,
            1.45f,
            0.f,
            false);

        m_fCurrentCoolTime = m_fMaxCoolTime;
    }
}

void CMeteor::Create_Bolt()
{
    if (m_fCurrentCoolTime <= 0.f)
    {
        _float3 vMouseWorld = {};
        _float fY = m_pPlayerTransformCom->Get_State(STATE::POSITION).y;
        m_pGameInstance->Get_IntersectAtY(fY, vMouseWorld);

        _float3 vPlayerPos = m_pPlayerTransformCom->Get_State(STATE::POSITION);
        m_vDirection = vMouseWorld - vPlayerPos;
        D3DXVec3Normalize(&m_vDirection, &m_vDirection);

        _float3 vLook = m_vDirection;
        _float3 vUp = { 0.f, 1.f, 0.f };
        _float3 vRight;

        if (fabsf(D3DXVec3Dot(&vLook, &vUp)) >= 0.99f)
            vUp = { 0.f, 0.f, 1.f };

        D3DXVec3Cross(&vRight, &vUp, &vLook);
        D3DXVec3Normalize(&vRight, &vRight);
        D3DXVec3Cross(&vUp, &vLook, &vRight);
        D3DXVec3Normalize(&vUp, &vUp);

        m_pTransformCom->Set_State(STATE::RIGHT, vRight);
        m_pTransformCom->Set_State(STATE::UP, vUp);
        m_pTransformCom->Set_State(STATE::LOOK, vLook);
        m_pTransformCom->Set_State(STATE::POSITION, vPlayerPos);

        _float4x4 TempMat{};
        D3DXMatrixIdentity(&TempMat);
       
        _float4x4 matOffset;
        D3DXMatrixIdentity(&matOffset);
        _float3 vForwardOffset = m_vDirection * 0.5f;
        matOffset._41 = vForwardOffset.x;
        matOffset._42 = vForwardOffset.y;
        matOffset._43 = vForwardOffset.z;

        m_pTransformCom->Scaling(1.5f, 1.5f, 1.5f);

        
        CEffect_Factory::GetInstance()->Create_Effect(
            GAMEOBJ_TYPE::PLAYER_SKILL,
            L"Prototype_Component_Texture_Effect_MeteorStart",
            *m_pTransformCom->Get_WorldMatrix(),
            matOffset,
            false);

        // --- 랜덤 위치 잡아주기
        _float fRandom = 5.f;       // 중심 기준 최대 랜덤 오프셋 
        _float fRanX = m_pGameInstance->Compute_Random(-fRandom, fRandom);
        _float fRanZ = m_pGameInstance->Compute_Random(-fRandom, fRandom);

        _float3 vSpawnPos = {
            vPlayerPos.x + fRanX,
            vPlayerPos.y + 12.f,
            vPlayerPos.z + fRanZ
        };

        _float4x4 matWorld;
        D3DXMatrixIdentity(&matWorld);
        matWorld._41 = vSpawnPos.x;
        matWorld._42 = vSpawnPos.y;
        matWorld._43 = vSpawnPos.z;


        m_pTransformCom->Set_State(STATE::RIGHT, _float3(matWorld.m[0][0], matWorld.m[0][1], matWorld.m[0][2]));
        m_pTransformCom->Set_State(STATE::UP, _float3(matWorld.m[1][0], matWorld.m[1][1], matWorld.m[1][2]));
        m_pTransformCom->Set_State(STATE::LOOK, _float3(matWorld.m[2][0], matWorld.m[2][1], matWorld.m[2][2]));
        m_pTransformCom->Set_State(STATE::POSITION, _float3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]));


        _float4x4 NoOffset;
        D3DXMatrixIdentity(&NoOffset);

        CEffect_Factory::GetInstance()->Create_Effect(
            GAMEOBJ_TYPE::PLAYER_SKILL,
            L"Prototype_Component_Texture_Meteor",
            *m_pTransformCom->Get_WorldMatrix(),
            NoOffset,
            _float3{ 0.f , -1.f, 0.f },
            8.f,
            1.45f,
            0.f,
            false);

        m_fCurrentCoolTime = m_fMaxCoolTime;
    }
}


HRESULT CMeteor::Ready_Components(void* pArg)
{

    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    METEORESC* MeteorDesc = static_cast<METEORESC*>(pArg);

    m_pPlayerTransformCom = MeteorDesc->pPlayerTransform;

    return S_OK;
}

CMeteor* CMeteor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMeteor* pInstance = new CMeteor(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CMeteor"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMeteor::Clone(void* pArg)
{
    CMeteor* pInstance = new CMeteor(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CMeteor"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMeteor::Free()
{
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    //Safe_Release(m_pPlayerTransformCom);
    m_pPlayerTransformCom = nullptr;

    __super::Free();
}
