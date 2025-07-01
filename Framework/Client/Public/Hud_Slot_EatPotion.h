#pragma once
#include "Client_Defines.h"
#include "ProgressBar.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CHud_Slot_EatPotion final : public CProgressBar
{
private:
								CHud_Slot_EatPotion(LPDIRECT3DDEVICE9 pGraphic_Device);
								CHud_Slot_EatPotion(const CHud_Slot_EatPotion& Prototype);
	virtual						~CHud_Slot_EatPotion() = default;
public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

public:
	void						Progerss_Set(_int iCulValue, _int iMaxValue);
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_bool						m_isFontRender = { true };

	void						Set_RenderState();
	void						Reset_RenderState();
private:
	HRESULT						Ready_Components();

public:
	static CHud_Slot_EatPotion*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
END
