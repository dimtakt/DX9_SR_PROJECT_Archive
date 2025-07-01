#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Engine_Defines.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CSun : public CGameObject
{
public:
	typedef struct tagSunDesc : public LIGHTDATA
	{
		_wstring	strLightID;
		_float3 vLightPos;
		_float3 vLightDir;
	}SUNDESC;
private:
	CSun(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSun(const CSun& Prototype);
	virtual ~CSun() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_Target(SUNDESC* pDesc);

private:
	_wstring m_strLightID = {};
	//CTransform* m_pTargetTransformCom = { nullptr };
	_float3 m_vOffsetPos = {};
	_float3 m_vOffsetDir = {};

public:
	static CSun* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END