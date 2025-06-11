#pragma once
#include "GameObject.h"
#include "GameInstance.h"
#include "TerrainBox.h"
#include "Field_Hp.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider_OBB;
END

BEGIN(Client)

class CMonster : public CGameObject
{
public:
	typedef struct tagMonsterDesc {
		_uint iLayerLevelIndex;
		_wstring strLayerTag;
		_uint iPrototypeLevelIndex;
		_wstring strPrototypeTag;
		_float3 vPosition;
		CTerrainBox* pTerrainBox;
		MONSTER_TYPE_A eType;
	}MONSTERDESC;
protected:
	CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Components(void* pArg);
	void SetUp_RenderState();
	void Reset_RenderState();
	void Ready_ExpBall();
	void Ready_Gold();

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

public:
	class CTransform* Get_Transform() { return m_pTransformCom; }

public:
	void Set_Damage(_float fDamage);
	HRESULT Render_Font(_int iDamage);
public:
	_bool Get_IsHit() { return m_bIsHit; }
	void Set_IsHit(_bool bHit) { m_bIsHit = bHit; }
	const MONSTER_TYPE Get_MonsterType() { return m_eMonsterType; }

	_int Get_HP() { return m_iCulHp; }
	void Set_HP(_int iHp) { m_iCulHp = iHp; }
	void Fill_HP() { m_iCulHp = m_iMaxHp; }

protected:
	class CTransform*		m_pTransformCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };
	class CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };
	CTerrainBox*			m_pTerrainBox = { nullptr };
	_bool					m_isTracking = false;
	_bool					m_bIsHit = false;
	_bool					m_isSummoned = false;
	MONSTER_TYPE			m_eMonsterType = {};
	DWORD					m_dwHitTime = {};

	CField_Hp* m_pHpBar		= { nullptr };

	// Ω∫≈» ¡§∫∏
	_int		m_iCulHp = {};
	_int		m_iMaxHp = {};

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END