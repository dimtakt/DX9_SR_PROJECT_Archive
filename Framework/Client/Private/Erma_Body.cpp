#include "Erma_Body.h"
#include "Effect_Factory.h"

CErma_Body::CErma_Body(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{
}

CErma_Body::CErma_Body(const CErma_Body& Prototype)
    : CMonster{ Prototype }
{
}

HRESULT CErma_Body::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CErma_Body::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(this->Ready_Components(pArg)))
        return E_FAIL;


    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    CTransform* pTerrainTransform = dynamic_cast<CTransform*>(desc->pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
    _float3 fTerrainPos = pTerrainTransform->Get_State(STATE::POSITION);
    _float3 fTerrainScale = pTerrainTransform->Get_Scaled();

    // 위치 정해지는 부분. from __super
    m_pTransformCom->Set_State(STATE::POSITION, _float3(
        fTerrainPos.x,
        0.f,
        fTerrainPos.z + fTerrainScale.z / 2 - 1.2f));

    // 크기 조정
    m_pTransformCom->Scaling(6.f, 3.f, 3.f);







    m_isSummoned = true;
    //Ready_Object();

    // 임시
    m_iMaxHp = 500;
    m_iCulHp = 500;

    return S_OK;
}

void CErma_Body::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
    //if (m_pHpBar != nullptr &&
    //    m_isSummoned)
    //    m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);

    //if (m_iCulHp <= 0)
    //    m_bDead = true;
}

void CErma_Body::Update(_float fTimeDelta)
{
    // Update
    // 


    if (m_pTerrainBox != nullptr) {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.7f, 0.05f));
    }
}

void CErma_Body::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CErma_Body::Render()
{
    if (!m_pTransformCom)
        return S_OK;


    m_pTransformCom->Bind_Matrix();

    m_pAnimatorCom->Update_State(); // Bind_Texture

    m_pVIBufferCom->Bind_Buffers();

    SetUp_RenderState();

    m_pVIBufferCom->Render();

    m_pTerrainBox->Render();


    Reset_RenderState();

    return S_OK;
}

HRESULT CErma_Body::Ready_Components(void* pArg)
{
    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    // 컴포넌트 추가
    /* For.Com_Texture */
    // Body_Broken
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Body_Broken"),
        TEXT("Com_Texture_Body_Broken"), reinterpret_cast<CComponent**>(&m_pTextureCom_Body_Broken))))
        return E_FAIL;
    // Body
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_BigGolem_Body"),
        TEXT("Com_Texture_Body"), reinterpret_cast<CComponent**>(&m_pTextureCom_Body))))
        return E_FAIL;


    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Boss_Arma_Body";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;
    
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    // State 삽입
    m_pAnimatorCom->Add_State(L"Idle",      { m_pTextureCom_Body, 4, true });
    m_pAnimatorCom->Add_State(L"Broken",    { m_pTextureCom_Body_Broken, 4, true });





    return S_OK;
}

HRESULT CErma_Body::Ready_Object()
{
    m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));

    return S_OK;
}

void CErma_Body::OnCollision(CGameObject* pGameObject)
{
    __super::OnCollision(pGameObject);

    switch (pGameObject->Get_ObjType())
    {
    case GAMEOBJ_TYPE::PLAYER_EFFECT:
        break;
    }
}

CErma_Body* CErma_Body::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CErma_Body* pInstance = new CErma_Body(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CErma_Body"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


CGameObject* CErma_Body::Clone(void* pArg)
{
    CErma_Body* pInstance = new CErma_Body(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CErma_Body"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CErma_Body::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom_Body);
    Safe_Release(m_pTextureCom_Body_Broken);


    Safe_Release(m_pAnimatorCom);
}