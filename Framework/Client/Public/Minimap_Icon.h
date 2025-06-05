#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CMinimap_Icon final : public CUIObject
{
private:
									CMinimap_Icon(LPDIRECT3DDEVICE9 pGraphic_Device);
									CMinimap_Icon(const CMinimap_Icon& Prototype);
	virtual							~CMinimap_Icon() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLEVEL);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
	LEVEL							m_eLEVEL = {};
	_float							m_fFrame = {};
	_float							m_fSpeed = {};
	_uint							m_iValue = {};
	_uint							m_iTexIndex = {};
private:
	HRESULT							Ready_Components();
	_wstring						Value_Text();
public:
	static CMinimap_Icon*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
