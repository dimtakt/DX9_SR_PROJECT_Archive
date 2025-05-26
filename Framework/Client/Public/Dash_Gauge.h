#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CPlayerStats;
END
BEGIN(Client)
class CDash_Gauge final : public CUIObject
{
public:
	typedef struct tagCDash_Gauge
	{
		_int iIndex;
	}UIHUD_DASH_GAUGE_DESC;

private:
									CDash_Gauge(LPDIRECT3DDEVICE9 pGraphic_Device);
									CDash_Gauge(const CDash_Gauge& Prototype);
	virtual							~CDash_Gauge() = default;

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
	CPlayerStats*					m_pPlayerStatsCom = { nullptr };
	_float							m_fDash;
	_bool							m_bIsRender = {};
	_int							m_iIndex = {};
private:
	HRESULT							Ready_Components();

	void							SetUp_RenderState();
	void							Reset_RenderState();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	HRESULT							Reday_SyncingObject();

	void							SetUp_Render_Gauge();
	void							Reset_Render_Gauge();

public:
	static CDash_Gauge*				Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
