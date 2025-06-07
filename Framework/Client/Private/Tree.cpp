#include "Tree.h"

#include "GameInstance.h"
#include "Client_Struct.h"
#include "Shader.h"

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
    if (pArg == nullptr)
        return S_OK;

    MAP_OBJECT_DESC* pObject_Desc = static_cast<MAP_OBJECT_DESC*>(pArg);
    auto it = find(m_vecTree.begin(), m_vecTree.end(), pObject_Desc->iTextureIndex);
    if (it != m_vecTree.end())
        m_bIsTree = true;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, pObject_Desc->vPos);

    //X축 회전
    if (pObject_Desc->vRotate.x != 0.f || pObject_Desc->vRotate.y != 0.f || pObject_Desc->vRotate.z != 0.f)
        m_pTransformCom->ApplyEulerRotation(pObject_Desc->vRotate);
       
    m_pTransformCom->Set_RotationEuler(pObject_Desc->vRotate.x, pObject_Desc->vRotate.y,pObject_Desc->vRotate.z);

    m_pTransformCom->Scaling(pObject_Desc->vScale.x, pObject_Desc->vScale.y, pObject_Desc->vScale.z);

    m_iTextureIndex = pObject_Desc->iTextureIndex;
    m_pTextureCom->Bind_Texture(m_iTextureIndex);
    m_eObjType = GAMEOBJ_TYPE::OBJECT;
    if (pObject_Desc->eType == GAMEOBJ_TYPE::OBJECT_DECO)
        m_eObjType = GAMEOBJ_TYPE::OBJECT_DECO;
    //후에 추가한건데 이전맵 데이터들 꼬일까봐 이렇게 처리해둠.

    if (FAILED(Ready_Shader()))
        return E_FAIL;

    return S_OK;
}

void CTree::Priority_Update(_float fTimeDelta)
{

    Shader_Update();
   
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
    SetUp_RenderState();

    m_pVIBufferCom->Bind_Buffers();
    //m_pTransformCom->Bind_Matrix();

    /*if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIndex)))
        return E_FAIL;*/


    _float4x4 ViewMatrix, ProjMatrix;
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
    m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

    if (FAILED(m_pShaderCom->Bind_Matrix("gWorldMatrix", m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("gViewMatrix", &ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("gProjMatrix", &ProjMatrix)))
        return E_FAIL;

    if (m_bIsTree)
    {
        _float fTime = static_cast<_float>(GetTickCount()) * 0.001f;
        if (FAILED(m_pShaderCom->Set_Float("gTime", fTime)))
            return E_FAIL;
    }
    
    std::vector<std::wstring> vecLightKeys = { L"Player_Light" };
    m_pGameInstance->Apply_ToShader(m_pShaderCom, vecLightKeys);

    m_pTextureCom->Bind_Texture(m_pShaderCom, "gTexture", m_iTextureIndex);

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Render();

    m_pShaderCom->End();

    Reset_RenderState();

    return S_OK;
}

void CTree::Shader_Update()
{
    //// 1. 카메라 기준 조명 방향 갱신
    //CTransform* pCameraTransform = static_cast<CTransform*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Camera"))->Find_Component(TEXT("Com_Transform")));
    //CTransform* pPlayerTransform = static_cast<CTransform*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Player"))->Find_Component(TEXT("Com_Transform")));
    //if (pCameraTransform && pPlayerTransform)
    //{
    //    _float3 vCamPos = pCameraTransform->Get_State(STATE::POSITION);
    //    _float3 vPlayerPos = pPlayerTransform->Get_State(STATE::POSITION);
    //    _float3 vLightDir = _float3(-0.4f, -1.0f, -0.7f);
    //    D3DXVec3Normalize(&vLightDir, &vLightDir);

    //    m_pShaderCom->Set_Vector("gLightDir", reinterpret_cast<_float4*>(&vLightDir));
    //    m_pShaderCom->Set_Vector("gPlayerPos", reinterpret_cast<_float4*>(&vPlayerPos));
    //}
}

void CTree::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CTree::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pGraphic_Device->SetTexture(0, NULL);

    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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

    if (m_bIsTree) {
        ///* For.Com_Shader */
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Shader_Tree"),
            TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
            return E_FAIL;
    }        
    else
    {
        ///* For.Com_Shader */
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Shader_Light"),
            TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
            return E_FAIL;
    }

    /*m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ));

    if (nullptr == m_pVIBufferCom)
        return E_FAIL;

    if (nullptr == Get_Component(TEXT("Com_VIBuffer")))
        m_Components.emplace(TEXT("Com_VIBuffer"), m_pVIBufferCom);

    Safe_AddRef(m_pVIBufferCom);*/

    return S_OK;
}

HRESULT CTree::Ready_Shader()
{
    //// 난반사 빛 (강한 태양광 느낌, 흰색에 살짝 노란 톤)
    //_float4 vLightDiffuse = _float4(2.f, 2.f, 1.8f, 1.f);  // R > B
    //m_pShaderCom->Set_Vector("gLightDiffuse", &vLightDiffuse);

    //// 스펙큘러 (날카로운 반사광, 하이라이트 강조)
    //_float4 vLightSpecular = _float4(5.0f, 5.0f, 5.0f, 1.f);  // 최대치까지 올려도 좋음
    //m_pShaderCom->Set_Vector("gLightSpecular", &vLightSpecular);

    //// 주변광 (거의 제거 → 실루엣 강조 목적)
    //_float4 vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
    //m_pShaderCom->Set_Vector("gAmbient", &vAmbient);

    // 머티리얼 스펙큘러 (표면 반사 성질: 은색 또는 흰색 반사)
    _float4 vMaterialSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);
    m_pShaderCom->Set_Vector("gMaterialSpecular", &vMaterialSpecular);

    //// 반짝임 날카롭게 (값이 클수록 하이라이트가 작고 강하게)
    //m_pShaderCom->Set_Float("gSpecularPower", 128.f); // 또는 128.f

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
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
    __super::Free();
}
