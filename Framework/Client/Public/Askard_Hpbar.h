#pragma once
#include "Client_Defines.h"
#include "ProgressBar.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CAskard_Hpbar final : public CProgressBar
{
private:
	CAskard_Hpbar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAskard_Hpbar(const CAskard_Hpbar& Prototype);
	virtual					~CAskard_Hpbar() = default;
public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

public:
	void					HpBar_Set(_int iCulValue, _int iMaxValue);
private:
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	_bool					m_isFontRender = { true };

private:
	HRESULT					Ready_Components();

public:
	static CAskard_Hpbar*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END
