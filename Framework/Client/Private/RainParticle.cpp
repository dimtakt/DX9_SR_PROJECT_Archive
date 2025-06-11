#include "RainParticle.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CRainParticle::CRainParticle(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CParticle{ pGraphic_Device }
{
}

CRainParticle::CRainParticle(const CRainParticle& Prototype)
    : CParticle{ Prototype }
{
}

HRESULT CRainParticle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CRainParticle::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (pArg == nullptr)
    {
        m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, 0.f, 0.f));
        return S_OK;
    }

    if (pArg != nullptr)
    {
        PARTICLE_DESC* pDesc = static_cast<PARTICLE_DESC*>(pArg);

        m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPos);

        m_pTransformCom->Scaling(pDesc->vScale.x, pDesc->vScale.y, pDesc->vScale.z);

        m_pTextureCom->Bind_Texture(m_iTextureIndex);
        m_eObjType = GAMEOBJ_TYPE::PARTICLE;
    }

    return S_OK;
}

void CRainParticle::Priority_Update(_float fTimeDelta)
{

}

void CRainParticle::Update(_float fTimeDelta)       //한꺼번에 생성해서 각자 따로 움직이게 해줘야한다? 
{
    if (!m_bIsActive)
    {
        _float m_fPlayTimer = 0.f;
        _float m_fPlayInterval = 5.f;
        m_fPlayTimer += fTimeDelta;
        if (m_fPlayTimer >= m_fPlayInterval)
        {
            m_bIsActive = true;
        }
        else
            return;
    }
    
    m_fElapsedTime += fTimeDelta;

    m_vPos = m_pTransformCom->Get_State(STATE::POSITION);
    m_vPos.y -= m_fSpeed * fTimeDelta;
    m_pTransformCom->Set_State(STATE::POSITION, m_vPos);


    if (m_fElapsedTime >= m_fSpawnTimer + m_fLifeTime)
    {
        m_bActive = false;
        m_fElapsedTime = 0.f;
        m_pTransformCom->Set_State(STATE::POSITION, m_vStartPos);
    }
}

void CRainParticle::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CRainParticle::Render()
{
    SetUp_RenderState();

    m_pTransformCom->Bind_Matrix();
    if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIndex)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

   Reset_RenderState();

    return S_OK;
}

void CRainParticle::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CRainParticle::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CRainParticle::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Rain"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;


    return S_OK;
}

CRainParticle* CRainParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CRainParticle* pInstance = new CRainParticle(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CRainParticle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CRainParticle::Clone(void* pArg)
{
    CRainParticle* pInstance = new CRainParticle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CRainParticle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CRainParticle::Free()
{
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);

    __super::Free();
}
