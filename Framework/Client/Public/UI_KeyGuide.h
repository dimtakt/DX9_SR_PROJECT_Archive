#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CUI_KeyGuide final : public CUIObject
{
public:
	typedef struct tagKey_Guide : public UIOBJECT_DESC
	{
		CTransform* pTransform = nullptr;
		_bool		bTarget = false;
		_bool		Default = false;
		_wstring    strKey;
	}KEYGUIDE_DESC;
private:
								CUI_KeyGuide(LPDIRECT3DDEVICE9 pGraphic_Device);
								CUI_KeyGuide(const CUI_KeyGuide& Prototype);
	virtual						~CUI_KeyGuide() = default;
public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };
	_wstring					m_strKey = {};
	RENDERGROUP					m_eRenderGroup = {};
	CTransform*					m_pTarget_Transform = { nullptr };
	_bool						m_bTarget = { false };

	_float						m_iWinPosX = {};
	_float						m_iWinPosY = {};

private:
	HRESULT						Ready_Components();
	void						Target_Pos();
public:
	static						CUI_KeyGuide* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;

};
END
