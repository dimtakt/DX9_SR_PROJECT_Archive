#include "FrozenHammer.h"

#include "GameInstance.h"
#include "Client_Struct.h"
#include "Stat_Manager.h"
#include "Effect_Factory.h"
#include "Room_Manager.h"
#include "Client_Defines_Event.h"

CFrozenHammer::CFrozenHammer(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CFrozenHammer::CFrozenHammer(const CFrozenHammer& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CFrozenHammer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFrozenHammer::Initialize(void* pArg)
{
    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    m_eObjType = GAMEOBJ_TYPE::FROZENHAMMER;

    FROZENHAMMERDESC* pDesc = static_cast<FROZENHAMMERDESC*>(pArg);
    m_pTargetTransform = pDesc->pTargetTransform;
    Safe_AddRef(m_pTargetTransform);
    m_pTransformCom->Set_State(STATE::POSITION, m_pTargetTransform->Get_State(STATE::POSITION) - _float3(1.5f, 0.f, 0.f));
    m_pTransformCom->Scaling(2.f, 2.f, 2.f);

    m_pGameInstance->Subscribe(ENUM_CLASS(EVENT_TYPE::FROZENHAMMER), this);
    m_pGameInstance->Item_MaxCool(TEXT("Snow Hamer"), m_fAttackCool);
    m_fCulAttackCool = 0.f;
    return S_OK;
}

void CFrozenHammer::Priority_Update(_float fTimeDelta)
{
    m_pGameInstance->Item_CulCool(TEXT("Snow Hamer"), m_fCulAttackCool);

    if (m_fCulAttackCool > 0) {
        m_fCulAttackCool--;
    }
    
}

void CFrozenHammer::Update(_float fTimeDelta)
{
    _float3 vDelta = m_pTargetTransform->Get_State(STATE::POSITION) - m_pTransformCom->Get_State(STATE::POSITION);
    _float fDistSq = D3DXVec3LengthSq(&vDelta);
    if (sqrt(fDistSq) > 10)
    {
        m_pTransformCom->Set_State(STATE::POSITION, m_pTargetTransform->Get_State(STATE::POSITION));
    }
    _float3 vPos = m_pTargetTransform->Get_State(STATE::POSITION);
    vPos.y = 2.4f;
    m_pTransformCom->Move_To(vPos, fTimeDelta, 1.f);
}

void CFrozenHammer::Late_Update(_float fTimeDelta)
{
    if (m_fCulAttackCool <= 0)
        m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CFrozenHammer::Render()
{
    SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();

    m_pVIBufferCom->Bind_Buffers();

    if (FAILED(m_pTextureCom->Bind_Texture(0)))
        return E_FAIL;

    m_pVIBufferCom->Render();

    Reset_RenderState();


    return S_OK;
}

void CFrozenHammer::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CFrozenHammer::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

void CFrozenHammer::OnEvent(_uint iTypeindex, const EVENTDATA* pData)
{
    if (static_cast<EVENT_TYPE>(iTypeindex) == EVENT_TYPE::FROZENHAMMER)
    {
        auto EventData = static_cast<const FROZENHAMMER*>(pData);

        if (m_fCulAttackCool <= 0.f)
        {
            _float4x4 tempMat{};
            D3DXMatrixIdentity(&tempMat);
            _float3 vThrownDir = {};
            _float fThrownPower = 25.f;
            _float fThrownAtkLifeTime = 0.8f;

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

                if (sqrt(fMinDistSq) > 6.f)
                    pNearMonster = nullptr;

                if (pNearMonster != nullptr)
                {
                    vThrownDir = pNearMonster->Get_Transform()->Get_State(STATE::POSITION) - m_pTransformCom->Get_State(STATE::POSITION);
                }
                else {
                    _float3 vDir = EventData->vDir;
                    vThrownDir = vDir;
                }
            }
            else {
                _float3 vDir = EventData->vDir;
                vThrownDir = vDir;
            }
            m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::ITEM_EFFECT));
            m_pGameInstance->PlaySoundW(L"attackMagicalIce.wav", ENUM_CLASS(CHANNELID::ITEM_EFFECT2), g_fEFFECTVolume - 0.7f);
            CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::PLAYER_EFFECT, L"Prototype_Component_Texture_FrozenHammer_Shoot",
                *m_pTransformCom->Get_WorldMatrix(), tempMat, vThrownDir, fThrownPower, fThrownAtkLifeTime, 0.f, true);
            m_fCulAttackCool = m_fAttackCool;
        }

    }
}

HRESULT CFrozenHammer::Ready_Components(void* pArg)
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_FrozenHammer"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    return S_OK;
}

CFrozenHammer* CFrozenHammer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFrozenHammer* pInstance = new CFrozenHammer(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CFrozenHammer"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFrozenHammer::Clone(void* pArg)
{
    CFrozenHammer* pInstance = new CFrozenHammer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CFrozenHammer"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFrozenHammer::Free()
{
    m_pGameInstance->Unsubscribe(ENUM_CLASS(EVENT_TYPE::FROZENHAMMER), this);
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTargetTransform);

}