#pragma once
#include "Client_Defines.h"
#include "Button.h"

BEGIN(Client)
class CInven_Button final : public CButton
{
private:
									CInven_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
									CInven_Button(const CInven_Button& Prototype);
	virtual							~CInven_Button() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	CGameObject*					m_pTargetUI = { nullptr };
private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	HRESULT							Setting_Target();
public:
	static CInven_Button*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
