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
	virtual HRESULT				Initialize(void* pArg = nullptr);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	virtual void				Push_Item(class CItemObject* pItem);
	virtual void				Push_Item_Count(_uint iItemCount);
	virtual void				IsPick_off();
	_int						Get_Value() { return m_iValue; }
protected:
	virtual _bool				Check_Key_Down(HWND hWnd , _int iKey);
	virtual _bool				Check_Key_UP(HWND hWnd, _int iKey);

	HRESULT						Bind_ButtonTex_Single(HWND hWnd, _uint iTextureIndex);
	HRESULT						Bind_ButtonTex_Double(HWND hWnd, _uint iTextureIndex_Default, _uint iTextureIndex_NotDefault);

protected:
	HRESULT						Render_Button(_uint iTextureIndex);

protected:
	class CTexture*				m_pTextureCom = {nullptr};
	class CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	_float						m_iValue = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void				Free();
};
END
