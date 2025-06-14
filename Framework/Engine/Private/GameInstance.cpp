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
#include "Event_Manager.h"
#include "UIObject_Manager.h"
#include "Sound_Manager.h"
#include "Particle_Manager.h"
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

    m_pLight_Manager = CLight_Manager::Create();
    if (nullptr == m_pLight_Manager)
        return E_FAIL;

    m_pAnimation_Manager = CAnim_Manager::Create();
    if (nullptr == m_pAnimation_Manager)
        return E_FAIL;
    
    m_pItem_Manager = CItem_Manager::Create(*ppOut);
    if (nullptr == m_pItem_Manager)
        return E_FAIL;

    m_pEvent_Manager = CEvent_Manager::Create();
    if (nullptr == m_pEvent_Manager)
        return E_FAIL;

    m_pUIObject_Manager = CUIObject_Manager::Create(EngineDesc.iNumLevels);
    if (nullptr == m_pUIObject_Manager)
        return E_FAIL;

    m_pSound_Manager = CSound_Manager::Create();
    if (nullptr == m_pSound_Manager)
        return E_FAIL;

    m_pParticle_Manager = CParticle_Manager::Create();
    if (nullptr == m_pUIObject_Manager)
        return E_FAIL;

    return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
    m_pKey_Manager->Update(fTimeDelta);
    
    m_pObject_Manager->Priority_Update(fTimeDelta);
    m_pPicking->Update();
    m_pObject_Manager->Update(fTimeDelta);
    m_pItem_Manager->Update();
    m_pObject_Manager->Late_Update(fTimeDelta);
    m_pLevel_Manager->Update(fTimeDelta);

    // 콜리전 동기화
    m_pCollision_Manager->Update();
    // 콜리전 충돌확인
    m_pCollision_Manager->Check_RoomCollisions();
    if (IsKeyDown('B'))
        m_pCollision_Manager->Set_IsRender();
    
}

HRESULT CGameInstance::Clear_Resources(_uint iClearLevelID)
{
    m_pRenderer->Clear();

    m_pCollision_Manager->Clear_Colliders();

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

    // 충돌체 시각화
    if(m_pCollision_Manager->Get_IsRender())
        m_pCollision_Manager->Render();

    if (FAILED(m_pLevel_Manager->Render()))
        return E_FAIL;

    return S_OK;
}

void CGameInstance::Render_End(HWND hWnd)
{
    if (nullptr != m_pGraphic_Device)
        m_pGraphic_Device->Render_End();
}
void CGameInstance::Seed_Random()
{
    srand(static_cast<unsigned int>(time(NULL)));
}

_float CGameInstance::Rand_Normal()
{
    return static_cast<_float>(rand()) / RAND_MAX;
}

_float CGameInstance::Rand(_float fMin, _float fMax)
{
    return fMin + Rand_Normal() * (fMax - fMin);
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
_uint CGameInstance::Get_CurrentLevel()
{
    return m_pLevel_Manager->Get_CurrentLevel();
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
void CGameInstance::Renderer_Clear()
{
    return m_pRenderer->Clear();
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
void CGameInstance::Remove_Timer(const _wstring& strTimerTag)
{
	m_pTimer_Manager->Remove_Timer(strTimerTag);
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

void CGameInstance::Check_RoomCollisions()
{
    m_pCollision_Manager->Check_RoomCollisions();
}

void CGameInstance::Clear_Colliders()
{
    m_pCollision_Manager->Clear_Colliders();
}
void CGameInstance::Clear_AllColliders()
{
    m_pCollision_Manager->Clear_AllColliders();
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

#pragma region LIGHT_MANAGEr

void CGameInstance::Add_Light(const _wstring& strID, const LIGHTDATA& data)
{
    return m_pLight_Manager->Add_Light(strID, data);
}
void CGameInstance::Remove_Light(const _wstring& strID)
{
    return Remove_Light(strID);
}
const unordered_map<_wstring, LIGHTDATA>& CGameInstance::Get_Lights() const
{
    return m_pLight_Manager->Get_Lights();
}
const LIGHTDATA* CGameInstance::Get_Light(const _wstring& strID) const
{
    return m_pLight_Manager->Get_Light(strID);
}
void CGameInstance::Update_LightDirection(const _wstring& strID, const D3DXVECTOR3& vDir)
{
    return m_pLight_Manager->Update_LightDirection(strID, vDir);
}
void CGameInstance::Update_LightPosition(const _wstring& strID, const D3DXVECTOR3& vPos)
{
    return m_pLight_Manager->Update_LightPosition(strID, vPos);
}
void CGameInstance::Apply_ToShader(CShader* pShader, const vector<_wstring>& vecKeys)
{
    return m_pLight_Manager->Apply_ToShader(pShader, vecKeys);
}
#pragma endregion


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

#pragma region ITEM_MANAGER
HRESULT CGameInstance::Setting_Item(void* pArg, _uint iMaxItemIndex, _uint iLevelIndex, const _wstring& strItemBaseTag)
{
    return m_pItem_Manager->Setting_Item(pArg, iMaxItemIndex, iLevelIndex, strItemBaseTag);
}

CBase* CGameInstance::find_ItemObject(_uint iIndex)
{
    return m_pItem_Manager->find_ItemObject(iIndex);
}

CItemObject* CGameInstance::Get_ItemObject(_uint iIndex, _bool isInven)
{
    return m_pItem_Manager->Get_ItemObject(iIndex, isInven);
}
CItemObject* CGameInstance::Pop_Item()
{
    return m_pItem_Manager->Pop_Item();
}
CButton* CGameInstance::Pop_Slot()
{
    return m_pItem_Manager->Pop_Slot();
}
const _uint CGameInstance::Pop_ISlot_Type()
{
    return m_pItem_Manager->Pop_ISlot_Type();
}
_uint CGameInstance::Pop_Item_Count()
{
    return m_pItem_Manager->Pop_Item_Count();
}
void CGameInstance::Pick_ItemSlot(CItemObject* pPickItem, CButton* pSlot, _uint iItemCount, _uint iSlottype)
{
    m_pItem_Manager->Pick_ItemSlot(pPickItem, pSlot, iItemCount, iSlottype);
}
void CGameInstance::Pick_Reset()
{
    m_pItem_Manager->Pick_Reset();
}
const vector<_int> CGameInstance::AcquiredItem_List()
{
    return m_pItem_Manager->AcquiredItem_List();
}
void CGameInstance::Item_CulCool(_wstring szEffectTag, _float fCulcool)
{
    m_pItem_Manager->Item_CulCool(szEffectTag, fCulcool);
}
void CGameInstance::Item_MaxCool(_wstring szEffectTag, _float fMaxcool)
{
    m_pItem_Manager->Item_MaxCool(szEffectTag, fMaxcool);
}
const _float CGameInstance::Get_ItemCool(bool bMaxCool, _wstring szEffectTag)
{
    return m_pItem_Manager->Get_ItemCool(bMaxCool, szEffectTag);
}
#pragma endregion

#pragma region EVENT_MANAGER
void CGameInstance::Subscribe(_uint iTypeIndex, class IEventListener* pListener)
{
    m_pEvent_Manager->Subscribe(iTypeIndex, pListener);
}

void CGameInstance::Unsubscribe(_uint iTypeIndex, class IEventListener* pListener)
{
    m_pEvent_Manager->Unsubscribe(iTypeIndex, pListener);
}

void CGameInstance::Broadcast(_uint iTypeIndex, const EVENTDATA* pData)
{
    m_pEvent_Manager->Broadcast(iTypeIndex, pData);
}
#pragma endregion

#pragma region UI_MANAGER
HRESULT CGameInstance::Add_UIObject(_uint iLevelIndex, const _wstring& strUITag, CUIObject* pUIObj)
{
    return m_pUIObject_Manager->Add_UIObject(iLevelIndex, strUITag, pUIObj);
}
void CGameInstance::Update_On(_uint iLevelIndex, const _wstring& strUITag)
{
    m_pUIObject_Manager->Update_On(iLevelIndex, strUITag);
}
void CGameInstance::Update_Off(_uint iLevelIndex, const _wstring& strUITag)
{
    m_pUIObject_Manager->Update_Off(iLevelIndex, strUITag);
}
void CGameInstance::All_Update_On()
{
    m_pUIObject_Manager->All_Update_On();
}
void CGameInstance::All_Update_Off()
{
    m_pUIObject_Manager->All_Update_Off();
}
void CGameInstance::Clear_UiObj(_uint iLevelIndex)
{
    m_pUIObject_Manager->Clear_UiObj(iLevelIndex);
}
CUIObject* CGameInstance::Find_UIObj(_uint iLevelIndex, const _wstring& strUITag)
{
    return m_pUIObject_Manager->Find_UIObj(iLevelIndex, strUITag);
}

#pragma endregion


#pragma region PARTICLE_MAGAGER
void CGameInstance::Priority_Update(_float fTimeDelta)
{
}
void CGameInstance::Update(_float fTimeDelta, _uint iParticleType)
{
    m_pParticle_Manager->Update(fTimeDelta, iParticleType);
}
void CGameInstance::Late_Update(_float fTimeDelta, _uint iParticleType)
{
    m_pParticle_Manager->Late_Update(fTimeDelta, iParticleType);
}

HRESULT CGameInstance::Create_Particle_Fast(_uint iParticleType, _uint iLayerLevelIndex, const _wstring& strLayerTag, _int iOffSetType, _float3 vScale, _bool bUseOrtho)
{
    m_pParticle_Manager->Create_Particle_Fast(iParticleType, iLayerLevelIndex, strLayerTag, iOffSetType, vScale, bUseOrtho);

    return S_OK;    
}

HRESULT CGameInstance::Create_Particle_Low(_uint iParticleType, _uint iLayerLevelIndex, const _wstring& strLayerTag, _int iOffSetType, _bool bUseOrtho)
{
    m_pParticle_Manager->Create_Particle_Low(iParticleType, iLayerLevelIndex, strLayerTag, iOffSetType, bUseOrtho);

    return S_OK;
}

HRESULT CGameInstance::Play(_uint iParticleType, _float3 vPos)
{
    m_pParticle_Manager->Play(iParticleType, vPos);

    return S_OK;
}
void CGameInstance::Clear()
{
    m_pParticle_Manager->Clear();
}
#pragma endregion

#pragma region SOUND_MANAGER
void CGameInstance::PlaySoundW(const TCHAR* pSoundKey, _uint SoundChannel, float fVolume)
{
    m_pSound_Manager->PlaySoundW(pSoundKey, SoundChannel, fVolume);
}
void CGameInstance::PlayLoopSound(const TCHAR* pSoundKey, _uint SoundChannel, float fVolume)
{
    m_pSound_Manager->PlayLoopSound(pSoundKey, SoundChannel, fVolume);
}
void CGameInstance::PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
    m_pSound_Manager->PlayBGM(pSoundKey, fVolume);
}
void CGameInstance::StopSound(_uint SoundChannel)
{
    m_pSound_Manager->StopSound(SoundChannel);
}
void CGameInstance::StopAll()
{
    m_pSound_Manager->StopAll();
}
void CGameInstance::SetChannelVolume(_uint SoundChannel, float fVolume)
{
    m_pSound_Manager->SetChannelVolume(SoundChannel, fVolume);
}
bool CGameInstance::IsPlaying(_uint SoundChannel)
{
    return m_pSound_Manager->IsPlaying(SoundChannel);
}
#pragma endregion

void CGameInstance::Release_Engine()
{
    Safe_Release(m_pSound_Manager);
    Safe_Release(m_pParticle_Manager);

    Release();

    Safe_Release(m_pCollision_Manager);
    Safe_Release(m_pUIObject_Manager);
    Safe_Release(m_pTimer_Manager);
    Safe_Release(m_pLevel_Manager);
    Safe_Release(m_pGraphic_Device);
    Safe_Release(m_pPrototype_Manager);
    Safe_Release(m_pObject_Manager);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pEvent_Manager);
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
    __super::Free();
}