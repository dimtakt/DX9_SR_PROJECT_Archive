#pragma once
#include "Client_Defines.h"
#include "ProgressBar.h"
#include "Engine_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
END
BEGIN(Client)
class CAttackGauge final : public CProgressBar
{
private:
	CAttackGauge(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAttackGauge(const CAttackGauge& Prototype);
	virtual					~CAttackGauge() = default;
public:
	void					Render_Progress(CTransform* pTransform, _int iCulValue, _int iMaxValue);

public:
	virtual HRESULT			Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	LEVEL					m_eLevel = {};

private:
	HRESULT					Ready_Components();

	HRESULT					Ready_ChildPrototype(LEVEL eLevel);
	HRESULT					Ready_Children();

	void					Target_Pos(CTransform* pTransform);
	void					Target_ProgressBar(CTransform* pTransform);
public:
	static CAttackGauge* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void			Free() override;
};
END

