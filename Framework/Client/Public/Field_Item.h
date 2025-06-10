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
		_int			m_iGold;
	}FIELD_ITEM_DESC;
private:
							CField_Item(LPDIRECT3DDEVICE9 pGraphic_Device);
							CField_Item(const CField_Item& Prototype);
	virtual					~CField_Item() = default;

public:
	//랜더 아이템 업데이트에서 호출 하면됩니다. 폰트 랜더 끄면 텍스트 출력 안되고 아이템만 보여줌
	void					Render_Field_Item(_float fTimeDelta, _bool bFontRender);
	
	//구매 함수 돈 부족하면 구매 안됨 (충돌 지역에서 F키 눌렀을 때 함수 사용하시면 됩니다.)
	void					Buy_Item();

public:
	virtual HRESULT			Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render() override;

public:
	const _int				Get_Value() { return m_iGold; }
private:
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };

	LEVEL					m_eLevel = {};
	_float3					m_vTargetPos;

	_uint					m_iItemID = {};
	_uint					m_iItemTex = {};

	_float					m_fAccTime = {};
	_float					m_fSpeed = {};
	_float					m_fRange = {};

	_float					m_iGold = {};
private:
	HRESULT					Ready_Components();

	HRESULT					Ready_ChildPrototype(LEVEL eLevel);
	HRESULT					Ready_Children();

public:
	static CField_Item*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END

