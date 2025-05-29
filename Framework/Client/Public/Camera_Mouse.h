#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Mouse final : public CCamera
{
private:
	CCamera_Mouse(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera_Mouse(const CCamera_Mouse& Prototype);
	virtual ~CCamera_Mouse() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	POINT			m_OldPoint = {};
	_float			m_fMouseSensor = {};

private:
	void Ready_Mouse();
	HRESULT Ready_Components(void* pArg);

private:
	void	Mouse_Move(_float fTimeDelta);
	void	Key_Input(_float fTimeDelta);

public:
	static CCamera_Mouse* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END