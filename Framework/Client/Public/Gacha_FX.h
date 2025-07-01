#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CGacha_FX final : public CUIObject
{
private:
	CGacha_FX(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGacha_FX(const CGacha_FX& Prototype);
	virtual							~CGacha_FX() = default;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
	_uint							m_iTexIndex = {};
	_float							m_fAccTime = {};
	
	_float							m_fRange = {};
	_float							m_fSpeed = {};
private:
	HRESULT							Ready_Components();

	void							Render_Font();
public:
	static CGacha_FX* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END
