#pragma once
#include "Client_Defines.h"
#include "Button.h"

BEGIN(Client)
class CTalent_Reset final : public CButton
{
private:
									CTalent_Reset(LPDIRECT3DDEVICE9 pGraphic_Device);
									CTalent_Reset(const CTalent_Reset& Prototype);
	virtual							~CTalent_Reset() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	_bool							m_bHold = { false };
	_int							m_iClickValue = {};

private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	void							Font_Render();

	_bool							Reset_Pick();

public:
	static CTalent_Reset*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END