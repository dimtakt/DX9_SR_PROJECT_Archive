#pragma once
#include "Client_Defines.h"
#include "ProgressBar.h"
#include "EventListener.h"
BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CTalent_Progress final : public CProgressBar, public IEventListener
{
private:
	CTalent_Progress(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTalent_Progress(const CTalent_Progress& Prototype);
	virtual					~CTalent_Progress() = default;
public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

public:
	void					Progress_Update(_float fCulValue);

private:
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	_bool					m_isFontRender = { true };

private:
	HRESULT					Ready_Components();

public:
	virtual void OnEvent(_uint iTypeindex, const EVENTDATA* pData);

public:
	static CTalent_Progress* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void			Free() override;
};
END
