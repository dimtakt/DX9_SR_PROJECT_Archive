#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "TerrainBox.h"
#include "EventListener.h"
#include "Field_Hp.h"
#include "Field_Npc_Chat.h"
BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CPlayerStats;
class CAnimator;
class CCollider_OBB;
END

BEGIN(Client)

class CPlayer final : public CGameObject, public IEventListener
{
public:
	typedef struct tagPlayerDesc
	{
		_uint iLayerIndex;
	}PLAYERDESC;
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_TerrainBox(CTerrainBox* pTerrainBox, _int iIndex);
	

public:
	virtual void OnCollision(CGameObject* pGameObject) override;
	void ChangeStat(STAT_INFO eStat, float fValue);
	virtual void OnEvent(_uint iTypeindex, const EVENTDATA* pData);

public:
	void Hit(_int iDamage);
	_bool Get_IsHit() { return m_bIsHit; }

private:
	CVIBuffer_Rect*			m_pVIBufferCom					= { nullptr };
	CTransform*				m_pTransformCom					= { nullptr };

	CTexture*				m_pTextureCom_Roll				= { nullptr };
	CTexture*				m_pTextureCom_Air				= { nullptr };
	CTexture*				m_pTextureCom_Down				= { nullptr };
	CTexture*				m_pTextureCom_Idle_Lower		= { nullptr };
	CTexture*				m_pTextureCom_Idle_Upper		= { nullptr };
	CTexture*				m_pTextureCom_Move_Lower		= { nullptr };
	CTexture*				m_pTextureCom_Move_Upper		= { nullptr };
	CTexture*				m_pTextureCom_Attack_Lower		= { nullptr };
	CTexture*				m_pTextureCom_Attack_Upper		= { nullptr };
	CTexture*				m_pTextureCom_GreatSwordHeavyAttack_Lower	= { nullptr };
	CTexture*				m_pTextureCom_GreatSwordHeavyAttack_Upper	= { nullptr };
	CTexture*				m_pTextureCom_WhirlWind_Ready	= { nullptr };
	CTexture*				m_pTextureCom_WhirlWind_Cycle	= { nullptr };

	CPlayerStats*			m_pPlayerStatsCom				= { nullptr };
	CAnimator*				m_pAnimatorCom					= { nullptr };
	CAnimator*				m_pAnimatorTransCom				= { nullptr };

	_bool					m_isFlippedX					= false;

	CTerrainBox*			m_pTerrainBox					= { nullptr };
	//CCollider_OBB*			m_pCollider						= { nullptr };
	
	_wstring				m_strTimerTag					= {};

	_float					m_fStackedTime					= 0;

	_float3					m_vDashDir						= {};
	_float3					m_vCursorDir					= {};

	_bool					m_isReadyFury					= false;
	_float					m_fGodModeTime					= {};

	//CField_Hp*				m_pHpBar						= { nullptr };
	CField_Npc_Chat*		m_pChat							= { nullptr };
	_bool					m_bIsHit						= { false };
	_bool					m_bIsStun						= false;
	DWORD					m_dwHitTime						= {};
private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_Object();					//필드 HP바 테스트용
	HRESULT Ready_Item(void* pArg);
	HRESULT Ready_Skill(void* pArg);
	void SetUp_RenderState();
	void Reset_RenderState();
	HRESULT Render_Font(_int iDamage);					//필드 폰트 테스트용
	HRESULT Render_Font_Parry();

public:
	void Ready_Parry();
	_bool Get_IsGodMode();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
