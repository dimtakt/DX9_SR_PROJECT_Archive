#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CTalent_Point final : public CUIObject
{
private:
	CTalent_Point(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTalent_Point(const CTalent_Point& Prototype);
	virtual							~CTalent_Point() = default;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_uint							m_iValue = {};
private:
	HRESULT							Ready_Components();
	void							Font_Render();

public:
	static CTalent_Point* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END
