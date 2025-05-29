#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Free final : public CCamera
{
public:
	typedef struct tagCameraFreeDesc : public CCamera::CAMERA_DESC
	{
		_float			fMouseSensor{};
		CTransform*		pTargetTransform{ nullptr };
	}CAMERA_DESC;

private:
	CCamera_Free(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera_Free(const CCamera_Free& Prototype);
	virtual ~CCamera_Free() = default;

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
	POINT			m_OldPoint = {};
	_float			m_fMouseSensor = {};

private:
	CTransform* m_pTargetTransformCom = { nullptr };

private:
	HRESULT Ready_Components(void* pArg);

private:
	void	Mouse_Move(_float fTimeDelta);
	void	Move_Angle(_float fAngle, _float fTimeDelta);
	void	Follow_Target();

public:
	static CCamera_Free* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END