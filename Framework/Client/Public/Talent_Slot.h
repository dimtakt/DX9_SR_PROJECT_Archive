#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CTalent_Slot final : public CUIObject
{
private:
									CTalent_Slot(LPDIRECT3DDEVICE9 pGraphic_Device);
									CTalent_Slot(const CTalent_Slot& Prototype);
	virtual							~CTalent_Slot() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	void							Add_Value(_int iValue);
	const _uint						Get_Value() { return m_iCulValue; }
	void							Reset_Value();

private:
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };

	LEVEL							m_eLevel = {};
	_uint							m_iIndex = {};
	_int							m_iPreValue = {};
	_int							m_iCulValue = {};

private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	void							Font_Render();

	void							Player_State_Update();

public:
	static CTalent_Slot*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
