#pragma once
#include "Client_Defines.h"
#include "Button.h"

BEGIN(Client)
class CInven_Slot final : public CButton
{
private:
									CInven_Slot(LPDIRECT3DDEVICE9 pGraphic_Device);
									CInven_Slot(const CInven_Slot& Prototype);
	virtual							~CInven_Slot() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	_bool							m_bIsOver = {};

private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();
public:
	static CInven_Slot*				Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

END