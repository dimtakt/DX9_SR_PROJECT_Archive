#include "Potal.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CPotal::CPotal(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CPotal::CPotal(const CPotal& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CPotal::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPotal::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (pArg == nullptr)
    {
        m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, 0.f, 0.f));
        return S_OK;
    }

    OBJECT_INTERACTION_DESC* pObject_Desc = static_cast<OBJECT_INTERACTION_DESC*>(pArg);

    m_pTransformCom->Set_State(STATE::POSITION, pObject_Desc->vPos);

    //X축 회전
    if (pObject_Desc->vRotate.x != 0.f || pObject_Desc->vRotate.y != 0.f || pObject_Desc->vRotate.z != 0.f)
        m_pTransformCom->ApplyEulerRotation(pObject_Desc->vRotate);
       
    if(pObject_Desc->vRotate != nullptr)
      m_pTransformCom->Set_RotationEuler(pObject_Desc->vRotate.x, pObject_Desc->vRotate.y,pObject_Desc->vRotate.z);

    if (pObject_Desc->vRotate != nullptr)
    m_pTransformCom->Scaling(pObject_Desc->vScale.x, pObject_Desc->vScale.y, pObject_Desc->vScale.z);

    m_iTextureIndex = pObject_Desc->iTextureIndex;
    m_pTextureCom->Bind_Texture(m_iTextureIndex);
    m_eDirType = pObject_Desc->ePotalType;

    return S_OK;
}

void CPotal::Priority_Update(_float fTimeDelta)
{

}

void CPotal::Update(_float fTimeDelta)
{
}

void CPotal::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CPotal::Render()
{
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIndex)))
        return E_FAIL;

    /* 그리기위해 이용할 자원과 설정들을 장치에 바인딩한다. */
    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();
    return S_OK;
}

HRESULT CPotal::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_SHARED), TEXT("Prototype_Component_Texture_Potal"),
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

CPotal* CPotal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPotal* pInstance = new CPotal(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPotal::Clone(void* pArg)
{
    CPotal* pInstance = new CPotal(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CPotal"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPotal::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
}
