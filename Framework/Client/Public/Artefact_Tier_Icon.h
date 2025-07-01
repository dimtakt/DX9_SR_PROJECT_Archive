#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
#include "Item_Base.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CArtefact_Tier_Icon final : public CUIObject
{
private:
	CArtefact_Tier_Icon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CArtefact_Tier_Icon(const CArtefact_Tier_Icon& Prototype);
	virtual							~CArtefact_Tier_Icon() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	void							Set_Textuer(_int iTexTuer) { m_iTextIndex = iTexTuer; }
private:
	LEVEL							m_eLevel = {};
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
	
	_int							m_iTextIndex = {};
private:
	HRESULT							Ready_Components();

public:
	static CArtefact_Tier_Icon* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END
