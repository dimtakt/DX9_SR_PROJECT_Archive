#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)
class CField_Font final : public CUIObject
{
public:
	enum class FIELD_FONT_TYPE {PLAYER_DAMAGE, MONSTER_DAMAGE, CRITICAL, PARING, TEXT, FONT_END };
	
	
	typedef struct Field_Font{
		CTransform*				pTransform;
		_int					iValue = {};
		FIELD_FONT_TYPE			eType;
		_wstring				szText = {};
		D3DXCOLOR				TextCol = {};
	}FIELD_FONT_DESC;

private:
								CField_Font(LPDIRECT3DDEVICE9 pGraphic_Device);
								CField_Font(const CField_Font& Prototype);
	virtual						~CField_Font() = default;
public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;
	
private:
	LEVEL						m_eLevel = {};
	_int						m_iValue = {};
	FIELD_FONT_TYPE				m_eType = {};
	_float						m_fAlpha = {};

	_float						m_WindowX = {};
	_float						m_WindowY = {};
	_float						m_fAccTime = {};
	_float						m_fSpeed = {};

	_float3						m_vTarget_Pos = {};
	_wstring					m_szText = {};
	D3DXCOLOR					m_vTextCol = {};
private:
	HRESULT						Ready_Components();
	void						Target_Pos_Set(_float3 vTargetPos);
	void						Target_Pos_Update(_float3 vTargetPos);

	void						Font_Render();

	void						SetUp_RenderState();
	void						Reset_RenderState();

public:
	static CField_Font*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

END

