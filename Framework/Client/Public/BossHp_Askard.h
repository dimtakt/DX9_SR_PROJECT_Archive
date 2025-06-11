#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CBossHp_Askard final : public CUIObject
{
private:
								CBossHp_Askard(LPDIRECT3DDEVICE9 pGraphic_Device);
								CBossHp_Askard(const CBossHp_Askard& Prototype);
	virtual						~CBossHp_Askard() = default;

public:
	void						Render_Hpbar(_int iCulvalue, _int iMaxValue, _float fTimeDelta);
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
	static CBossHp_Askard*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
END