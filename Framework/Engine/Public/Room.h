#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRoom : public CGameObject
{
protected:
	CRoom(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRoom(const CRoom& Prototype);
	virtual ~CRoom() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Update(_float fTimeDelta) override;
    virtual void Update(_float fTimeDelta) override;
    virtual void Late_Update(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    virtual void Enter();
    virtual void Exit();
    virtual void Activate();
    virtual void Deactivate();

public:
    _bool GetIsActive() { return m_bIsActive; }
    _bool GetIsVisited() { return m_bIsVisited; }

protected:
    _bool m_bIsActive = false;
    _bool m_bIsVisited = false;

public:
    virtual CRoom* Clone(void* pArg);
    virtual void Free() override;
};

END