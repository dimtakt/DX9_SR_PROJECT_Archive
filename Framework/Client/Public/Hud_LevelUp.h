#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Client)
class CHud_LevelUp final : public CUIObject
{
private:
									CHud_LevelUp(LPDIRECT3DDEVICE9 pGraphic_Device);
									CHud_LevelUp(const CHud_LevelUp& Prototype);
	virtual							~CHud_LevelUp() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };

	_float							m_fAccTime = {};
	_float							m_fSpeed = {};
	_float							m_fFrame = {};
	_uint							m_iTexIndex = {};
private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();
	void							Render_Font();
public:
	static CHud_LevelUp*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END