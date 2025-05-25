#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CInventory final : public CUIObject
{
private:
								CInventory(LPDIRECT3DDEVICE9 pGraphic_Device);
								CInventory(const CInventory& Prototype);
	virtual						~CInventory() = default;
public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

public:
	void						UI_Switch();
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	LEVEL						m_eLevel = {};
	_bool						m_bIsOpen = {};

private:
	HRESULT						Ready_Components();

	HRESULT						Ready_ChildPrototype(LEVEL eLevel);
	HRESULT						Ready_Children();

public:
	static CInventory*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;

};
END
