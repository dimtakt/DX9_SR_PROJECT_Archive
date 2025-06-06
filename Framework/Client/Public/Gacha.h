#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
END
BEGIN(Client)
class CGacha final : public CUIObject
{
public:
	enum class GACHA_TYPE { ALL, ARTEFACT, STONE};
private:
									CGacha(LPDIRECT3DDEVICE9 pGraphic_Device);
									CGacha(const CGacha& Prototype);
	virtual							~CGacha() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	void							UI_Open(GACHA_TYPE eType);

	void							UI_Switch();
	void							Rand_Item_Set();
	void							Random_off() { m_bIsRandom = false; }
	void							Release_Slot();
private:
	LEVEL							m_eLevel = {};
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };

	_bool							m_bIsOpen = { false };
	_bool							m_bIsRandom = { false };

	GACHA_TYPE						eGacha_Type = {};
private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();

	void							Ready_Fx();
private:
	void							Rand_Itme(GACHA_TYPE eType);

public:
	static CGacha*					Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END

