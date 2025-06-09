#include "Field_Font.h"
#include "GameInstance.h"


CField_Font::CField_Font(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CField_Font::CField_Font(const CField_Font& Prototype) : CUIObject(Prototype), m_eType(FIELD_FONT_TYPE::FONT_END), m_eLevel(Prototype.m_eLevel)
{
}
HRESULT CField_Font::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;
	return S_OK;
}

HRESULT CField_Font::Initialize(void* pArg)
{
	FIELD_FONT_DESC* Desc = static_cast<FIELD_FONT_DESC*>(pArg);
	
	m_eType = Desc->eType;
	m_iValue = Desc->iValue;
	m_szText = Desc->szText;
	m_vTextCol = Desc->TextCol;

	m_fAlpha = 1.f;
	m_fSpeed = 5.f;

	m_fSizeX = 100;
	m_fSizeY = 100;
	m_fX = 0;
	m_fY = 0;
	m_fZ = UI_DEPTH::FILED_FONT;
	m_iWinSizeX = g_iWinSizeX;
	m_iWinSizeY = g_iWinSizeY;


	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	__super::Update_Position();
	m_vTarget_Pos = Desc->pTransform->Get_State(STATE::POSITION);

	m_pGameInstance->Add_UIObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Field_States"), this);

	Target_Pos_Set(m_vTarget_Pos);
	m_WindowY = m_pTransformCom->Get_State(STATE::POSITION).y;
	return S_OK;
}

void CField_Font::Priority_Update(_float fTimeDelta)
{

}

void CField_Font::Update(_float fTimeDelta)
{
	if (m_fAlpha <= 0)
		m_bDead = true;

	m_fAccTime += fTimeDelta;
	_float fT = m_fAccTime / 1.f;
	m_WindowY -= (m_WindowY + 1 - m_WindowY) * (1 - (1 - fT) * (1 - fT));

	m_fAlpha -= 0.8f * fTimeDelta;
}

void CField_Font::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CField_Font::Render()
{
	Target_Pos_Update(m_vTarget_Pos);
	SetUp_RenderState();
	__super::Begin();
	Font_Render();
	__super::End();
	Reset_RenderState();
	return S_OK;
}

HRESULT CField_Font::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CField_Font::Target_Pos_Update(_float3 vTargetPos)
{
	_float3 Target_Pos = vTargetPos;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	_float3 vWinPos{};
	_float4x4 vVPMatrix = m_OldViewMatrix * m_OldProjMatrix;
	D3DXVec3TransformCoord(&vWinPos, &Target_Pos, &vVPMatrix);

	_float fWinPosX = (vWinPos.x + 1.f) * 0.5 * g_iWinSizeX;
	_float fWinPosY = (-vWinPos.y + 1.f) * 0.5 * g_iWinSizeY;

	m_vWorldPos.x = fWinPosX - m_iWinSizeX * 0.5f;
	m_vWorldPos.y = -fWinPosY + m_iWinSizeY * 0.5f;


	m_pTransformCom->Set_State(STATE::POSITION, _float3{ m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z });

}

void CField_Font::Target_Pos_Set(_float3 vTargetPos)
{
	_float3 Target_Pos = vTargetPos;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	_float3 vWinPos{};
	_float4x4 vVPMatrix = m_OldViewMatrix * m_OldProjMatrix;
	D3DXVec3TransformCoord(&vWinPos, &Target_Pos, &vVPMatrix);

	_float fWinPosX = (vWinPos.x + 1.f) * 0.5 * g_iWinSizeX;
	_float fWinPosY = (-vWinPos.y + 1.f) * 0.5 * g_iWinSizeY;

	m_vWorldPos.x = fWinPosX - m_iWinSizeX * 0.5f;
	m_vWorldPos.y = -fWinPosY + m_iWinSizeY * 0.5f;


	m_pTransformCom->Set_State(STATE::POSITION, _float3{ m_vWorldPos.x, m_vWorldPos.y + m_fY, m_vWorldPos.z });
}

void CField_Font::Font_Render()
{
	TCHAR szText[64];
	RECT  Offset = {};

	m_vTexRect.left = (m_vWorldPos.x + g_iWinSizeX * 0.5) - m_fSizeX * 0.5;
	m_vTexRect.top = (m_WindowY + g_iWinSizeY * 0.5) - m_fSizeY * 0.5;
	m_vTexRect.right = (m_vWorldPos.x + g_iWinSizeX * 0.5) + m_fSizeX * 0.5;
	m_vTexRect.bottom = (m_WindowY + g_iWinSizeY * 0.5) + m_fSizeY * 0.5;

	switch (m_eType)
	{
	case CField_Font::FIELD_FONT_TYPE::PLAYER_DAMAGE:

		_stprintf_s(szText, TEXT("%d"), m_iValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_22_Damage"), szText, m_vTexRect, D3DXCOLOR(1.f, 0.2f, 0.2f, m_fAlpha), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	case CField_Font::FIELD_FONT_TYPE::MONSTER_DAMAGE:
		_stprintf_s(szText, TEXT("%d"), m_iValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_22_Damage"), szText, m_vTexRect, D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	case CField_Font::FIELD_FONT_TYPE::CRITICAL:
		//Offset.left = m_vTexRect.left + 2;
		//Offset.top = m_vTexRect.top + 2;
		//Offset.right = m_vTexRect.right + 2;
		//Offset.bottom = m_vTexRect.bottom + 2;
		//_stprintf_s(szText, TEXT("%d"), m_iValue);
		//m_pGameInstance->Render_Font(TEXT("UI_Font_26_Damage"), szText, Offset, D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		
		_stprintf_s(szText, TEXT("%d"), m_iValue);
		m_pGameInstance->Render_Font(TEXT("UI_Font_26_Damage"), szText, m_vTexRect, D3DXCOLOR(1.f, 0.9f, 0.f, m_fAlpha), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	case CField_Font::FIELD_FONT_TYPE::PARING:
		_stprintf_s(szText, TEXT("PARING"));
		m_pGameInstance->Render_Font(TEXT("UI_Font_22_Damage"), szText, m_vTexRect, D3DXCOLOR(0.f, 0.9f, 1.f, m_fAlpha), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	case CField_Font::FIELD_FONT_TYPE::TEXT:
		m_pGameInstance->Render_Font(TEXT("UI_Font_22_Damage"), m_szText, m_vTexRect, m_vTextCol, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;

	}
	
}

void CField_Font::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CField_Font::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CField_Font* CField_Font::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CField_Font* pInstance = new CField_Font(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created : CField_Font"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CField_Font::Clone(void* pArg)
{
	CField_Font* pInstance = new CField_Font(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CField_Font"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CField_Font::Free()
{
	__super::Free();
}
