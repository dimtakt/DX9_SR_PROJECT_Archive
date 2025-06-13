#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEvent_Circle_Guide final : public CUIObject
{
private:
	CEvent_Circle_Guide(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEvent_Circle_Guide(const CEvent_Circle_Guide& Prototype);
	virtual						~CEvent_Circle_Guide() = default;

public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };

	LEVEL						m_eLevel = {};

private:
	HRESULT						Ready_Components();

	void						Set_Renderstate();
	void						Reset_Renderstate();

public:
	static						CEvent_Circle_Guide* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;

};
END
