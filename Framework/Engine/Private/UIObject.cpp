#include "UIObject.h"
#include "GameInstance.h"
CUIObject::CUIObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CUIObject::CUIObject(const CUIObject& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CUIObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
    D3DXMatrixIdentity(&m_ViewMatrix);

    
    D3DVIEWPORT9    ViewportDesc{};
    //현재 디바이스에 뷰포트를 가져옴
    m_pGraphic_Device->GetViewport(&ViewportDesc);
    
    //직교 투영 행렬 생성 (저장할 행렬, 너비, 높이, 니어, 파)
    D3DXMatrixOrthoLH(&m_ProjMatrix, ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f);
    
    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

    m_fX = pDesc->fX;
    m_fY = pDesc->fY;
    m_fZ = pDesc->fZ;
    m_fSizeX = pDesc->fSizeX;
    m_fSizeY = pDesc->fSizeY;
    m_iWinSizeX = pDesc->iWinSizeX;
    m_iWinSizeY = pDesc->iWinSizeY;

    return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
    for (auto child : m_vecChildren)
        child->Priority_Update(fTimeDelta);
}

void CUIObject::Update(_float fTimeDelta)
{
    for (auto child : m_vecChildren)
        child->Update(fTimeDelta);
}

void CUIObject::Late_Update(_float fTimeDelta)
{
    for (auto child : m_vecChildren)
        child->Late_Update(fTimeDelta);
}

HRESULT CUIObject::Render()
{
	return S_OK;
}
void CUIObject::Begin()
{
    //현재 뷰, 투영 행렬 저장
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
    m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

    //UI 행렬 적용
    m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
}

void CUIObject::End()
{
    //이전 행렬로 되돌리기
    m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_OldViewMatrix);
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);
}

_bool CUIObject::isPick(HWND hWnd)
{
	POINT			ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	RECT			rcUI = { m_vWorldPos.x - m_fSizeX * 0.5f, m_vWorldPos.y - m_fSizeY * 0.5f, m_vWorldPos.x + m_fSizeX * 0.5f, m_vWorldPos.y + m_fSizeY * 0.5f };

	return PtInRect(&rcUI, ptMouse);
}

void CUIObject::Update_Position()
{
    if (m_pParent)
    {
        m_vWorldPos.x = m_pParent->m_vWorldPos.x + m_fX;
        m_vWorldPos.y = m_pParent->m_vWorldPos.y + m_fY;
        m_vWorldPos.z = m_vWorldPos.z;
    }
    else
    {
        m_vWorldPos.x = m_fX;
        m_vWorldPos.y= m_fY;
        m_vWorldPos.z = m_fZ;
    }
    
    m_pTransformCom->Set_State(STATE::POSITION, _float3{ m_vWorldPos.x - m_iWinSizeX * 0.5f, -m_vWorldPos.y + m_iWinSizeY * 0.5f, m_vWorldPos.z });
    for (auto child : m_vecChildren)
        child->Update_Position();
}

void CUIObject::Add_Child(CUIObject* pChildUI)
{
    pChildUI->m_pParent = this; 
    m_vecChildren.push_back(pChildUI); 
    
    pChildUI->Update_Position();
   
}

void CUIObject::Free()
{
    for (auto& pChildren : m_vecChildren)
        Safe_Release(pChildren);
    m_vecChildren.clear();

    m_pParent = nullptr;

    __super::Free();

    Safe_Release(m_pTransformCom);
    
}
