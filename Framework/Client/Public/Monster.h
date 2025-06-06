#pragma once
#include "GameObject.h"
#include "GameInstance.h"
#include "Client_Defines.h"
#include "TerrainBox.h"
#include "Field_Hp.h"

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

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

public:
	class CTransform* Get_Transform() { return m_pTransformCom; }

public:
	void Set_Damage(_float fDamage) { m_iCulHp += fDamage; }

protected:
	class CTransform* m_pTransformCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTerrainBox* m_pTerrainBox = { nullptr };
	_bool m_isTracking = false;
	_bool					m_bIsHit = false;
	DWORD					m_dwHitTime = {};

	CField_Hp* m_pHpBar = { nullptr };

	// Ω∫≈» ¡§∫∏
	_int		m_iCulHp = {};
	_int		m_iMaxHp = {};

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END