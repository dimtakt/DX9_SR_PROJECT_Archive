#pragma once
#include "Client_Defines.h"
#include "ProgressBar.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END
BEGIN(Client)
class CHud_Exp final : public CProgressBar
{
private:
							CHud_Exp(LPDIRECT3DDEVICE9 pGraphic_Device);
							CHud_Exp(const CHud_Exp& Prototype);
	virtual					~CHud_Exp() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

private:
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };

private:
	HRESULT					Ready_Components();
	void					SetUp_RenderState();
	void					Reset_RenderState();
public:
	static	CHud_Exp*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END
