#pragma once
#include "Client_Defines.h"
#include "Button.h"

BEGIN(Client)
class CGacha_Pass final : public CButton
{
private:
	CGacha_Pass(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGacha_Pass(const CGacha_Pass& Prototype);
	virtual							~CGacha_Pass() = default;

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

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	void							Font_Render();

	_bool							Button_Pick();

public:
	static CGacha_Pass* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END