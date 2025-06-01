#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CStatus_Window final : public CUIObject
{
private:
									CStatus_Window(LPDIRECT3DDEVICE9 pGraphic_Device);
									CStatus_Window(const CStatus_Window& Prototype);
	virtual							~CStatus_Window() = default;

public:
	void							UI_Switch();
public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	_bool							m_bIsOpen = { false };

private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();


public:
	static CStatus_Window*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END

