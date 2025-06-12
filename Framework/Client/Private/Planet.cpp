#include "Planet.h"

#include "GameInstance.h"
#include "Client_Struct.h"
#include "Stat_Manager.h"
#include "Effect_Factory.h"
#include "Room_Manager.h"

CPlanet::CPlanet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CPlanet::CPlanet(const CPlanet& Prototype)
	: CGameObject { Prototype }
{
}

HRESULT CPlanet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlanet::Initialize(void* pArg)
{
    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    m_eObjType = GAMEOBJ_TYPE::PLANET;

    PLANETDESC* pDesc = static_cast<PLANETDESC*>(pArg);
    m_pTargetTransform = pDesc->pTargetTransform;
    Safe_AddRef(m_pTargetTransform);
    m_pTransformCom->Set_State(STATE::POSITION, m_pTargetTransform->Get_State(STATE::POSITION) - _float3(1.5f, 0.f, 0.f));
    m_pTransformCom->Scaling(1.f, 1.f, 1.f);
 
    m_pTransformCom->Set_OrbitInfo_AroundY(m_pTargetTransform, D3DXToRadian(45.f));
	return S_OK;
}

void CPlanet::Priority_Update(_float fTimeDelta)
{

    m_fAttackFrame++;
}

void CPlanet::Update(_float fTimeDelta)
{
    m_pTransformCom->Orbit(fTimeDelta, m_pTargetTransform->Get_State(STATE::POSITION));


    if (m_fAttackFrame > 120.f)
    {
        vector<CMonster*> vMonster = CRoom_Manager::GetInstance()->Get_CurrentRoom()->Get_MonsterList();

        if (vMonster.size() > 0)
        {
            CMonster* pNearMonster = nullptr;
            _float fMinDistSq = FLT_MAX;

            for (auto& pMonster : vMonster)
            {
                _float3 vMonsterPos = pMonster->Get_Transform()->Get_State(STATE::POSITION);

                _float3 vDelta = vMonsterPos - m_pTransformCom->Get_State(STATE::POSITION);
                _float fDistSq = D3DXVec3LengthSq(&vDelta);

                if (fDistSq < fMinDistSq)
                {
                    fMinDistSq = fDistSq;
                    pNearMonster = pMonster;
                }

            }

            if (pNearMonster != nullptr)
            {
                _float4x4 tempMat{};
                D3DXMatrixIdentity(&tempMat);
                _float3 vThrownDir = pNearMonster->Get_Transform()->Get_State(STATE::POSITION) - m_pTransformCom->Get_State(STATE::POSITION);
                vThrownDir.y = 0.f;
                _float fThrownPower = 5.f;
                _float fThrownAtkLifeTime = 3.f;
                _float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();
                CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::PLAYER_EFFECT, L"Prototype_Component_Texture_Planet_Bullet_Cycle",
                    *m_pTransformCom->Get_WorldMatrix(), tempMat, vThrownDir, fThrownPower, fThrownAtkLifeTime, 0.f, true);

            }


        }
        m_fAttackFrame = 0.f;
    }
   
    
}

void CPlanet::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPlanet::Render()
{
    SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();

    m_pVIBufferCom->Bind_Buffers();

    m_pAnimatorCom->Update_State();
    m_pVIBufferCom->Render();

    Reset_RenderState();


	return S_OK;
}

void CPlanet::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CPlanet::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CPlanet::Ready_Components(void* pArg)
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Planet"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Planet";
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorCom->Add_State(L"Planet", { m_pTextureCom, 4, true });

    return S_OK;
}

CPlanet* CPlanet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlanet* pInstance = new CPlanet(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CPlanet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlanet::Clone(void* pArg)
{
    CPlanet* pInstance = new CPlanet(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CPlanet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlanet::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pAnimatorCom);
    Safe_Release(m_pTargetTransform);

}