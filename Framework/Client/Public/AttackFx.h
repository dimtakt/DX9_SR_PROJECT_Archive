#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)
class CAttackFx final : public CUIObject
{
public:
	void						Render_Frame(CTransform* pTransform, _int iCulValue, _int iMaxValue);
private:
								CAttackFx(LPDIRECT3DDEVICE9 pGraphic_Device);
								CAttackFx(const CAttackFx& Prototype);
	virtual						~CAttackFx() = default;
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
	void						Target_Pos(CTransform* pTransform);

	HRESULT					Ready_ChildPrototype(LEVEL eLevel);
	HRESULT					Ready_Children();

public:
	static CAttackFx* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};

END

