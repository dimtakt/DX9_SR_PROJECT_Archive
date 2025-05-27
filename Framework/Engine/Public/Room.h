#pragma once
#include "GameObject.h"
#include "GameInstance.h"
#include "LandObject.h"
#include "Monster.h"

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

public:
    void Add_Monster(CMonster* pMonster)
    {
        m_vMonster.push_back(pMonster);
    }
    void Add_Object(CVIBuffer_Rect* pVIBuffer)
    {
        m_vObject.push_back(pVIBuffer);
    }

protected:
    _bool m_bIsActive = false;
    _bool m_bIsVisited = false;
    CLandObject* m_pLand = { nullptr };
    // 맵툴 진행중이라 추후에 어떻게 처리할지 고민중
    vector<CVIBuffer_Rect*> m_vObject = {};
    vector<CMonster*> m_vMonster = {};
    CTransform* m_pTransformCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    ROOM_STATE m_eRoomState = {};

public:
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END