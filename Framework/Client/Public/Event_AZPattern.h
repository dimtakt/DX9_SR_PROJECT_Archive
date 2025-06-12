#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
#include "EventKey.h"
#include "Erma.h"
BEGIN(Client)
class CEvent_AZPattern final : public CUIObject
{
private:
									CEvent_AZPattern(LPDIRECT3DDEVICE9 pGraphic_Device);
									CEvent_AZPattern(const CEvent_AZPattern& Prototype);
	virtual							~CEvent_AZPattern() = default;

public:
	void							Start_Event();
	void							End_Event();


public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	LEVEL							m_eLevel = {};
	CErma*							m_pBoss = { nullptr };

	_bool							m_bIsEvent = { false };
	_bool							m_bIsResult = { false };
	_bool							m_bIsClear = { false };
	_bool							m_bIsOver = {false};
	_bool							m_bReset = { false };
	_bool							m_bOneClick = { false };
	_bool							m_bIsStart = { false };

	_float							m_fAcctime = {};
	
	_float							m_fEventTime = {};
	_float							m_fEventTimeOver = {};

	vector<CEventKey*>				m_vecEventKey;
	vector<_int>					m_vecInputKey_Boss;

private:
	HRESULT							Setting_Event();
	void							Reset_Player();
	void							Reset_Boss();

	void							Reset_Pattern();
private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();


public:
	static CEvent_AZPattern* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END
