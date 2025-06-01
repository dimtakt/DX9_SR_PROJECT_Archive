#include "Tree.h"

#include "GameInstance.h"
#include "Client_Struct.h"

CTree::CTree(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CTree::CTree(const CTree& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CTree::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTree::Initialize(void* pArg)
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

    //X축 회전
    if (pObject_Desc->vRotate.x != 0.f || pObject_Desc->vRotate.y != 0.f || pObject_Desc->vRotate.z != 0.f)
        m_pTransformCom->ApplyEulerRotation(pObject_Desc->vRotate);
       
    m_pTransformCom->Set_RotationEuler(pObject_Desc->vRotate.x, pObject_Desc->vRotate.y,pObject_Desc->vRotate.z);

    m_pTransformCom->Scaling(pObject_Desc->vScale.x, pObject_Desc->vScale.y, pObject_Desc->vScale.z);

    m_iTextureIndex = pObject_Desc->iTextureIndex;
    m_pTextureCom->Bind_Texture(m_iTextureIndex);
    m_eObjType = GAMEOBJ_TYPE::OBJECT;

    return S_OK;
}

void CTree::Priority_Update(_float fTimeDelta)
{

}

void CTree::Update(_float fTimeDelta)
{
}

void CTree::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CTree::Render()
{
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIndex)))
        return E_FAIL;

    /* 그리기위해 이용할 자원과 설정들을 장치에 바인딩한다. */
    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();
    return S_OK;
}

HRESULT CTree::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Tree"),
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

CTree* CTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTree* pInstance = new CTree(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTree::Clone(void* pArg)
{
    CTree* pInstance = new CTree(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CTree"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTree::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
}
