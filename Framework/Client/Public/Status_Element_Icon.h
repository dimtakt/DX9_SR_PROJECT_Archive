#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CStatus_Element_Icon final : public CUIObject
{
private:
									CStatus_Element_Icon(LPDIRECT3DDEVICE9 pGraphic_Device);
									CStatus_Element_Icon(const CStatus_Element_Icon& Prototype);
	virtual							~CStatus_Element_Icon() = default;

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

	_uint							m_iIndex = {};

	_int							m_iMaxValue = {};
	_int							m_iCulValue = {};

private:
	HRESULT							Ready_Components();

	void							Update_Value();
	void							Font_Render();

public:
	static CStatus_Element_Icon*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
