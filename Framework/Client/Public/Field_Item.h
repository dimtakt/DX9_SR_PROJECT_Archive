#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "Engine_Defines.h"
#include "Client_Defines_Item.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CField_Item final : public CUIObject
{
public:
	typedef struct tagFiled_Item : public UIOBJECT_DESC
	{
		_float3			m_vTargetPos;
		_uint			m_iItemID;
	}FIELD_ITEM_DESC;
private:
							CField_Item(LPDIRECT3DDEVICE9 pGraphic_Device);
							CField_Item(const CField_Item& Prototype);
	virtual					~CField_Item() = default;

public:
	void					Render_Field_Item();
public:
	virtual HRESULT			Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render() override;

private:
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };

	LEVEL					m_eLevel = {};
	_float3					m_vTargetPos;

	_uint					m_iItemID = {};
	_uint					m_iItemTex = {};
private:
	HRESULT					Ready_Components();

	HRESULT					Ready_ChildPrototype(LEVEL eLevel);
	HRESULT					Ready_Children();

	void					Target_Pos();

public:
	static CField_Item*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END

