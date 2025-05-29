#pragma once
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)

class ENGINE_DLL CLandObject abstract : public CGameObject
{
protected:
	CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLandObject(const CLandObject& Prototype);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	class CTransform*		m_pTransformCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };
	class CVIBuffer_Cube*	m_pVIBufferCom = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END