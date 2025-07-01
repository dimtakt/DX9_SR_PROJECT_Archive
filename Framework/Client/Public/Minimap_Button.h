#pragma once
#include "Client_Defines.h"
#include "Button.h"

BEGIN(Client)
class CMinimap_Button final : public CButton
{
public:
	typedef struct tagMinimap_Button :public CUIObject::UIOBJECT_DESC
	{
		_int		RoomID;
		_float3		vRoomPos;
	}MINIMAP_BUTTON_DESC;
private:
									CMinimap_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
									CMinimap_Button(const CMinimap_Button& Prototype);
	virtual							~CMinimap_Button() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	_int							m_iTexIndex = {};
	_float							m_fFrame = {};
	_float							m_fSpeed = {};
	_uint							m_iEventType = {};
	_float3							m_vRoomPos = {};
	_int							m_iRoomID = {};
	_int							m_bClearRoom = {false };
	_bool							m_bIsPlayer = { false };
private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children(_float fX, _float fY);
		
	_bool							Button_Pick();

public:
	static CMinimap_Button*				Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END