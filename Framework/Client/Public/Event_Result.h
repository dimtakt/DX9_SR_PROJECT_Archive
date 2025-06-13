#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEvent_Result final : public CUIObject
{
public:
	typedef struct tagResult_Event : public UIOBJECT_DESC
	{
		_wstring    strFont;
	}RESUlT_DESC;

private:
								CEvent_Result(LPDIRECT3DDEVICE9 pGraphic_Device);
								CEvent_Result(const CEvent_Result& Prototype);
	virtual						~CEvent_Result() = default;
public:
	void						Perfect_Setting();
	void						Succes_Setting();
	void						Fail_Setting();

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
	
	_bool						m_bIsRender = {false};
	_int						m_iTexIndex = {};
	_wstring					m_strKey = {};
	_wstring					m_strFont = {};
private:
	HRESULT						Ready_Components();

	void						Set_Renderstate();
	void						Reset_Renderstate();

public:
	static						CEvent_Result* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;

};
END
