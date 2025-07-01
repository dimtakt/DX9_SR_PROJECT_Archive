#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Engine_Defines.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CPoint : public CGameObject
{
public:
	typedef struct tagPointDesc : public LIGHTDATA
	{
		_wstring	strLightID;
		CTransform* pTargetTransform;
	}POINTDESC;
private:
	CPoint(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPoint(const CPoint& Prototype);
	virtual ~CPoint() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_Target(POINTDESC* pDesc);

private:
	_wstring m_strLightID = {};
	CTransform* m_pTargetTransformCom = { nullptr };
	_float3 m_vOffsetPos = {};
	_float3 m_vOffsetDir = {};

public:
	static CPoint* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END