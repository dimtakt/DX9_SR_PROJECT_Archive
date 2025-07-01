#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CAnimator;
END

BEGIN(Client)

class CTitle_Tree final : public CUIObject
{
private:
	CTitle_Tree(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTitle_Tree(const CTitle_Tree& Prototype);
	virtual ~CTitle_Tree() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom_Title_Tree = { nullptr };
	CTexture* m_pTextureCom_Title_TreeBegin = { nullptr };

	CAnimator* m_pAnimatorCom = { nullptr };
private:
	HRESULT Ready_Components();
	void SetUp_RenderState();
	void Reset_RenderState();

public:
	static CTitle_Tree* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END