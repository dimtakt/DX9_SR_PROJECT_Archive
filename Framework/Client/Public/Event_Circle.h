#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEvent_Circle final : public CUIObject
{
private:
	CEvent_Circle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEvent_Circle(const CEvent_Circle& Prototype);
	virtual						~CEvent_Circle() = default;

public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };

	_bool						m_bIsClick = { false };
	_bool						m_bIsRender = { false };
	_int						m_iTexIndex = {};
	LEVEL						m_eLevel = {};
	_float						m_fCircle_Size = {};

	_wstring					m_strKey = {};
private:
	HRESULT						Ready_Components();

	void						Set_Renderstate();
	void						Reset_Renderstate();

	_int						KeyCheck();
	HRESULT						Ready_ChildPrototype(LEVEL eLevel);
	HRESULT						Ready_Children();

public:
	static						CEvent_Circle* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;

};
END
