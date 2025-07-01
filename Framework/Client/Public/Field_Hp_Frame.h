#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)
class CField_Hp_Frame final : public CUIObject
{
public:
	void						Render_HP_Frame(CTransform* pTransform);
private:
								CField_Hp_Frame(LPDIRECT3DDEVICE9 pGraphic_Device);
								CField_Hp_Frame(const CField_Hp_Frame& Prototype);
	virtual						~CField_Hp_Frame() = default;
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

public:
	static CField_Hp_Frame*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

END

