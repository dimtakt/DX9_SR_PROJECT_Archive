#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CMonster_Default final : public CMonster
{
public:
	typedef struct tagMonsterDesc
	{
		int			iData = {};

	}MONSTER_DESC;
private:
	CMonster_Default(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonster_Default(const CMonster_Default& Prototype);
	virtual ~CMonster_Default() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:

	/* 멤버변수르ㅗ 직접들고 있을때? */
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

	_uint						m_iData = { };

private: /* 이 객체에게 필요한 컴포넌트들을 복제하여 추가해주는 기능. */
	HRESULT Ready_Components();
	void SetUp_RenderState();
	void Reset_RenderState();


public:
	static CMonster_Default* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END