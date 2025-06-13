#include "FireParticle.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CFireParticle::CFireParticle(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CParticle{ pGraphic_Device }
{
}

CFireParticle::CFireParticle(const CFireParticle& Prototype)
    : CParticle{ Prototype }
{
}

HRESULT CFireParticle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFireParticle::Initialize(void* pArg)
{
    if (pArg != nullptr)
    {
        PARTICLE_DESC* pDesc = static_cast<PARTICLE_DESC*>(pArg);
        m_iTextureType = pDesc->iType;          //추후 이거로 같은 기능 텍스처만 다르게 설정할 수 있을듯 ?
    }

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

        m_fLifeTime = pDesc->fLifeTime;
        m_fSpawnTimer = pDesc->fSpwanTimer;
        m_fSpeed = pDesc->fSpeed;
        m_iTextureIndex = pDesc->iTexture;
        if (m_iTextureType != 1)        //먼지면 텍스쳐 인덱스 고정
            m_iTextureIndex = 0;

        m_eObjType = GAMEOBJ_TYPE::PARTICLE;

        m_fCurrentAlpha = 1.f;
    }

    return S_OK;
}

void CFireParticle::Priority_Update(_float fTimeDelta)
{

}

void CFireParticle::Update(_float fTimeDelta)       //한꺼번에 생성해서 각자 따로 움직이게 해줘야한다? 
{
    if (!m_bIsActive)
    {
        _float m_fPlayTimer = 0.f;
        m_fPlayTimer += fTimeDelta;
        if (m_fPlayTimer >= m_fSpawnTimer)
        {
            m_bIsActive = true;
        }
        else
            return;
    }
    
    m_fElapsedTime += fTimeDelta;

    m_vPos = m_pTransformCom->Get_State(STATE::POSITION);
    m_vPos.y += m_fSpeed * fTimeDelta;

    _float fShakeAmount = sinf(m_fElapsedTime * 4.f) * 0.02f;            // 흔들리는 속도, 흔들리는 거리 조절값
    m_vPos.x += fShakeAmount;

    m_pTransformCom->Set_State(STATE::POSITION, m_vPos);
    
    _float fRatio = 1.f - (m_fElapsedTime / (m_fLifeTime + m_fSpawnTimer));         //알파값 설정
    fRatio = max(0.2f, min(1.f, fRatio)); 
    m_fCurrentAlpha = fRatio; 

    if (m_fElapsedTime >= m_fSpawnTimer + m_fLifeTime)
    {
        m_bActive = false;
        m_fElapsedTime = 0.f;
        m_pTransformCom->Set_State(STATE::POSITION, m_vStartPos);
    }
}

void CFireParticle::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CFireParticle::Render()
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

void CFireParticle::SetUp_RenderState()
{

    // 조금 쨍한 버전


    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 50);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);    
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);


 

    //m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    //m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    //m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    //// 흐리게 할려면 알파 테스트 반드시 꺼야 한다고 함.
    //m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //// 알파 적용
    //DWORD dwAlpha = static_cast<DWORD>(m_fCurrentAlpha * 255.f);
    //m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(dwAlpha, 255, 255, 255));

    //// 컬러도 팩터 곱해주기
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

    //// 알파 팩터 적용
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
}



void CFireParticle::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);    
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);


    //  // 기본적인 렌더 상태 복원
    //m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    //m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    //m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    //// TextureFactor 초기화 (알파 255로)
    //m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    //m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

    //m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    //m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    //m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    //m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CFireParticle::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;


    if (m_iTextureType == 1)
    {
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_FireParticle"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
    }
    else if (m_iTextureType == 2)
    {
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_DustParticle"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
    }
    else if (m_iTextureType == 3)
    {
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_LightParticle"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
    }

    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;


    return S_OK;
}

CFireParticle* CFireParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFireParticle* pInstance = new CFireParticle(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CFireParticle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFireParticle::Clone(void* pArg)
{
    CFireParticle* pInstance = new CFireParticle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CFireParticle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFireParticle::Free()
{
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);

    __super::Free();
}
