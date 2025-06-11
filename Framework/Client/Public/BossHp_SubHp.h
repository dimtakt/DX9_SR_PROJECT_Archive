#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CBossHp_SubHp final : public CUIObject
{
private:
	CBossHp_SubHp(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBossHp_SubHp(const CBossHp_SubHp& Prototype);
	virtual						~CBossHp_SubHp() = default;
public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	LEVEL						m_eLevel = {};

private:
	HRESULT						Ready_Components();

	HRESULT						Ready_ChildPrototype(LEVEL eLevel);
	HRESULT						Ready_Children();

public:
	static CBossHp_SubHp* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
END