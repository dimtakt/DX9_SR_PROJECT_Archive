#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CChapMap_PlayerSymbol final : public CUIObject
{
private:
									CChapMap_PlayerSymbol(LPDIRECT3DDEVICE9 pGraphic_Device);
									CChapMap_PlayerSymbol(const CChapMap_PlayerSymbol& Prototype);
	virtual							~CChapMap_PlayerSymbol() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	void							Player_Move(_float fX, _float fY);
	const _float					Player_OffsetPos() { return m_fY; };

private:
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };

	_float							m_fAngle = {};
	_float							m_fRotSpeed = {};

private:
	HRESULT							Ready_Components();

public:
	static CChapMap_PlayerSymbol*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END

