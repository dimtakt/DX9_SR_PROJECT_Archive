#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
class CTexture;
class CAnimator;

class ENGINE_DLL CEffect abstract : public CGameObject
{
public:
	typedef struct tagEffectDesc
	{
		wstring strEffectTag;

		_float3 vPos;
		D3DXQUATERNION qRot;
		_float3 vScale;
	} EFFECT_DESC;

protected:
	CEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffect(const CEffect& Prototype);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	_int			m_iImagePerFrame = 0;
	_wstring		m_strEffectTag = {};



public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END