#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CAnimator;
END

BEGIN(Client)
class CLoding_Ani final : public CUIObject
{
private:
									CLoding_Ani(LPDIRECT3DDEVICE9 pGraphic_Device);
									CLoding_Ani(const CLoding_Ani& Prototype);
	virtual							~CLoding_Ani() = default;

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
	CAnimator*						m_pAnimatorCom = { nullptr };

	_uint							m_iTexIndex = {};
	_float							m_fSpeed = {};
private:
	HRESULT							Ready_Components();
public:
	static CLoding_Ani*				Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
