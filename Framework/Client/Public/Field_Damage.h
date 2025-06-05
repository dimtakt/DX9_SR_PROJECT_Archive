#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)
class CField_Damage final : public CUIObject
{
public:
	void						Render_HP_Frame(CTransform* pTransform);
private:
								CField_Damage(LPDIRECT3DDEVICE9 pGraphic_Device);
								CField_Damage(const CField_Damage& Prototype);
	virtual						~CField_Damage() = default;
public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	
	LEVEL						m_eLevel = {};

private:
	HRESULT						Ready_Components();
	void						Target_Pos(CTransform* pTransform);

public:
	static CField_Damage*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

END

