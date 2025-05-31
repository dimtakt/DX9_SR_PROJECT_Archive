#pragma once
#include "Client_Defines.h"
#include "Client_Defines_Map.h"
#include "Button.h"

BEGIN(Client)
class CChapMap_Button final : public CButton
{
private:
									CChapMap_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
									CChapMap_Button(const CChapMap_Button& Prototype);
	virtual							~CChapMap_Button() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	_uint							m_iMapIndex = {};
	_uint							m_iMapTex = {};
	_uint							m_iPlayerLine = {};
private:
	void							Click_Event();
private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

public:

	static CChapMap_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END
