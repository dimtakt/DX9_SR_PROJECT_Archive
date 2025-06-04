#include "Title_Tree.h"
#include "GameInstance.h"
#include "Animator.h"

CTitle_Tree::CTitle_Tree(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CTitle_Tree::CTitle_Tree(const CTitle_Tree& Prototype)
    : CUIObject{ Prototype }
{
}

HRESULT CTitle_Tree::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTitle_Tree::Initialize(void* pArg)
{
    m_fSizeX = 300.f;
    m_fSizeY = 300.f;
    m_fX = 0.f;
    m_fY = -120.f;
    m_fZ = 0.0f;
    m_iWinSizeX = g_iWinSizeX;
    m_iWinSizeY = g_iWinSizeY;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    __super::Update_Position();
    m_pAnimatorCom->Change_State(TEXT("Title_TreeBegin"));

    return S_OK;
}

void CTitle_Tree::Priority_Update(_float fTimeDelta)
{

}

void CTitle_Tree::Update(_float fTimeDelta)
{
    if (m_pAnimatorCom->Get_IsLastFrame() && m_pAnimatorCom->Get_CurStateTag() == TEXT("Title_TreeBegin"))
    {
        m_pAnimatorCom->Change_State(TEXT("Title_Tree"));
        //m_pParent->
    }
        
}

void CTitle_Tree::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CTitle_Tree::Render()
{
    SetUp_RenderState();

    m_pVIBufferCom->Bind_Buffers();

    m_pAnimatorCom->Update_State();

    __super::Begin();
    m_pVIBufferCom->Render();
    __super::End();

    Reset_RenderState();

    return S_OK;
}

HRESULT CTitle_Tree::Ready_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer_Tree"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_Tree"),
        TEXT("Com_Texture_Title_Tree"), reinterpret_cast<CComponent**>(&m_pTextureCom_Title_Tree))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_Title_TreeBegin"),
        TEXT("Com_Texture_Title_TreeBegin"), reinterpret_cast<CComponent**>(&m_pTextureCom_Title_TreeBegin))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform_Title_Tree"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Title_BackGround_Main";

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator_Title_Tree"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;

    m_pAnimatorCom->Add_State(L"Title_TreeBegin", { m_pTextureCom_Title_TreeBegin, 5, true });
    m_pAnimatorCom->Add_State(L"Title_Tree", { m_pTextureCom_Title_Tree, 5, true });

    return S_OK;
}

void CTitle_Tree::SetUp_RenderState()
{
    // 알파 블렌딩 활성화
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // 알파 테스트로 투명한 배경 제거
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 80);  // 이 값보다 낮은 알파는 버림
}

void CTitle_Tree::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    // 색을 섞어서 처리(알파블렌딩)
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CTitle_Tree* CTitle_Tree::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTitle_Tree* pInstance = new CTitle_Tree(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failde to Created : CTitle_Tree"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTitle_Tree::Clone(void* pArg)
{
    CTitle_Tree* pInstance = new CTitle_Tree(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failde to Cloned : CTitle_Tree"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTitle_Tree::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom_Title_Tree);
    Safe_Release(m_pTextureCom_Title_TreeBegin);
    Safe_Release(m_pAnimatorCom);
}
