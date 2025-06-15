#pragma once

#include "Prototype_Manager.h"
BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

#pragma region ENGINE
public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, LPDIRECT3DDEVICE9* ppOut);
	void Update_Engine(_float fTimeDelta);
	HRESULT Clear_Resources(_uint iClearLevelID);

public:
	void Render_Begin(D3DXCOLOR Color);
	HRESULT Draw();
	void Render_End(HWND hWnd = 0);

public:
	_float Compute_Random_Normal();
	_float Compute_Random(_float fMin, _float fMax);
	void Seed_Random();

public:
	_float Rand_Normal();
	_float Rand(_float fMin, _float fMax);
#pragma endregion

#pragma region LEVEL_MANAGER
public:
	HRESULT Open_Level(_uint iLevelID, class CLevel* pNewLevel);
	_uint Get_CurrentLevel();
#pragma endregion

#pragma region PROTOTYPE_MANAGER
public:
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase* Clone_Prototype(PROTOTYPE ePrototype, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region OBJECT_MANAGER
public:
	HRESULT Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring strPrototypeTag, void* pArg = nullptr);
	CComponent* Get_Component(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	class CGameObject* Get_GameObject(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iIndex = 0);
	class CGameObject* Get_LastGameObject(_uint iLayerLevelIndex, const _wstring& strLayerTag);
	void Remove_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, class CGameObject* pGameObject);
	HRESULT Add_ItemObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint ItemIndex, void* pArg = nullptr);
	class CLayer* Find_Layer(_uint iLayerLevelIndex, const _wstring& strLayerTag);
	HRESULT Add_Direct_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, class CGameObject* pGameObject);
#pragma endregion

#pragma region RENDERER
public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	void Renderer_Clear();
#pragma endregion

#pragma region TIMER_MANAGER
public:
	_float	Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT	Add_Timer(const _wstring& strTimerTag);
	void	Compute_TimeDelta(const _wstring& strTimerTag);
	void	Remove_Timer(const _wstring& strTimerTag);
#pragma endregion

#pragma region KEY_MANAGER
	void AddTrackingKey(int iKey);
	bool IsKeyDown(int iKey) const;   // 이번 프레임에 눌림
	bool IsKeyUp(int iKey) const;     // 이번 프레임에 떼짐
	bool IsKeyHold(int iKey) const;   // 계속 눌림
	float GetKeyHoldTime(int iKey) const; // 눌린 시간 (선택사항)
#pragma endregion

#pragma region NETWORK_MANAGER
	//TEST* Ping();
	//list<USER*> Get_AllUsers();
#pragma endregion

#pragma region PICKING
	void Transform_Picking_ToLocalSpace(const _float4x4& WorldMatrixInverse);
	_bool Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Get_IntersectAtY(_float targetY, _float3& intersectPos);
#pragma endregion

#pragma region COLLISION_MANAGER
	// 해당 구간 추후 좀더 추가 예정
	HRESULT Add_Collider(class CCollider_OBB* pCollider);
	void Check_RoomCollisions();
	void Clear_Colliders();
	void Clear_AllColliders();
	void Set_Next(_bool bNext);
	void Remove_Collider_ByOwner(CGameObject* pOwner);
#pragma endregion

#pragma region FONT_MANAGER
	HRESULT Ready_Font(const _wstring& strFontTag,
		const _wstring& strFontPath,
		const _wstring& strFontName,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

	void Render_Font(const wstring& strFontTag,
		const _wstring& strText,
		const RECT& TexRect,
		D3DXCOLOR d3dxColor,
		DWORD dwFormat = DT_NOCLIP);

	//사용법
	/*CFont_Manager::GetInstance()->Ready_Font(
		pGraphicDev,
		L"TitleFont",
		L"./Fonts/NanumSquare.ttf",
		L"NanumSquare",
		0, 28, FW_BOLD
	);

	CFont_Manager::GetInstance()->Render_Font(
		L"TitleFont",
		L"스테이지 클리어!",
		&_float2{ 400.f, 100.f },
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f),
		DT_CENTER | DT_TOP
	);*/
#pragma endregion
	
#pragma region LIGHT_MANAGER
	void Add_Light(const _wstring& strID, const LIGHTDATA& data);
	void Remove_Light(const _wstring& strID);
	const unordered_map<_wstring, LIGHTDATA>& Get_Lights() const;
	const LIGHTDATA* Get_Light(const _wstring& strID) const;
	void Update_LightDirection(const _wstring& strID, const D3DXVECTOR3& vDir);
	void Update_LightPosition(const _wstring& strID, const D3DXVECTOR3& vPos);
	void Apply_ToShader(CShader* pShader, const vector<_wstring>& vecKeys);
#pragma endregion

#pragma region ANIMATION_MANAGER
	HRESULT Insert_Animation(const wstring& strAnimTag, CAnimation* anim);
	CAnimation* Find_Animation(const wstring& strAnimTag);
#pragma endregion


#pragma region ITEM_MANAGER
	HRESULT						Setting_Item(void* pArg, _uint iMaxItemIndex, _uint iLevelIndex, const _wstring& strItemBaseTag);
	CBase*						find_ItemObject(_uint iIndex);
	CItemObject*				Get_ItemObject(_uint iIndex, _bool isInven);
	class CItemObject*			Pop_Item();
	class CButton*				Pop_Slot();
	const _uint					Pop_ISlot_Type();
	_uint						Pop_Item_Count();
	void						Pick_ItemSlot(class CItemObject* pPickItem, class CButton* pSlot, _uint iItemCount, _uint iSlottype);
	void						Pick_Reset();
	const vector<_int>			AcquiredItem_List();
	void						Item_CulCool(_wstring szEffectTag, _float fCulcool);
	void						Item_MaxCool(_wstring szEffectTag, _float fMaxcool);
	const _float				Get_ItemCool(bool bMaxCool, _wstring szEffectTag);
	void						AcquiredItem_List_Add(_int iIndex);
#pragma endregion

#pragma region EVENT_MANAGER
	void Subscribe(_uint iTypeIndex, class IEventListener* pListener);
	void Unsubscribe(_uint iTypeIndex, class IEventListener* pListener);
	void Broadcast(_uint iTypeIndex, const EVENTDATA* pData);
#pragma endregion

#pragma region UIOBJECT_MANAGER
	HRESULT				Add_UIObject(_uint iLevelIndex, const _wstring& strUITag, class CUIObject* pUIObj);
	void				Update_On(_uint iLevelIndex, const _wstring& strUITag);
	void				Update_Off(_uint iLevelIndex, const _wstring& strUITag);
	void				All_Update_On();
	void				All_Update_Off();			
	void				Clear_UiObj(_uint iLevelIndex);
	class CUIObject*	Find_UIObj(_uint iLevelIndex, const _wstring& strUITag);

#pragma endregion

#pragma region SOUND_MANAGER

	void PlaySoundW(const TCHAR* pSoundKey, _uint SoundChannel, float fVolume);
	void PlayLoopSound(const TCHAR* pSoundKey, _uint SoundChannel, float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void StopSound(_uint SoundChannel);
	void StopAll();
	void SetChannelVolume(_uint SoundChannel, float fVolume);
	bool IsPlaying(_uint SoundChannel);
#pragma endregion

#pragma region PARTICLE_MANAGER
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta, _uint iParticleType);
	void Late_Update(_float fTimeDelta, _uint iParticleType);
	HRESULT Create_Particle_Fast(_uint iParticleType, _uint iLayerLevelIndex, const _wstring& strLayerTag, _int iOffSetType, _float3 vScale, _bool bUseOrtho = false);
	HRESULT Create_Particle_Low(_uint iParticleType, _uint iLayerLevelIndex, const _wstring& strLayerTag, _int iOffSetType, _bool bUseOrtho = false);
	HRESULT Play(_uint iParticleType, _float3 vPos);
	void Clear();
#pragma endregion

private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CLevel_Manager*		m_pLevel_Manager = { nullptr };
	class CObject_Manager*		m_pObject_Manager = { nullptr };
	class CPrototype_Manager*	m_pPrototype_Manager = { nullptr };
	class CRenderer*			m_pRenderer = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };
	class CKey_Manager*			m_pKey_Manager = { nullptr };
	//class CNetwork_Manager*		m_pNetwork_Manager = { nullptr };
	class CPicking*				m_pPicking = { nullptr };
	class CCollision_Manager*	m_pCollision_Manager = { nullptr };
	class CFont_Manager*		m_pFont_Manager = { nullptr };
	class CLight_Manager*		m_pLight_Manager = { nullptr };
	class CAnim_Manager*		m_pAnimation_Manager = { nullptr };
	class CItem_Manager*		m_pItem_Manager = { nullptr };
	class CEvent_Manager*		m_pEvent_Manager = { nullptr };
	class CUIObject_Manager*	m_pUIObject_Manager = { nullptr };
	class CSound_Manager*		m_pSound_Manager = { nullptr };
	class CParticle_Manager*	m_pParticle_Manager = { nullptr };

public:
	void Release_Engine();
	virtual void Free() override;
};

END

