#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CHud_Exp_Frame final : public CUIObject
{
private:
								CHud_Exp_Frame(LPDIRECT3DDEVICE9 pGraphic_Device);
								CHud_Exp_Frame(const CHud_Exp_Frame& Prototype);
	virtual						~CHud_Exp_Frame() = default;
public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	CVIBuffer_Rect*				m_pVIBufferCom1 = { nullptr };
	CVIBuffer_Rect*				m_pVIBufferCom2 = { nullptr };
	LEVEL						m_eLevel = {};
	
	_float						m_fSizeY2 = {};

private:
	HRESULT						Ready_Components();

	HRESULT						Ready_ChildPrototype(LEVEL eLevel);
	HRESULT						Ready_Children();

	void						Render_Buffer1();
	void						Render_Buffer2();

public:
	static CHud_Exp_Frame*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
END