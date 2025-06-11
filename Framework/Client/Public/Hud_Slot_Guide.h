#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CHud_Slot_Guide final : public CUIObject
{
public:
	typedef struct tagSlot_Key_Guide : public UIOBJECT_DESC
	{
		_wstring    strKey;
	}SLOT_KEYGUIDE_DESC;
private:
								CHud_Slot_Guide(LPDIRECT3DDEVICE9 pGraphic_Device);
								CHud_Slot_Guide(const CHud_Slot_Guide& Prototype);
	virtual						~CHud_Slot_Guide() = default;
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
	_wstring					m_strKey = {};

private:
	HRESULT						Ready_Components();

public:
	static						CHud_Slot_Guide* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;

};
END
