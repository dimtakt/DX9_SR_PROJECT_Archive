#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CTalent_Button_Selete final : public CUIObject
{
private:
	CTalent_Button_Selete(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTalent_Button_Selete(const CTalent_Button_Selete& Prototype);
	virtual							~CTalent_Button_Selete() = default;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };

	_uint							m_iIndex = {};
private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();

public:
	static CTalent_Button_Selete* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END
