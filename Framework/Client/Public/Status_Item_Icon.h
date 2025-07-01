#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CStatus_Item_Icon final : public CUIObject
{
private:
									CStatus_Item_Icon(LPDIRECT3DDEVICE9 pGraphic_Device);
									CStatus_Item_Icon(const CStatus_Item_Icon& Prototype);
	virtual							~CStatus_Item_Icon() = default;

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

	_uint							m_iCulItemIndex = {};	//0 단검 1 대검
	_uint							m_iPreItemIndex = {};	//0 단검 1 대검

private:
	HRESULT							Ready_Components();
	void							IconSetting();

public:
	static CStatus_Item_Icon*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
