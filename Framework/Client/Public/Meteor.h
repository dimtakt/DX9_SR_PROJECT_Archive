#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "EventListener.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CMeteor final : public CGameObject, public IEventListener
{
public:
	typedef struct tagMeteorDesc
	{
		CTransform* pPlayerTransform;
	}METEORESC;
private:
	CMeteor(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMeteor(const CMeteor& Prototype);
	virtual ~CMeteor() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnEvent(_uint iTypeindex, const EVENTDATA* pData);
	void Create_Bolt();

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_float3 m_vDirection = {};			//발사 방향

	_float m_fCurrentCoolTime = 0.f;
	_float m_fMaxCoolTime = 26.f;

	CTransform* m_pPlayerTransformCom = { nullptr };

private:
	HRESULT Ready_Components(void* pArg);

public:
	static CMeteor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
