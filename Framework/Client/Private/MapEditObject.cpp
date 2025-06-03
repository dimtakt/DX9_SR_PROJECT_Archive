#include "MapEditObject.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CMapEditObject::CMapEditObject(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CMapEditObject::CMapEditObject(const CMapEditObject& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CMapEditObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMapEditObject::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (pArg == nullptr)
    {
        m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, 0.f, 0.f));
        return S_OK;
    }

    MAP_OBJECT_DESC* pObject_Desc = static_cast<MAP_OBJECT_DESC*>(pArg);

    m_pTransformCom->Set_State(STATE::POSITION, pObject_Desc->vPos);

    //XÃà È¸Àü
    if (pObject_Desc->vRotate.x != 0.f || pObject_Desc->vRotate.y != 0.f || pObject_Desc->vRotate.z != 0.f)
        m_pTransformCom->ApplyEulerRotation(pObject_Desc->vRotate);
       
    m_pTransformCom->Set_RotationEuler(pObject_Desc->vRotate.x, pObject_Desc->vRotate.y,pObject_Desc->vRotate.z);

    m_pTransformCom->Scaling(pObject_Desc->vScale.x, pObject_Desc->vScale.y, pObject_Desc->vScale.z);

    m_iTextureIndex = pObject_Desc->iTextureIndex;
    m_pTextureCom->Bind_Texture(m_iTextureIndex);
    m_eObjType = pObject_Desc->eType;

    return S_OK;
}

void CMapEditObject::Priority_Update(_float fTimeDelta)
{

}

void CMapEditObject::Update(_float fTimeDelta)
{
}

void CMapEditObject::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CMapEditObject::Render()
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

void CMapEditObject::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CMapEditObject::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);
}

HRESULT CMapEditObject::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_Component_Texture_Interaction"),
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

CMapEditObject* CMapEditObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMapEditObject* pInstance = new CMapEditObject(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMapEditObject::Clone(void* pArg)
{
    CMapEditObject* pInstance = new CMapEditObject(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CMapEditObject"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMapEditObject::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
}
