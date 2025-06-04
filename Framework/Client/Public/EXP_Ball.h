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

class CEXP_Ball final : public CGameObject
{
public:
	typedef struct tagEXPBallDesc
	{
		_float	fValue;
		_float3	vPosition;
	}EXPBALLDESC;
private:
	CEXP_Ball(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEXP_Ball(const CEXP_Ball& Prototype);
	virtual ~CEXP_Ball() = default;

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
	_float Get_EXP() { return m_fEXPValue; }

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CAnimator* m_pAnimatorCom = { nullptr };

	_float		m_fEXPValue = {};
private:
	HRESULT Ready_Components(void* pArg);

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

public:
	static CEXP_Ball* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END