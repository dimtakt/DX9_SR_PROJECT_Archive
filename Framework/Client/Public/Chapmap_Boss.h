#include "UIObject.h"
#include "Client_Defines_Map.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CChapmap_Boss final : public CUIObject
{
public:
	typedef struct tagMapBoss
	{
		MAP_BOSS	eMap_Boss;
	}MAP_BOSS_DESC;
private:
									CChapmap_Boss(LPDIRECT3DDEVICE9 pGraphic_Device);
									CChapmap_Boss(const CChapmap_Boss& Prototype);
	virtual							~CChapmap_Boss() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };

	_uint							m_iMapTex = {};
private:
	HRESULT							Ready_Components();
	void							SetUp_RenderState();
	void							Reset_RenderState();


public:
	static CChapmap_Boss*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END



