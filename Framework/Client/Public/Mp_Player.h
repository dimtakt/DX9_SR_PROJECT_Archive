#pragma once
#include "Client_Defines.h"
#include "ProgressBar.h"
BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CMp_Player final : public CProgressBar
{
private:
							CMp_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
							CMp_Player(const CMp_Player& Prototype);
	virtual					~CMp_Player() = default;
public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

private:
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };

private:
	HRESULT					Ready_Components();

public:
	static CMp_Player*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END
