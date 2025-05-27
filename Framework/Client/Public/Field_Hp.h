#pragma once
#include "Client_Defines.h"
#include "ProgressBar.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
END
BEGIN(Client)
class CField_Hp final : public CProgressBar
{
public:
	typedef struct tagField_Hp
	{
		_int iTarget_Index;
		_float fY;

	}UI_FIELD_HP_DESC;
private:
							CField_Hp(LPDIRECT3DDEVICE9 pGraphic_Device);
							CField_Hp(const CField_Hp& Prototype);
	virtual					~CField_Hp() = default;
public:
	virtual HRESULT			Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

private:
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	LEVEL					m_eLevel = {};

	CTransform*				m_pTarget_TransformCom = { nullptr };
	_uint					m_iTarget_Index = { };

private:
	HRESULT					Ready_Components();

	HRESULT					Ready_ChildPrototype(LEVEL eLevel);
	HRESULT					Ready_Children();

	HRESULT					Reday_SyncingObject();

	void					Target_Pos();
	void					Target_ProgressBar();
public:
	static CField_Hp*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END
