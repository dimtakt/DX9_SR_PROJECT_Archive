#include "IceBolt.h"
#include "GameInstance.h"
#include "Client_Struct.h"
#include "Effect_Factory.h"

CIceBolt::CIceBolt(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CIceBolt::CIceBolt(const CIceBolt& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CIceBolt::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CIceBolt::Initialize(void* pArg)
{
    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    m_pGameInstance->Item_MaxCool(L"Ice Bolt", m_fMaxCoolTime);
    m_pGameInstance->Subscribe(ENUM_CLASS(EVENT_TYPE::ICEBOLT), this);

    return S_OK;
}

void CIceBolt::Priority_Update(_float fTimeDelta)
{
    if (m_fCurrentCoolTime > 0.f)
        m_fCurrentCoolTime -= 1;
}

void CIceBolt::Update(_float fTimeDelta)
{
  /*  if (m_pGameInstance->IsKeyDown('R'))
    {
        Create_Bolt();
    }*/
    m_pGameInstance->Item_CulCool(L"Ice Bolt", m_fCurrentCoolTime);
}

void CIceBolt::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CIceBolt::Render()
{
    return S_OK;
}

void CIceBolt::OnEvent(_uint iTypeindex, const EVENTDATA* pData)
{
    if (static_cast<EVENT_TYPE>(iTypeindex) == EVENT_TYPE::ICEBOLT)
    {
        if (m_fCurrentCoolTime <= 0.f)
        {
            _float3 vMouseWorld = {};
            _float fY = m_pPlayerTransformCom->Get_State(STATE::POSITION).y;
            m_pGameInstance->Get_IntersectAtY(fY, vMouseWorld);

            _float3 vPlayerPos = m_pPlayerTransformCom->Get_State(STATE::POSITION);
            m_vDirection = vMouseWorld - vPlayerPos;
            D3DXVec3Normalize(&m_vDirection, &m_vDirection);

            // --- 방향 벡터들 계산 (Look 방향 기준) ---
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
            // ---- 오프셋 계산
            _float4x4 matOffset;
            D3DXMatrixIdentity(&matOffset);
            _float3 vForwardOffset = m_vDirection * 0.5f;
            matOffset._41 = vForwardOffset.x;
            matOffset._42 = vForwardOffset.y;
            matOffset._43 = vForwardOffset.z;

            m_pTransformCom->Scaling(2.f, 2.f, 2.f);

            // ---- 시작 이펙트는 회전 없이 사용 ----
            CEffect_Factory::GetInstance()->Create_Effect(
                GAMEOBJ_TYPE::PLAYER_SKILL,
                L"Prototype_Component_Texture_Effect_IceBoltStart",
                *m_pTransformCom->Get_WorldMatrix(),
                matOffset,
                false);

            // ---- 아이스볼트용 회전 보정 (+90도 X축 회전) ----
            _float4x4 matRotateX;
            D3DXMatrixRotationX(&matRotateX, D3DXToRadian(+90.f));

            // ---- 월드 행렬 만들고 누운 상태로 설정 ----
            _float4x4 matWorld = matRotateX * *m_pTransformCom->Get_WorldMatrix();

            m_pTransformCom->Set_State(STATE::RIGHT, _float3(matWorld.m[0][0], matWorld.m[0][1], matWorld.m[0][2]));
            m_pTransformCom->Set_State(STATE::UP, _float3(matWorld.m[1][0], matWorld.m[1][1], matWorld.m[1][2]));
            m_pTransformCom->Set_State(STATE::LOOK, _float3(matWorld.m[2][0], matWorld.m[2][1], matWorld.m[2][2]));
            m_pTransformCom->Set_State(STATE::POSITION, _float3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]));

            // ---- 아이스볼트 발사
            CEffect_Factory::GetInstance()->Create_Effect(
                GAMEOBJ_TYPE::PLAYER_SKILL,
                L"Prototype_Component_Texture_IceBolt",
                *m_pTransformCom->Get_WorldMatrix(),
                matOffset,
                m_vDirection,
                8.f,
                1.f,
                0.f,
                false);

            m_fCurrentCoolTime = m_fMaxCoolTime;
        }
    }
}

void CIceBolt::Create_Bolt()
{
    if (m_fCurrentCoolTime <= 0.f)
    {
        _float3 vMouseWorld = {};
        _float fY = m_pPlayerTransformCom->Get_State(STATE::POSITION).y;
        m_pGameInstance->Get_IntersectAtY(fY, vMouseWorld);

        _float3 vPlayerPos = m_pPlayerTransformCom->Get_State(STATE::POSITION);
        m_vDirection = vMouseWorld - vPlayerPos;
        D3DXVec3Normalize(&m_vDirection, &m_vDirection);

        // --- 방향 벡터들 계산 (Look 방향 기준) ---
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
        // ---- 오프셋 계산
        _float4x4 matOffset;
        D3DXMatrixIdentity(&matOffset);
        _float3 vForwardOffset = m_vDirection * 0.5f;
        matOffset._41 = vForwardOffset.x;
        matOffset._42 = vForwardOffset.y;
        matOffset._43 = vForwardOffset.z;

        m_pTransformCom->Scaling(2.f, 2.f, 2.f);

        // ---- 시작 이펙트는 회전 없이 사용 ----
        CEffect_Factory::GetInstance()->Create_Effect(
            GAMEOBJ_TYPE::PLAYER_SKILL,
            L"Prototype_Component_Texture_Effect_IceBoltStart",
            *m_pTransformCom->Get_WorldMatrix(),
            matOffset,
            false);

        // ---- 아이스볼트용 회전 보정 (+90도 X축 회전) ----
        _float4x4 matRotateX;
        D3DXMatrixRotationX(&matRotateX, D3DXToRadian(+90.f));

        // ---- 월드 행렬 만들고 누운 상태로 설정 ----
        _float4x4 matWorld = matRotateX * *m_pTransformCom->Get_WorldMatrix();

        m_pTransformCom->Set_State(STATE::RIGHT, _float3(matWorld.m[0][0], matWorld.m[0][1], matWorld.m[0][2]));
        m_pTransformCom->Set_State(STATE::UP, _float3(matWorld.m[1][0], matWorld.m[1][1], matWorld.m[1][2]));
        m_pTransformCom->Set_State(STATE::LOOK, _float3(matWorld.m[2][0], matWorld.m[2][1], matWorld.m[2][2]));
        m_pTransformCom->Set_State(STATE::POSITION, _float3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]));

        // ---- 아이스볼트 발사
        CEffect_Factory::GetInstance()->Create_Effect(
            GAMEOBJ_TYPE::PLAYER_SKILL,
            L"Prototype_Component_Texture_IceBolt",
            *m_pTransformCom->Get_WorldMatrix(),
            matOffset,
            m_vDirection,
            8.f,
            1.f,
            0.f,
            false);

        m_fCurrentCoolTime = m_fMaxCoolTime;
    }
}


HRESULT CIceBolt::Ready_Components(void* pArg)
{

    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    BOLTDESC* BoltDesc = static_cast<BOLTDESC*>(pArg);

    m_pPlayerTransformCom = BoltDesc->pPlayerTransform;

    return S_OK;
}

CIceBolt* CIceBolt::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CIceBolt* pInstance = new CIceBolt(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CIceBolt"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CIceBolt::Clone(void* pArg)
{
    CIceBolt* pInstance = new CIceBolt(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CIceBolt"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CIceBolt::Free()
{
    m_pGameInstance->Unsubscribe(ENUM_CLASS(EVENT_TYPE::ICEBOLT), this);

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pPlayerTransformCom);

    __super::Free();
}
