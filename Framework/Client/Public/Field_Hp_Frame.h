#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)
class CField_Hp_Frame final : public CUIObject
{
public:
	typedef struct tagField_Hp_Fream
	{
		_float		fSizeX, fSizeY, fX, fY;;
		CTransform* pTarget_TransformCom;
		_uint		iTarget_Index;

	}UI_FIELD_HP_FREAM_DESC;

private:
	CField_Hp_Frame(LPDIRECT3DDEVICE9 pGraphic_Device);
	CField_Hp_Frame(const CField_Hp_Frame& Prototype);
	virtual						~CField_Hp_Frame() = default;
public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	LEVEL						m_eLevel = {};
	CTransform* m_pTarget_TransformCom = { nullptr };
	_uint						m_iTarget_Index = { };
private:
	HRESULT						Ready_Components();
	void						Target_Pos();

public:
	static CField_Hp_Frame* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};

END

