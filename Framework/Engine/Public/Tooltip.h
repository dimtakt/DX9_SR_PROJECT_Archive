#pragma once
#include "UIObject.h"

BEGIN(Engine)
class ENGINE_DLL CTooltip abstract : public CUIObject
{
public:
	typedef struct Tag_Tooltip : public UIOBJECT_DESC
	{
		class CItemObject* pItem;

	}TOOLTIP_DESC;
protected:
								CTooltip(LPDIRECT3DDEVICE9 pGraphic_Device);
								CTooltip(const CTooltip& Prototype);
	virtual						~CTooltip() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg = nullptr);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	void						Change_Item(class CItemObject* pItem);
protected:
	class CItemObject*			m_pItemObject = {nullptr};

public:
	virtual CGameObject*		Clone(void* pArg) = 0;
	virtual void				Free();
};
END
