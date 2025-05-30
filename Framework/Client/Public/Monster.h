#pragma once
#include "GameObject.h"
#include "GameInstance.h"
#include "Client_Defines.h"
#include "TerrainBox.h"

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
	HRESULT Ready_Components();
	void SetUp_RenderState();
	void Reset_RenderState();

public:
	CCollider_OBB* Get_Collider() { return m_pCollider; }
	virtual void OnCollision(CGameObject* pGameObject) override;

protected:
	class CTransform* m_pTransformCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTerrainBox* m_pTerrainBox = { nullptr };
	CCollider_OBB* m_pCollider = { nullptr };

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END