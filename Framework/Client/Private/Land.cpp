#include "Land.h"
#include "GameInstance.h"

CLand::CLand(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLand_Cube{ pGraphic_Device }
{
}

CLand::CLand(const CLand& Prototype)
	: CLand_Cube{ Prototype }
{
}

HRESULT CLand::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLand::Initialize(void* pArg)
{

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaling(5.f, 1.f, 5.f);

	return S_OK;
}

void CLand::Priority_Update(_float fTimeDelta)
{
}

void CLand::Update(_float fTimeDelta)
{

}

void CLand::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CLand::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(3)))
		return E_FAIL;

	/* 그리기위해 이용할 자원과 설정들을 장치에 바인딩한다. */
	m_pVIBufferCom->Bind_Buffers();
	
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CLand::Ready_Components(void* pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For,Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), pArg)))
		return E_FAIL;

	/* For,Com_Texture*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_Component_Texture_Sky"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLand::Ready_Material()
{
	D3DMATERIAL9		MtrlDesc{};

	MtrlDesc.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	MtrlDesc.Specular = { 1.f, 1.f, 1.f, 1.f };
	MtrlDesc.Ambient = { 1.f, 1.f, 1.f, 1.f };

	MtrlDesc.Emissive = { 0.f, 0.f, 0.f, 0.f };
	MtrlDesc.Power = 1.f;
	m_pGraphic_Device->SetMaterial(&MtrlDesc);

	return S_OK;
}

CLand* CLand::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLand* pInstance = new CLand(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CLand::Clone(void* pArg)
{
	CLand* pInstance = new CLand(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CLand::Free()
{
	__super::Free();

}
