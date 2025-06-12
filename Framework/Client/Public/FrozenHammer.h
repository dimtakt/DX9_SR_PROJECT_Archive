#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "EventListener.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CAnimator;
class CTransform;
END

BEGIN(Client)

class CFrozenHammer final : public CGameObject, public IEventListener 
{
public:
	typedef struct tagFronzenHammerDesc
	{
		CTransform* pTargetTransform;
	}FROZENHAMMERDESC;
private:
	CFrozenHammer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFrozenHammer(const CFrozenHammer& Prototype);
	virtual ~CFrozenHammer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void SetUp_RenderState();
	void Reset_RenderState();
	virtual void OnEvent(_uint iTypeindex, const EVENTDATA* pData);
private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTargetTransform = { nullptr };

	_float m_fAttackCool = { 60.f };
	_float m_fCulAttackCool = { 0.f };


private:
	HRESULT Ready_Components(void* pArg);

public:
	static CFrozenHammer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END