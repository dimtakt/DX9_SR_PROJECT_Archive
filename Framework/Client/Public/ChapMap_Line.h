#pragma once
#include "Client_Defines.h"
#include "Client_Defines_Map.h"
#include "UIObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CChapMap_Line final : public CUIObject
{
public:
	typedef struct tagMapLine
	{
		_float fX;
		_float fY;
		_float  fAngle;
		_uint  iIndex;
		_bool  bisDefalut;

	}MAP_LINE_DESC;
private:
									CChapMap_Line(LPDIRECT3DDEVICE9 pGraphic_Device);
									CChapMap_Line(const CChapMap_Line& Prototype);
	virtual							~CChapMap_Line() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	void							Set_TexIndex(_uint iIndex) { m_iTexIndex = iIndex; }
	void							isRend_On() { m_bisRand = true; }
	void							isRend_Off() { m_bisRand = false; }
private:
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };

	_float							m_fAngle = {};
	_uint							m_iIndex = {};
	_uint							m_iTexIndex = {};

	_bool							m_bisRand = { true };
	_bool							m_bisDefalut = { false };

private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();

public:
	static CChapMap_Line*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END



