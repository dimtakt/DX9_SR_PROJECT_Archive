#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CField_Npc_Face final : public CUIObject
{
private:
	CField_Npc_Face(LPDIRECT3DDEVICE9 pGraphic_Device);
	CField_Npc_Face(const CField_Npc_Face& Prototype);
	virtual							~CField_Npc_Face() = default;

public:
	void							Render_SmallFace_On(_int iNumber);
	void							Render_SmallFace_Off();

	void							Render_Face_On(_int iNumber, _bool bCinematic);
	void							Render_Face_Off();

	void							Change_Deth(_float fDeth) { m_fZ = fDeth; }
public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
	LEVEL							m_eLevel = {};

	_bool							m_bCinematicFace = { false };
	_bool							m_bIsSmallRender = { false };
	_bool							m_bIsSmallRenderSwitch = { false };
	_bool							m_bIsRender = {false};
	_int							m_iTexIndex = {};

	_float							fAcctime = {};
private:
	HRESULT							Ready_Components();

	void							Render_Font();
	void							Render_Font_Small();
	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();
public:
	static CField_Npc_Face*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
