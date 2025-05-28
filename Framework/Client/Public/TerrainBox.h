#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_TerrainBox;
END

class CTerrainBox final : public CGameObject
{
public:
	typedef struct TerrainBoxDesc
	{
		_float3		vScale{};
		_float3		vPosition{};
	}TERBOXDESC;
private:
	CTerrainBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTerrainBox(const CTerrainBox& Prototype);
	virtual ~CTerrainBox() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_TerrainBox* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureTop = { nullptr };			//윗면 전용 텍스처
	CTexture* m_pTextureSide = { nullptr };			//사이드 전용 텍스처
	CTransform* m_pTransformCom = { nullptr };

	_int m_iTopTextureIndex = 0;
	_int m_iSideTextureIndex = 0;

private:
	HRESULT Ready_Components();
	HRESULT Ready_Material();

public:
	static CTerrainBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

