#pragma once
#include "Client_Defines.h"
#include "Button.h"

BEGIN(Client)
class CGacha_Slot_Selete final : public CButton
{
private:
	CGacha_Slot_Selete(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGacha_Slot_Selete(const CGacha_Slot_Selete& Prototype);
	virtual							~CGacha_Slot_Selete() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	class CTexture* m_pSelete_TextureCom = { nullptr };
	_bool							m_bIsOver = {};

private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();
public:
	static CGacha_Slot_Selete* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};

END

