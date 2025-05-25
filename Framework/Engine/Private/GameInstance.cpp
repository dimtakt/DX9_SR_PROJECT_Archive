#include "Network_Manager.h"
#include "GameInstance.h"

#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Prototype_Manager.h"
#include "Object_Manager.h"
#include "Renderer.h"
#include "Timer_Manager.h"
#include "Key_Manager.h"
#include "Picking.h"
#include "Collision_Manager.h"
#include "Room_Manager.h"
#include "Font_Manager.h"
#include "Light_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

#pragma region ENGINE
HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, LPDIRECT3DDEVICE9* ppOut)
{
    m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.eWinMode, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppOut);
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;

    m_pLevel_Manager = CLevel_Manager::Create();
    if (nullptr == m_pLevel_Manager)
        return E_FAIL;

    m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
    if (nullptr == m_pPrototype_Manager)
        return E_FAIL;

    m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
    if (nullptr == m_pObject_Manager)
        return E_FAIL;

    m_pRenderer = CRenderer::Create(*ppOut);
    if (nullptr == m_pRenderer)
        return E_FAIL;

    m_pTimer_Manager = CTimer_Manager::Create();
    if (nullptr == m_pTimer_Manager)
        return E_FAIL;

    m_pKey_Manager = CKey_Manager::Create();
    if (nullptr == m_pKey_Manager)
        return E_FAIL;

    m_pNetwork_Manager = CNetwork_Manager::Create();
    if (nullptr == m_pNetwork_Manager)
        return E_FAIL;

    m_pPicking = CPicking::Create(*ppOut, EngineDesc.hWnd, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY);
    if (nullptr == m_pPicking)
        return E_FAIL;

    m_pCollision_Manager = CCollision_Manager::Create();
    if (nullptr == m_pCollision_Manager)
        return E_FAIL;

    m_pRoom_Manager = CRoom_Manager::Create();
    if (nullptr == m_pRoom_Manager)
        return E_FAIL;

    m_pFont_Manager = CFont_Manager::Create(*ppOut);
    if (nullptr == m_pFont_Manager)
        return E_FAIL;

    m_pLight_Manager = CLight_Manager::Create(*ppOut);
    if (nullptr == m_pLight_Manager)
        return E_FAIL;

    return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
    m_pObject_Manager->Priority_Update(fTimeDelta);
    m_pRoom_Manager->Priority_Update(fTimeDelta);

    m_pPicking->Update();

    m_pObject_Manager->Update(fTimeDelta);
    m_pRoom_Manager->Update(fTimeDelta);

    m_pObject_Manager->Late_Update(fTimeDelta);
    m_pRoom_Manager->Late_Update(fTimeDelta);

    m_pLevel_Manager->Update(fTimeDelta);

    m_pKey_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Clear_Resources(_uint iClearLevelID)
{
    m_pPrototype_Manager->Clear(iClearLevelID);

    m_pObject_Manager->Clear(iClearLevelID);

    m_pRoom_Manager->Clear();

    return S_OK;
}

void CGameInstance::Render_Begin(D3DXCOLOR Color)
{
    if (nullptr != m_pGraphic_Device)
        m_pGraphic_Device->Render_Begin(Color);
}

HRESULT CGameInstance::Draw()
{
    if (nullptr == m_pLevel_Manager)
        return E_FAIL;

    if (FAILED(m_pRenderer->Draw()))
        return E_FAIL;

    if (FAILED(m_pLevel_Manager->Render()))
        return E_FAIL;

    return S_OK;
}

void CGameInstance::Render_End(HWND hWnd)
{
    if (nullptr != m_pGraphic_Device)
        m_pGraphic_Device->Render_End();
}
_float CGameInstance::Compute_Random_Normal()
{
    return rand() / static_cast<_float>(RAND_MAX);
}
_float CGameInstance::Compute_Random(_float fMin, _float fMax)
{
    return fMin + (fMax - fMin) * Compute_Random_Normal();
}
#pragma endregion

#pragma region LEVEL_MANAGER
HRESULT CGameInstance::Open_Level(_uint iLevelID, CLevel* pNewLevel)
{
    if (nullptr == m_pLevel_Manager)
        return E_FAIL;

    return m_pLevel_Manager->Open_Level(iLevelID, pNewLevel);
}
#pragma endregion

#pragma region PROTOTYPE_MANAGER
HRESULT CGameInstance::Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
    if (nullptr == m_pPrototype_Manager)
        return E_FAIL;

    return m_pPrototype_Manager->Add_Prototype(iPrototypeLevelIndex, strPrototypeTag, pPrototype);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototype, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
    if (nullptr == m_pPrototype_Manager)
        return nullptr;

    return m_pPrototype_Manager->Clone_Prototype(ePrototype, iPrototypeLevelIndex, strPrototypeTag, pArg);
}
#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring strPrototypeTag, void* pArg)
{
    if (nullptr == m_pObject_Manager)
        return E_FAIL;

    return m_pObject_Manager->Add_GameObject_ToLayer(iLayerLevelIndex, strLayerTag, iPrototypeLevelIndex, strPrototypeTag, pArg);
}
CComponent* CGameInstance::Get_Component(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
    return m_pObject_Manager->Get_Component(iLayerLevelIndex, strLayerTag, strComponentTag, iIndex);
}
#pragma endregion

#pragma region RENDERER
HRESULT CGameInstance::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
    if (nullptr == m_pRenderer)
        return E_FAIL;

    return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}
#pragma endregion

#pragma region TIMER_MANAGER
_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
    return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
    return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Compute_TimeDelta(const _wstring& strTimerTag)
{
    m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}
#pragma endregion 

#pragma region KEY_MANAGER
void CGameInstance::AddTrackingKey(int iKey)
{
    return m_pKey_Manager->AddTrackingKey(iKey);
}
bool CGameInstance::IsKeyDown(int iKey) const
{
    return m_pKey_Manager->IsKeyDown(iKey);
}
bool CGameInstance::IsKeyUp(int iKey) const
{
    return m_pKey_Manager->IsKeyUp(iKey);
}
bool CGameInstance::IsKeyHold(int iKey) const
{
    return m_pKey_Manager->IsKeyHold(iKey);
}
float CGameInstance::GetKeyHoldTime(int iKey) const
{
    return m_pKey_Manager->GetKeyHoldTime(iKey);
}
#pragma endregion

#pragma region NETWORK_MANAGER
TEST* CGameInstance::Ping()
{
    return m_pNetwork_Manager->Ping();
}
list<USER*> CGameInstance::Get_AllUsers()
{
    return m_pNetwork_Manager->Get_AllUsers();
}
#pragma endregion

#pragma region PICKING
void CGameInstance::Transform_Picking_ToLocalSpace(const _float4x4& WorldMatrixInverse)
{
    m_pPicking->Transform_ToLocalSpace(WorldMatrixInverse);
}
_bool CGameInstance::Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
    return m_pPicking->Picking_InWorld(vPickedPos, vPointA, vPointB, vPointC);
}
_bool CGameInstance::Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
    return m_pPicking->Picking_InLocal(vPickedPos, vPointA, vPointB, vPointC);
}
#pragma endregion

#pragma region COLLISION_MANAGER
HRESULT CGameInstance::Add_Collider(class CCollider* pCollider)
{
    return m_pCollision_Manager->Add_Collider(pCollider);
}
#pragma endregion

#pragma region ROOM_MANAGER
HRESULT CGameInstance::Add_Room(class CRoom* pRoom)
{
    return m_pRoom_Manager->Add_Room(pRoom);
}
HRESULT CGameInstance::Enter_Room(_int iRoomID)
{
    return m_pRoom_Manager->Enter_Room(iRoomID);
}
CRoom* CGameInstance::Get_CurrentRoom()
{
    return m_pRoom_Manager->Get_CurrentRoom();
}
CRoom* CGameInstance::Get_RoomByID(_int iRoomID)
{
    return m_pRoom_Manager->Get_RoomByID(iRoomID);
}

#pragma endregion

#pragma region FONT_MANAGER
HRESULT CGameInstance::Ready_Font(const _wstring& strFontTag, 
    const _wstring& strFontPath, 
    const _wstring& strFontName, 
    const _uint& iWidth, 
    const _uint& iHeight, 
    const _uint& iWeight)
{
    return m_pFont_Manager->Ready_Font(strFontTag, strFontPath, strFontName, iWidth, iHeight, iWeight);
}
void CGameInstance::Render_Font(const wstring& strFontTag, 
    const _wstring& strText, 
    const _float2* pVec2Pos, 
    D3DXCOLOR d3dxColor, 
    DWORD dwFormat)
{
    m_pFont_Manager->Render_Font(strFontTag, strText, pVec2Pos, d3dxColor, dwFormat);
}
#pragma endregion

HRESULT CGameInstance::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
    return m_pLight_Manager->Ready_Light(pLightInfo, iIndex);
}

void CGameInstance::Release_Engine()
{
    Release();
    
    Safe_Release(m_pTimer_Manager);
    Safe_Release(m_pLevel_Manager);
    Safe_Release(m_pGraphic_Device);
    Safe_Release(m_pPrototype_Manager);
    Safe_Release(m_pObject_Manager);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pKey_Manager);
    Safe_Release(m_pNetwork_Manager);
    Safe_Release(m_pPicking);
    Safe_Release(m_pCollision_Manager);
    Safe_Release(m_pRoom_Manager);
    Safe_Release(m_pFont_Manager);
    Safe_Release(m_pLight_Manager);
}

void CGameInstance::Free()
{
    __super::Free();
}
