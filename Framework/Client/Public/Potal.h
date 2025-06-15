#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimator;
END

BEGIN(Client)

class CPotal final : public CGameObject
{
private:
	CPotal(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPotal(const CPotal& Prototype);
	virtual ~CPotal() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	//이벤트 함수 추가, 룸매니저의 체크포탈 함수 호출 매개변수로 m_eDirType

	POTAL_TYPE Get_PotalType() { return m_eDirType; }

	void SetUp_RenderState();
	void Reset_RenderState();

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CAnimator* m_pAnimatorCom = { nullptr };

	_uint m_iTextureIndex = 0;
	POTAL_TYPE m_eDirType = {};

private:
	HRESULT Ready_Components();

public:
	static CPotal* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END