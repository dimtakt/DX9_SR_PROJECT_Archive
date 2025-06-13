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

    return S_OK;
}

void CIceBolt::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyDown('R'))
    {
        Create_Bolt();
    }
}

void CIceBolt::Update(_float fTimeDelta)
{
  /*  if (m_pGameInstance->IsKeyDown('R'))
    {
        Create_Bolt();
    }*/
}

void CIceBolt::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CIceBolt::Render()
{
 /*   SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIndex)))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

    Reset_RenderState();*/

    return S_OK;
}

void CIceBolt::Create_Bolt()
{
    //_float3 vMouseWorld = {};
    //_float fY = m_pPlayerTransformCom->Get_State(STATE::POSITION).y;                

    //m_pGameInstance->Get_IntersectAtY(fY, vMouseWorld);                

    //_float3 vPlayerPos = m_pPlayerTransformCom->Get_State(STATE::POSITION);
    //m_vDirection = vMouseWorld - vPlayerPos;              //현재 마우스 방향 체크

    //D3DXVec3Normalize(&m_vDirection, &m_vDirection);     // 마우스 방향 단일화

    //_float4x4 matWorld = *m_pPlayerTransformCom->Get_WorldMatrix(); //현재 플레이어 월드행렬

    //_float4x4 matOffset;
    //D3DXMatrixIdentity(&matOffset);                     //마법진 오프셋값  

    //_float3 vForwardOffset = m_vDirection * 0.5f;       //마우스가 있는 방향으로 + 오프셋 설정

    ////마우스 방향에 맞춰 마법진 오프셋 설정
    //matOffset._41 += vForwardOffset.x;
    //matOffset._42 += vForwardOffset.y;
    //matOffset._43 += vForwardOffset.z;

    ////마법진 제작
    //CEffect_Factory::GetInstance()->Create_Effect(
    //    GAMEOBJ_TYPE::PLAYER_SKILL,
    //    L"Prototype_Component_Texture_Effect_IceBoltStart",
    //    matWorld,            // 시작위치
    //    matOffset,           // 오프셋값
    //    false);

    ////m_vDirection.x;     // 왼쪽 쏠때 z가더 큼 , 오른쪽 쏠 때 x가 더 큼
    ////m_vDirection.y;     //위 쏠 때 z가 더 큼 , 아래 쏠 때 x가더 큼
    ////m_vDirection.z;

    //m_pTransformCom->Set_State(STATE::POSITION, vPlayerPos);

    //if (m_vDirection.x > m_vDirection.z)
    //{
    //    if (m_vDirection.x >= 0.8f) // 오른쪽  
    //    {

    //        m_pTransformCom->Rotation({ m_vDirection.x, 0.f, 0.f }, D3DXToRadian(90.f));
    //        m_pTransformCom->Rotation({ 0.f, 0.f,  m_vDirection.z }, D3DXToRadian(90.f));
    //    }
    //    else   //아래
    //        m_pTransformCom->Rotation({ m_vDirection.x, 0.f, 0.f }, D3DXToRadian(-90.f));
    //}
    //else
    //{
    //    if (m_vDirection.z >= 0.8f) // 위
    //    {
    //        m_pTransformCom->Rotation({ m_vDirection.x, 0.f, 0.f }, D3DXToRadian(90.f));
    //    }
    //    else  // 왼쪽
    //    {
    //        m_pTransformCom->Rotation({ m_vDirection.x, 0.f, 0.f }, D3DXToRadian(90.f));
    //        m_pTransformCom->Rotation({ 0.f, 0.f,  m_vDirection.z }, D3DXToRadian(-90.f));
    //    }
    //}
   
    ////투사체 제작
    //CEffect_Factory::GetInstance()->Create_Effect(
    //    GAMEOBJ_TYPE::PLAYER_SKILL,
    //    L"Prototype_Component_Texture_IceBolt",
    //    *m_pTransformCom->Get_WorldMatrix(),       // 시작 위치
    //    matOffset,                                // 발사 오프셋
    //    m_vDirection,                             // 투사체 방향
    //    8.f,                                     // 속도
    //    1.f,                                     // 수명
    //    0.f,                                    // 회전 없음
    //    false);

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


     ////투사체 이펙트 제작 테스트
     //CEffect_Factory::GetInstance()->Create_Effect(
     //GAMEOBJ_TYPE::PLAYER_SKILL,
     //    L"Prototype_Component_Texture_Effect_IceBoltEffect",
     //    *m_pTransformCom->Get_WorldMatrix(),      
     //    matOffset,
     //    m_vDirection,                             
     //    7.f,                                    
     //    1.1f,                                   
     //    0.f,                                    
     //    false);                                  //투사체이펙트는 이렇게 하면 안될거 같은데 이상한데



    // 투사체 끝날 때 터지는 이펙트 추가해줘야함 or 콜리전 발생시 이펙트 추가해줘야하는데..
    // 투사체가 죽은걸 어떻게 알아야하는지 ?
    // 투사체가 죽으면 죽은 위치에 이펙트 처리 해줘야하는데 위치는 어디인지? 저장 따로 해줘야할거 같은데..?

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
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pPlayerTransformCom);

    __super::Free();
}
