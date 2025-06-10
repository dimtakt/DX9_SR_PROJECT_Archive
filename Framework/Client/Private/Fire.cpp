#include "Fire.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CFire::CFire(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CFire::CFire(const CFire& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CFire::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFire::Initialize(void* pArg)
{

    MAP_OBJECT_DESC* pObject_Desc = static_cast<MAP_OBJECT_DESC*>(pArg);
 

    if (FAILED(Ready_Components()))
        return E_FAIL;


    if (pArg != nullptr)
    {
        // 조명 설정값
        ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
        m_Light.Type = D3DLIGHT_POINT;

        m_Light.Position = pObject_Desc->vPos;

        m_Light.Range = 10.0f;              //조명이 영향을 주는거리
        m_Light.Attenuation0 = 1.0f;        // 거리에 상관없이 일정한 밝기 값
        m_Light.Attenuation1 = 0.1f;        // 거리만큼 어두워지는 값
        m_Light.Attenuation2 = 0.01f;       // 거리의 제곱에 따라 더 어두워짐

        m_Light.Diffuse = D3DXCOLOR(1.0f, 0.6f, 0.2f, 1.0f);   // 불빛 색
        m_Light.Ambient = D3DXCOLOR(0.3f, 0.2f, 0.1f, 1.0f);   //  전체적인 밝기 
        m_Light.Specular = D3DXCOLOR(0.8f, 0.4f, 0.2f, 1.0f);   // 반사광

        m_iLightIndex = pObject_Desc->iTextureIndex;;  // 텍스처로 0~7 조명 인덱스 값 넣어줄려고 함. 구조체 변경시 터질 위험 있어서...

        // 불 이미지 설정 값
        m_pTransformCom->Set_State(STATE::POSITION, pObject_Desc->vPos);

        //X축 회전
        if (pObject_Desc->vRotate.x != 0.f || pObject_Desc->vRotate.y != 0.f || pObject_Desc->vRotate.z != 0.f)
            m_pTransformCom->ApplyEulerRotation(pObject_Desc->vRotate);

        m_pTransformCom->Set_RotationEuler(pObject_Desc->vRotate.x, pObject_Desc->vRotate.y, pObject_Desc->vRotate.z);

        m_pTransformCom->Scaling(pObject_Desc->vScale.x, pObject_Desc->vScale.y, pObject_Desc->vScale.z);

        m_iTextureIndex = 0;
        m_pTextureCom->Bind_Texture(m_iTextureIndex);
        m_eObjType = pObject_Desc->eType;
        //후에 추가한건데 이전맵 데이터들 꼬일까봐 이렇게 처리해둠.

    }
        
    return S_OK;
}

void CFire::Priority_Update(_float fTimeDelta)
{  
}

void CFire::Update(_float fTimeDelta)
{
}

void CFire::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CFire::Render()
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

void CFire::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 50);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    
}

void CFire::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);

    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFire::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Fire"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /*m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ));

    if (nullptr == m_pVIBufferCom)
        return E_FAIL;

    if (nullptr == Get_Component(TEXT("Com_VIBuffer")))
        m_Components.emplace(TEXT("Com_VIBuffer"), m_pVIBufferCom);

    Safe_AddRef(m_pVIBufferCom);*/

    return S_OK;
}


CFire* CFire::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFire* pInstance = new CFire(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFire::Clone(void* pArg)
{
    CFire* pInstance = new CFire(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CFire"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFire::Free()
{
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    __super::Free();
}
