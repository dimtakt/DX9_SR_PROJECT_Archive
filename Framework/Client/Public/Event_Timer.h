#pragma once
#include "Client_Defines.h"
#include "ProgressBar.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CEvent_Timer final : public CProgressBar
{
private:
	CEvent_Timer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEvent_Timer(const CEvent_Timer& Prototype);
	virtual						~CEvent_Timer() = default;
public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

public:
	void						Progerss_Set(_float iCulValue, _float iMaxValue);
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_bool						m_isFontRender = { true };

	_float						m_fCulTime = {};
	_float						m_fCulMaxTime = {};
	_float						m_fPreTime = {};
	_float						m_fPreMaxTime = {};

private:
	void						ProgerssX();
private:
	HRESULT						Ready_Components();

public:
	static CEvent_Timer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
END
