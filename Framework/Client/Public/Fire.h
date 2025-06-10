#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CFire final : public CGameObject
{
private:
	CFire(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFire(const CFire& Prototype);
	virtual ~CFire() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	D3DLIGHT9* Get_Lihgt() { return &m_Light; }
	_uint Get_LightIndex() { return m_iLightIndex; }
	
	void SetUp_RenderState();
	void Reset_RenderState();
private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };                    
	_uint m_iTextureIndex = 0;

	D3DLIGHT9		m_Light;         // 조명 정보
	_int			m_iLightIndex;	// 조명 인덱스 ( 같은 룸일 경우 따로 잡아줘야함 )
private:
	HRESULT Ready_Components();

public:
	static CFire* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END