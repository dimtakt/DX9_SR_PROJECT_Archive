#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "EventListener.h"
#include "Client_Defines_Event.h"
BEGIN(Client)

class CCamera_Follow final : public CCamera, public IEventListener
{
public:
	typedef struct tagCameraFolDesc
	{
		_uint iLayerIndex;
	}CAMERAFOLDESC;
public:
	enum class CAM_TRANS_STATE { NONE, ZOOM_OUT, ZOOM_IN_AFTER_TELEPORT };
	
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

public:
	virtual void OnEvent(_uint iTypeindex, const EVENTDATA* pData) override;

private:
	_float3			m_vOffset = {};
	_float			m_fCurrentAngle = {};
	_float			m_fMaxXRange = {};
	_float			m_fZMin = {};
	_float			m_fZMax = {};
	_float3			m_vTerrainPos = {};
	_bool			m_bCameraTransition = { false };
	_float3			m_vCamTransitionStartPos = {};
	_float3			m_vCamTransitionTargetPos = {};
	_float			m_fCamTransitionTimer = {};
	_float			m_fCamTransitionDuration = { };
	_float			m_fCurrentZRatio = {};
	CAM_TRANS_STATE m_eCamTransitionState = CAM_TRANS_STATE::NONE;
	_bool			m_bFirstFrame = true;

private:
	CTransform* m_pTargetPlayerTransformCom = { nullptr };
	vector<CTransform*> m_vRotateObjectsTransformCom = { };

private:
	HRESULT Ready_Target(void* pArg);
	HRESULT Ready_Components(void* pArg);

private:
	void	Move_Angle(_float fAngle, _float fTimeDelta);
	void	Follow_Target(_float fTimeDelta);

public:
	static CCamera_Follow* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END