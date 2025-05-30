//#include "Network_Manager.h"
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
#include "Font_Manager.h"
#include "Light_Manager.h"
#include "GameObject.h"
#include "Anim_Manager.h"
#include "Item_Manager.h"

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

    //m_pNetwork_Manager = CNetwork_Manager::Create();
    //if (nullptr == m_pNetwork_Manager)
    //    return E_FAIL;

    m_pPicking = CPicking::Create(*ppOut, EngineDesc.hWnd, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY);
    if (nullptr == m_pPicking)
        return E_FAIL;

    m_pCollision_Manager = CCollision_Manager::Create();
    if (nullptr == m_pCollision_Manager)
        return E_FAIL;

    m_pFont_Manager = CFont_Manager::Create(*ppOut);
    if (nullptr == m_pFont_Manager)
        return E_FAIL;

    m_pLight_Manager = CLight_Manager::Create(*ppOut);
    if (nullptr == m_pLight_Manager)
        return E_FAIL;

    m_pAnimation_Manager = CAnim_Manager::Create();
    if (nullptr == m_pAnimation_Manager)
        return E_FAIL;
    
    m_pItem_Manager = CItem_Manager::Create(*ppOut);
    if (nullptr == m_pItem_Manager)
        return E_FAIL;

    return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
    m_pKey_Manager->Update(fTimeDelta);
    m_pObject_Manager->Priority_Update(fTimeDelta);

    m_pPicking->Update();
    m_pCollision_Manager->Check_RoomCollisions();
    m_pObject_Manager->Update(fTimeDelta);
    m_pObject_Manager->Late_Update(fTimeDelta);

    m_pLevel_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Clear_Resources(_uint iClearLevelID)
{
    m_pPrototype_Manager->Clear(iClearLevelID);

    m_pObject_Manager->Clear(iClearLevelID);

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

CGameObject* CGameInstance::Get_GameObject(_uint iLayerLevelIndex, const _wstring& strLayerTag,_uint iIndex)
{
    return m_pObject_Manager->Get_GameObject(iLayerLevelIndex, strLayerTag, iIndex);
}

CGameObject* CGameInstance::Get_LastGameObject(_uint iLayerLevelIndex, const _wstring& strLayerTag)
{
    return m_pObject_Manager->Get_LastGameObject(iLayerLevelIndex, strLayerTag);
}
void CGameInstance::Remove_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, class CGameObject* pGameObject)
{
    m_pObject_Manager->Remove_GameObject_ToLayer(iLayerLevelIndex, strLayerTag, pGameObject);
}

HRESULT CGameInstance::Add_ItemObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint ItemIndex, void* pArg)
{
    return m_pObject_Manager->Add_ItemObject_ToLayer(iLayerLevelIndex, strLayerTag, ItemIndex, pArg);
}

CLayer* CGameInstance::Find_Layer(_uint iLayerLevelIndex, const _wstring& strLayerTag)
{
    return m_pObject_Manager->Find_Layer(iLayerLevelIndex, strLayerTag);
}

HRESULT CGameInstance::Add_Direct_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, CGameObject* pGameObject)
{
    return m_pObject_Manager->Add_Direct_GameObject_ToLayer(iLayerLevelIndex, strLayerTag, pGameObject);
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
//TEST* CGameInstance::Ping()
//{
//    return m_pNetwork_Manager->Ping();
//}
//list<USER*> CGameInstance::Get_AllUsers()
//{
//    return m_pNetwork_Manager->Get_AllUsers();
//}
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
_bool CGameInstance::Get_IntersectAtY(_float targetY, _float3& intersectPos)
{
    return m_pPicking->Get_IntersectAtY(targetY, intersectPos);
}
#pragma endregion

#pragma region COLLISION_MANAGER
HRESULT CGameInstance::Add_Collider(class CCollider_OBB* pCollider)
{
    return m_pCollision_Manager->Add_OBB_Collider(pCollider);
}

void CGameInstance::Clear_Colliders()
{
    m_pCollision_Manager->Clear_Colliders();
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
    const RECT& TexRect,
    D3DXCOLOR d3dxColor, 
    DWORD dwFormat)
{
    m_pFont_Manager->Render_Font(strFontTag, strText, TexRect, d3dxColor, dwFormat);
}
#pragma endregion

HRESULT CGameInstance::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
    return m_pLight_Manager->Ready_Light(pLightInfo, iIndex);
}


#pragma region ANIMATION_MANAGER
HRESULT CGameInstance::Insert_Animation(const wstring& strAnimTag, CAnimation* anim)
{
    return m_pAnimation_Manager->Insert_Animation(strAnimTag, anim);
}

CAnimation* CGameInstance::Find_Animation(const wstring& strAnimTag)
{
    return m_pAnimation_Manager->Find_Animation(strAnimTag);
#pragma endregion
}
HRESULT CGameInstance::Setting_Item(void* pArg, _uint iMaxItemIndex, _uint iLevelIndex, const _wstring& strItemBaseTag)
{
    return m_pItem_Manager->Setting_Item(pArg, iMaxItemIndex, iLevelIndex, strItemBaseTag);
}

CBase* CGameInstance::find_ItemObject(_uint iIndex)
{
    return m_pItem_Manager->find_ItemObject(iIndex);
}

CItemObject* CGameInstance::Get_ItemObject(_uint iIndex)
{
    return m_pItem_Manager->Get_ItemObject(iIndex);
}

void CGameInstance::Release_Engine()
{
    Release();
    Safe_Release(m_pCollision_Manager);
    Safe_Release(m_pTimer_Manager);
    Safe_Release(m_pLevel_Manager);
    Safe_Release(m_pGraphic_Device);
    Safe_Release(m_pPrototype_Manager);
    Safe_Release(m_pObject_Manager);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pKey_Manager);
    //Safe_Release(m_pNetwork_Manager);
    Safe_Release(m_pPicking);
    Safe_Release(m_pFont_Manager);
    Safe_Release(m_pLight_Manager);
    Safe_Release(m_pAnimation_Manager);
    Safe_Release(m_pItem_Manager);
}

void CGameInstance::Free()
{
    __super::Free();//
}