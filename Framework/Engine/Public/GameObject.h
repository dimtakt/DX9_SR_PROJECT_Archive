#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	class CComponent* Find_Component(const _wstring& strComponentTag);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();


public:
	_bool Get_IsDead() { return m_bDead; }
	_bool Get_IsActive() { return m_bActive; }
	GAMEOBJ_TYPE Get_ObjType() { return m_eObjType; }
public:
	void Set_IsDead(_bool bDead) { m_bDead = bDead; }	void Set_IsActive(_bool bActive) { m_bActive = bActive; }

public:
	virtual void OnCollision(CGameObject* pGameObject);

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	map<const _wstring, class CComponent*>		m_Components;

	_bool						m_bDead = {};
	GAMEOBJ_TYPE				m_eObjType = {};
	_bool						m_bActive = { true };

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag,
		const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END