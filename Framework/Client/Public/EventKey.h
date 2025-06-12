#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEventKey final : public CUIObject
{
public:
	typedef struct tagSlot_Key_Guide : public UIOBJECT_DESC
	{
		_wstring    strKey;
	}SLOT_KEYGUIDE_DESC;

private:
								CEventKey(LPDIRECT3DDEVICE9 pGraphic_Device);
								CEventKey(const CEventKey& Prototype);
	virtual						~CEventKey() = default;
public:
	void						Input_KeySetting();
	void						Clear_KeySetting();
	void						Reset_KeySetting();

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };
	_int						m_iTexIndex = {};
	_wstring					m_strKey = {};

private:
	HRESULT						Ready_Components();

public:
	static						CEventKey* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;

};
END
