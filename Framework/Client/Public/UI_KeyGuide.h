#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_KeyGuide final : public CUIObject
{
private:
								CUI_KeyGuide(LPDIRECT3DDEVICE9 pGraphic_Device);
								CUI_KeyGuide(const CUI_KeyGuide& Prototype);
	virtual						~CUI_KeyGuide() = default;
public:
	virtual HRESULT				Initialize_Prototype(const _wstring& strPrototypeTag);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_wstring					m_strKey = {};

private:
	HRESULT						Ready_Components();

public:
	static						CUI_KeyGuide* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _wstring& strPrototypeTag);
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;

};
END
