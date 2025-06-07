#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CItem_Tooltip_Bottom final : public CUIObject
{
private:
									CItem_Tooltip_Bottom(LPDIRECT3DDEVICE9 pGraphic_Device);
									CItem_Tooltip_Bottom(const CItem_Tooltip_Bottom& Prototype);
	virtual							~CItem_Tooltip_Bottom() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
private:
	HRESULT							Ready_Components();



public:
	static CItem_Tooltip_Bottom*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
