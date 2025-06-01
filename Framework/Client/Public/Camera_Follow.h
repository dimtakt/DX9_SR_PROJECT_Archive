#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"

BEGIN(Client)

class CCamera_Follow final : public CCamera
{
public:
	typedef struct tagCameraFolDesc
	{
		_uint iLayerIndex;
	}CAMERAFOLDESC;
private:
	CCamera_Follow(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera_Follow(const CCamera_Follow& Prototype);
	virtual ~CCamera_Follow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float3			m_vOffset{};
	_float			m_fCurrentAngle{};

private:
	CTransform* m_pTargetTransformCom = { nullptr };
	vector<CTransform*> m_vRotateObjectsTransformCom = { };

private:
	HRESULT Ready_Target(void* pArg);
	HRESULT Ready_Components(void* pArg);

private:
	void	Move_Angle(_float fAngle, _float fTimeDelta);
	void	Follow_Target();

public:
	static CCamera_Follow* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END