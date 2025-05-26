#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CBuff_Type_Base final : public CUIObject
{
public:
	typedef struct tagCBuff_Type_Base
	{
		_int iBuff_Type, iBuff_Index;
	}UIHUD_BUFF_TYPE_DESC;
private:
									CBuff_Type_Base(LPDIRECT3DDEVICE9 pGraphic_Device);
									CBuff_Type_Base(const CBuff_Type_Base& Prototype);
	virtual							~CBuff_Type_Base() = default;

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

	_int							m_iBuff_Type = {};
	_int							m_iBuff_Index = {};

private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();
public:
	static CBuff_Type_Base*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END

