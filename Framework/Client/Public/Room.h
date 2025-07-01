#pragma once
#include "Client_Defines.h"
#include "Room.h"
#include "GameObject.h"
#include "TerrainBox.h"
#include "Monster.h"
#include <fstream>
#include "Client_Struct.h"
#include "Potal.h"
#include "FogPlane.h"
#include "Fire.h"

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
    HRESULT Load_From_File(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _tchar* pLoadFileTag, _int iIndex, _int RoomX, _int RoomZ, ROOM_INFO Event);
    HRESULT Load_Particle(PARTICLE_TYPE eType, const _wstring& strLayerTag, _uint iLayerLevelIndex, _float3 PariticleScaled ,_int iOffSetType);
    HRESULT Ready_Potal(_uint iLayerLevelIndex, const _wstring& strLayerTag, _float3 vOffset, POTAL_TYPE eType);
    HRESULT Ready_Stage_Potal(_uint iLayerLevelIndex, const _wstring& strLayerTag, _float3 vOffset, POTAL_TYPE eType);
    CPotal* Find_Potal(POTAL_TYPE ePotal);

    HRESULT On_Fire();  //조명테스트
    HRESULT Off_Fire();

public:
    _bool GetIsActive() { return m_bIsActive; }
    _bool GetIsVisited() { return m_bIsVisited; }
    _int  GetID() { return m_iID; }
    CTerrainBox* Get_TerrainBox(){ return m_pTerrainBox; }
    vector<CMonster*> Get_MonsterList();
    _int Get_RoomX() { return m_iRoomX; }
    _int Get_RoomZ() { return m_iRoomZ; }
    void Set_RoomType(ROOM_INFO eType) { m_eRoomType = eType; }
    ROOM_INFO Get_RoomType() { return m_eRoomType; }
    vector<CFire*> Get_FireList() { return m_vFire; }
    void Set_Force_Active_Potal(_bool bActive) { m_bForcePotal_Active = bActive; }
    void Set_ParticleType(PARTICLE_TYPE eType) { m_eRoomParticle = eType; }
    void Set_ParticleOn() { m_bParticle = true; }       //들어간 룸만 켜줘야함. 안그럼 파티클 빨리돔!

public:
    void Add_TerrainBox(class CTerrainBox* pTerrainBox){m_pTerrainBox = pTerrainBox;}
    void Add_Monster(class CMonster* pMonster){m_vMonster.push_back(pMonster);}
    void Add_Object(class CGameObject* pVIBuffer){m_vObject.push_back(pVIBuffer);}
    void Insert_ID(_int iID){m_iID = iID;}
    void Compute_ObjectOffset(_int x, _int z);
    

public:
    CMonster* Find_Monster(MONSTER_TYPE eType);

protected:
    _int m_iID = {};
    _bool m_bIsActive = false;
    _bool m_bIsVisited = false;
    _bool m_bIsClear = false;
    CTerrainBox* m_pTerrainBox = { nullptr };
    //CFogPlane* m_pFogPlane = { nullptr };
    vector<CGameObject*> m_vObject = {};
    vector<CFire*> m_vFire = {};
    vector<CMonster*> m_vMonster = {};
    list<MAP_OBJECT_DESC> m_Object_Desc = {};
    vector<CPotal*> m_vPotal = {};

    _int m_iRoomX = {};
    _int m_iRoomZ = {};

    _float3 m_ObjectOffset = {};

    _bool m_bParticle = false;
    _bool m_bParticleRoom = false;

    PARTICLE_TYPE m_eRoomParticle = PARTICLE_TYPE::PARTICLE_END;

    ROOM_INFO m_eRoomType = {};

    _bool m_bForcePotal_Active = false;

public:
    static CRoom* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END