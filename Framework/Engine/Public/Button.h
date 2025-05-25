#pragma once
#include "UIObject.h"

BEGIN(Engine)
class ENGINE_DLL CButton abstract : public CUIObject
{
protected:
	CButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CButton(const CButton& Prototype);
	virtual ~CButton() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

protected:
	virtual _bool				Click_Check(HWND hWnd);
	HRESULT						Bind_ButtonTex_Single(HWND hWnd, _uint iTextureIndex);
	HRESULT						Bind_ButtonTex_Double(HWND hWnd, _uint iTextureIndex_Default, _uint iTextureIndex_NotDefault);

private:
	HRESULT						Render_Button(_uint iTextureIndex);

protected:
	class CTexture*				m_pTextureCom = {nullptr};
	class CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };

public:
	virtual CGameObject*		Clone(void* pArg) = 0;
	virtual void				Free();
};
END
