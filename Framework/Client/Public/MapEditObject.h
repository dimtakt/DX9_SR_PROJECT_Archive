#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CMapEditObject final : public CGameObject
{
private:
	CMapEditObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMapEditObject(const CMapEditObject& Prototype);
	virtual ~CMapEditObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	_uint Get_LightIndex() { return m_iLightIndex; }

	void SetUp_RenderState();
	void Reset_RenderState();
private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_uint m_iTextureIndex = 0;
	_uint m_iLightIndex = 0;

private:
	HRESULT Ready_Components();

public:
	static CMapEditObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

