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

class CProjSword final : public CGameObject
{
public:
	enum class SWORDDIR {LEFT, RIGHT};
public:
	typedef struct tagProjSwordDesc
	{
		CTransform* pTargetTransform;
	}PROJSWORDDESC;
private:
	CProjSword(LPDIRECT3DDEVICE9 pGraphic_Device);
	CProjSword(const CProjSword& Prototype);
	virtual ~CProjSword() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void SetUp_RenderState();
	void Reset_RenderState();

private:
	CVIBuffer_Rect* m_pVIBufferCom_0 = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom_1 = { nullptr };
	CTransform* m_pTransformCom_0 = { nullptr };
	CTransform* m_pTransformCom_1 = { nullptr };
	CTexture* m_pTextureCom_0 = { nullptr };
	CTexture* m_pTextureCom_1 = { nullptr };
	CAnimator* m_pAnimatorCom_0 = { nullptr };
	CAnimator* m_pAnimatorCom_1 = { nullptr };
	CTransform* m_pTargetTransform = { nullptr };

private:
	HRESULT Ready_Components(void* pArg);

private:
	void Init_ItemRot(CTransform* pTransform, SWORDDIR eDir);
	HRESULT Follow_Player_Side(CTransform* pTransform, _float fDeltaTime, SWORDDIR eDir);

public:
	static CProjSword* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END