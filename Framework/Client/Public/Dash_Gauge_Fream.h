#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CDash_Gauge_Fream final : public CUIObject
{
public:
	typedef struct tagCDash_Gauge_Fream
	{
		float fX;
	}UIHUD_DASH_FREAM_DESC;
private:
									CDash_Gauge_Fream(LPDIRECT3DDEVICE9 pGraphic_Device);
									CDash_Gauge_Fream(const CDash_Gauge_Fream& Prototype);
	virtual							~CDash_Gauge_Fream() = default;

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

private:
	HRESULT							Ready_Components();

	void							SetUp_RenderState();
	void							Reset_RenderState();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

public:
	static CDash_Gauge_Fream*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
