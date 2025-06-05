#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CMinimap_Player final : public CUIObject
{
private:
									CMinimap_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
									CMinimap_Player(const CMinimap_Player& Prototype);
	virtual							~CMinimap_Player() = default;

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

private:
	HRESULT							Ready_Components();
public:
	static CMinimap_Player*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
