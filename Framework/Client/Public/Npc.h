#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Field_Npc_Chat.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimator;
END

BEGIN(Client)

class CNpc final : public CGameObject
{
public:
	enum class NPC_TYPE {BLACKSMITH, ELDER, SHIELDDOG, SWORDSHIELD};
	typedef struct tagNpcDesc {
		NPC_TYPE eType;
	}NPCDESC;
private:
	CNpc(LPDIRECT3DDEVICE9 pGraphic_Device);
	CNpc(const CNpc& Prototype);
	virtual ~CNpc() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void SetUp_RenderState();
	void Reset_RenderState();


private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CAnimator* m_pAnimatorCom = { nullptr };
	CField_Npc_Chat* m_pChat = { nullptr };
	NPC_TYPE	m_eType = {};

private:
	HRESULT Ready_Components();

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

public:
	static CNpc* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END