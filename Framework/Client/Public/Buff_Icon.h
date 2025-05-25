#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CBuff_Icon final : public CUIObject
{
public:
	typedef struct tagCBuff_Icon
	{
		_int iBuff_Index;
	}UI_BUFF_ICON;

private:
									CBuff_Icon(LPDIRECT3DDEVICE9 pGraphic_Device);
									CBuff_Icon(const CBuff_Icon& Prototype);
	virtual							~CBuff_Icon() = default;

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

	_int							m_iBuff_Index = {};
private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();
public:
	static CBuff_Icon*				Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
