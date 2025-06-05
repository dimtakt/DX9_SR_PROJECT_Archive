#pragma once
#include "Client_Defines.h"
#include "Button.h"

BEGIN(Client)
class CMirror_Button final : public CButton
{
private:
									CMirror_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
									CMirror_Button(const CMirror_Button& Prototype);
	virtual							~CMirror_Button() = default;

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

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	HRESULT							Setting_Target();

public:
	static CMirror_Button*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END

