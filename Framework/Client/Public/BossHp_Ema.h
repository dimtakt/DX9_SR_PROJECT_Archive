#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CBossHp_Ema final : public CUIObject
{
private:
								CBossHp_Ema(LPDIRECT3DDEVICE9 pGraphic_Device);
								CBossHp_Ema(const CBossHp_Ema& Prototype);
	virtual						~CBossHp_Ema() = default;
public:
	void						Render_Hpbar(_int iCulTopHp, _int iCulTopMaxHp, _int iCulBottomHp, _int iCulBottomMaxHp, _float fTimeDelta);
	
public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	LEVEL						m_eLevel = {};

private:
	HRESULT						Ready_Components();

	HRESULT						Ready_ChildPrototype(LEVEL eLevel);
	HRESULT						Ready_Children();

public:
	static CBossHp_Ema*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
END