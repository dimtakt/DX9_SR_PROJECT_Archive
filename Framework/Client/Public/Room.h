#pragma once
#include "Client_Defines.h"
#include "Room.h"
#include "GameObject.h"
#include "TerrainBox.h"
#include "Monster.h"
#include <fstream>
#include "Client_Struct.h"
#include "Potal.h"

BEGIN(Client)

class CRoom final : public CGameObject
{
private:
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

private:
    HRESULT Ready_Components(void* pArg);
    HRESULT Ready_Objects(void* pArg);

public:
    virtual void Enter();
    virtual void Exit();
    virtual void Activate();
    virtual void Deactivate();
    HRESULT Load_From_File(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _tchar* pLoadFileTag, _int iIndex, _int RoomX, _int RoomZ);
    HRESULT Ready_Potal(_uint iLayerLevelIndex, const _wstring& strLayerTag, _float3 vOffset, POTAL_TYPE eType);
    CPotal* Find_Potal(POTAL_TYPE ePotal);

public:
    _bool GetIsActive() { return m_bIsActive; }
    _bool GetIsVisited() { return m_bIsVisited; }
    _int  GetID() { return m_iID; }
    CTerrainBox* Get_TerrainBox(){ return m_pTerrainBox; }
    vector<CMonster*> Get_MonsterList() { return m_vMonster; }
    _int Get_RoomX() { return m_iRoomX; }
    _int Get_RoomZ() { return m_iRoomZ; }

public:
    void Add_TerrainBox(class CTerrainBox* pTerrainBox){m_pTerrainBox = pTerrainBox;}
    void Add_Monster(class CMonster* pMonster){m_vMonster.push_back(pMonster);}
    void Add_Object(class CGameObject* pVIBuffer){m_vObject.push_back(pVIBuffer);}
    void Insert_ID(_int iID){m_iID = iID;}
    void Compute_ObjectOffset(_int x, _int z);

protected:
    _int m_iID = {};
    _bool m_bIsActive = false;
    _bool m_bIsVisited = false;
    CTerrainBox* m_pTerrainBox = { nullptr };
    // 맵툴 진행중이라 추후에 어떻게 처리할지 고민중
    vector<CGameObject*> m_vObject = {};
    vector<CMonster*> m_vMonster = {};
    ROOM_STATE m_eRoomState = {};
    list<MAP_OBJECT_DESC> m_Object_Desc = {};
    vector<CPotal*> m_vPotal = {};

    _int m_iRoomX = {};
    _int m_iRoomZ = {};

    _float3 m_ObjectOffset = {};

public:
    static CRoom* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END