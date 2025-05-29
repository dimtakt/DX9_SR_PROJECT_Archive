#pragma once
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)

class ENGINE_DLL CMonster abstract : public CGameObject
{
public:
	typedef struct tagMonsterDesc {
		_uint iLayerLevelIndex;
		_wstring strLayerTag;
		_uint iPrototypeLevelIndex;
		const _wstring strPrototypeTag;
		_uint iCount;
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

protected:
	class CTransform* m_pTransformCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END