#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CHud_Exp_Checkout final : public CUIObject
{
public:
	typedef struct tagHud_Exp_Checkout
	{
		float fX;
	}UIHUD_CHECKOUT_DESC;
private:
										CHud_Exp_Checkout(LPDIRECT3DDEVICE9 pGraphic_Device);
										CHud_Exp_Checkout(const CHud_Exp_Checkout& Prototype);
	virtual								~CHud_Exp_Checkout() = default;

public:
	virtual HRESULT						Initialize_Prototype();
	virtual HRESULT						Initialize(void* pArg) override;
	virtual void						Priority_Update(_float fTimeDelta) override;
	virtual void						Update(_float fTimeDelta) override;
	virtual void						Late_Update(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	CVIBuffer_Rect*						m_pVIBufferCom = { nullptr };

private:
	HRESULT								Ready_Components();

public:
	static CHud_Exp_Checkout*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*				Clone(void* pArg) override;
	virtual void						Free() override;

};
END
