#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CDash_Gauge_Frame final : public CUIObject
{
public:
	typedef struct tagCDash_Gauge_Fream
	{
		_float fX;
		_int iIndex;
	}UIHUD_DASH_FREAM_DESC;
private:
									CDash_Gauge_Frame(LPDIRECT3DDEVICE9 pGraphic_Device);
									CDash_Gauge_Frame(const CDash_Gauge_Frame& Prototype);
	virtual							~CDash_Gauge_Frame() = default;

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
	_int							m_iIndex = {};

private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();


public:
	static CDash_Gauge_Frame*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
