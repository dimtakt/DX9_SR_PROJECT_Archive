#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CAnimator;
class CTransform;
END

BEGIN(Client)

class CGoldLeaf final : public CGameObject
{
public:
	typedef struct tagGoldDesc
	{
		_float	fValue;
		_float3	vPosition;
	}GOLDLEAFDESC;
private:
	CGoldLeaf(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGoldLeaf(const CGoldLeaf& Prototype);
	virtual ~CGoldLeaf() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void SetUp_RenderState();
	void Reset_RenderState();

public:
	_float Get_Gold() { return m_fGoldValue; }

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_float		m_fGoldValue = {};
private:
	HRESULT Ready_Components(void* pArg);

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

public:
	static CGoldLeaf* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END