#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "Client_Defines_Map.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CChapMap_MapSymbol final : public CUIObject
{
public:
	typedef struct tagMapEvent
	{
		MAP_EVENT	eMap_EVENT;
		_uint		iMax_Event_Count;
		_uint		iEvent_Count;
	}MAP_EVENT_DESC;
private:
									CChapMap_MapSymbol(LPDIRECT3DDEVICE9 pGraphic_Device);
									CChapMap_MapSymbol(const CChapMap_MapSymbol& Prototype);
	virtual							~CChapMap_MapSymbol() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };

	_uint							m_iMapTex = {};
private:
	HRESULT							Ready_Components();

public:
	static CChapMap_MapSymbol*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END

