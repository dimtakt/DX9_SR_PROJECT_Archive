#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEvent_Circle final : public CUIObject
{
public:
	enum class CIRCLE_STATE{ CIRCLE_PLAYING, CIRCLE_PERFECT, CIRCLE_FAIL, CIRCLE_END};
private:
	CEvent_Circle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEvent_Circle(const CEvent_Circle& Prototype);
	virtual						~CEvent_Circle() = default;

public:
	const	CIRCLE_STATE		Get_CircleState() { return m_eState; }
	void						KeyInput_On() { m_bIsKeyInput = true; }

public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };
	LEVEL						m_eLevel = {};
	CIRCLE_STATE				m_eState = {};

	_bool						m_bIsClick = { false };
	_bool						m_bIsRender = { false };
	_bool						m_bIsOver = { false };
	_bool						m_bIsKeyInput = { false };

	_int						m_iTexIndex = {};
	_int						m_iInsertKey = {};

	_float						m_fCircle_Size = {};
	_float						m_fCircle_Speed = {};
	_float						m_fAcctime = {};

	_wstring					m_strKey = {};

	
private:
	HRESULT						Ready_Components();

	void						Set_Renderstate();
	void						Reset_Renderstate();

	_int						KeyCheck();
	HRESULT						Ready_ChildPrototype(LEVEL eLevel);
	HRESULT						Ready_Children();

	void						Set_String();
public:
	static						CEvent_Circle* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;

};
END
