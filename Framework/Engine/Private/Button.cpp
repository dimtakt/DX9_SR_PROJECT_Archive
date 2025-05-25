#include "Button.h"
#include "GameInstance.h"

CButton::CButton(LPDIRECT3DDEVICE9 pGraphic_Device) : CUIObject(pGraphic_Device)
{
}

CButton::CButton(const CButton& Prototype) : CUIObject(Prototype)
{
}

HRESULT CButton::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CButton::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

void CButton::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CButton::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CButton::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CButton::Render()
{
	return S_OK;
}

_bool CButton::Click_Check(HWND hWnd)
{
	if (isPick(hWnd))
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			return true;
		}
		return false;
	}
	return false;
}

HRESULT CButton::Bind_ButtonTex_Single(HWND hWnd, _uint iTextureIndex)
{
	if (FAILED(Render_Button(iTextureIndex)))
		return E_FAIL;
	return S_OK;
}

HRESULT CButton::Bind_ButtonTex_Double(HWND hWnd, _uint iTextureIndex_Default, _uint iTextureIndex_NotDefault)
{
	if (isPick(hWnd))
	{
		if (FAILED(Render_Button(iTextureIndex_NotDefault)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Render_Button(iTextureIndex_Default)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CButton::Render_Button(_uint iTextureIndex)
{
	m_pTransformCom->Bind_Matrix();
	if (FAILED(m_pTextureCom->Bind_Texture(iTextureIndex)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();

	__super::Begin();
	m_pVIBufferCom->Render();
	__super::End();

	return S_OK;
}


void CButton::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}