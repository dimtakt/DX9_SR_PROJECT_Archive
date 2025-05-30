#pragma once

#include "GameObject.h"
BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	typedef struct tagUIObjectDesc
	{
		_float		fX, fY, fZ, fSizeX, fSizeY;
		_uint		iWinSizeX, iWinSizeY;

	}UIOBJECT_DESC;
protected:
	CUIObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	const _float3	Get_WoldPos() { return m_vWorldPos; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg = nullptr);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	_uint					m_iWinSizeX = {};
	_uint					m_iWinSizeY = {};

	_float					m_fX = {};
	_float					m_fY = {};
	_float					m_fZ = {};
	_float					m_fSizeX = {};
	_float					m_fSizeY = {};
	_float3					m_vWorldPos = {};

	_float4x4				m_ViewMatrix = {};
	_float4x4				m_ProjMatrix = {};

	_float4x4				m_OldViewMatrix = {};
	_float4x4				m_OldProjMatrix = {};

	CUIObject*				m_pParent = { nullptr };
	vector<CUIObject*>		m_vecChildren = {};

	class CTransform*		m_pTransformCom = { nullptr };
	RECT					m_vTexRect = {};
protected:
	void					Begin();
	void					End();
	_bool					isPick(HWND hWnd);

	void					Update_Position();
	void					Add_Child(CUIObject* pChildUI);

	void					Font_Rect_Update();
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void			Free();
};


END