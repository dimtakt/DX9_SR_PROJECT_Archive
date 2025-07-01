#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
#include "Item_Base.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CSlate_Tooltip_Slot final : public CUIObject
{
private:
									CSlate_Tooltip_Slot(LPDIRECT3DDEVICE9 pGraphic_Device);
									CSlate_Tooltip_Slot(const CSlate_Tooltip_Slot& Prototype);
	virtual							~CSlate_Tooltip_Slot() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	void							Set_Vlaue(_int iValue) { m_iValue = iValue; }
	void							Set_Item(CItemObject* pItem) { m_pItemObject = pItem; }
private:
	LEVEL							m_eLevel = {};
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
	CItemObject*					m_pItemObject = { nullptr };

	_int							m_iValue = {};
private:
	HRESULT							Ready_Components();
	void							Render_Font();

public:
	static CSlate_Tooltip_Slot*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
