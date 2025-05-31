#include "Room.h"
#include "GameInstance.h"

CRoom::CRoom(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CRoom::CRoom(const CRoom& Prototype)
	: CGameObject{ Prototype }
	, m_bIsActive{ Prototype.m_bIsActive }
	, m_bIsVisited{ Prototype.m_bIsVisited }
{
}

HRESULT CRoom::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CRoom::Initialize(void* pArg)
{

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRoom::Priority_Update(_float fTimeDelta)
{
	if (m_bIsActive)
	{
		if (m_pTerrainBox != nullptr)
			m_pTerrainBox->Priority_Update(fTimeDelta);
		// 추후 지형 오브젝트 처리
		/*for (auto& obj : m_vObject)
		{
			obj->
		}*/

		for (auto& obj : m_vMonster)
		{
			obj->Priority_Update(fTimeDelta);
		}
	}
}

void CRoom::Update(_float fTimeDelta)
{
	if (m_bIsActive)
	{
		if (m_pTerrainBox != nullptr)
			m_pTerrainBox->Update(fTimeDelta);
		// 추후 지형 오브젝트 처리
		/*for (auto& obj : m_vObject)
		{
			obj->
		}*/


		for (auto& obj : m_vMonster)
		{
			obj->Update(fTimeDelta);
		}
	}

}

void CRoom::Late_Update(_float fTimeDelta)
{
	if (m_bIsActive)
	{
		if (m_pTerrainBox != nullptr)
			m_pTerrainBox->Late_Update(fTimeDelta);
		// 추후 지형 오브젝트 처리
		/*for (auto& obj : m_vObject)
		{
			obj->
		}*/

		for (auto& obj : m_vMonster)
		{
			obj->Late_Update(fTimeDelta);
		}
	}
}

HRESULT CRoom::Render()
{
	if (m_bIsActive)
	{
		if (m_pTerrainBox != nullptr)
			m_pTerrainBox->Render();
		// 추후 지형 오브젝트 처리
		/*for (auto& obj : m_vObject)
		{
			obj->
		}*/

		for (auto& obj : m_vMonster)
		{
			obj->Render();
		}
	}
	return S_OK;
}

HRESULT CRoom::Ready_Components(void* pArg)
{
	return S_OK;
}
HRESULT CRoom::Ready_Objects(void* pArg)
{
	  
	return S_OK;
}

HRESULT CRoom::Load_From_File(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _tchar* pLoadFileTag, _int iIndex, _int RoomX , _int RoomZ)
{
	Compute_ObjectOffset(RoomX, RoomZ);

	_tchar szFileName[MAX_PATH] = {};
	
	wsprintf(szFileName, pLoadFileTag, iIndex);

	std::ifstream ifile(szFileName, std::ios::binary);
	if (!ifile.is_open())
	{ 
		 
		MessageBox(NULL, szFileName, L"파일 열기 실패!", MB_OK);
		return E_FAIL;
	}

	if (ifile.is_open())
	{
	
		while (!ifile.eof())
		{
			MAP_OBJECT_DESC Desc = {};

			ifile.read(reinterpret_cast<char*>(&Desc), sizeof(MAP_OBJECT_DESC));

			if (ifile.gcount() == sizeof(MAP_OBJECT_DESC))
				m_Object_Desc.push_back(Desc);
		}
	}
	ifile.close();

	for (auto& pDesc : m_Object_Desc)
	{
		if (pDesc.eType == GAMEOBJ_TYPE::OBJECT)
		{
			MAP_OBJECT_DESC  tSrc{};
			tSrc.iTextureIndex = pDesc.iTextureIndex;
			tSrc.vPos = pDesc.vPos + m_ObjectOffset;
			tSrc.vScale = pDesc.vScale;
			tSrc.vRotate = pDesc.vRotate;

			m_pGameInstance->Add_GameObject_ToLayer(
				iLayerLevelIndex, strLayerTag,
				iLayerLevelIndex,
				TEXT("Prototype_GameObject_Tree"),
				&tSrc);

			CGameObject* pGameObject = m_pGameInstance->Get_LastGameObject(iLayerLevelIndex, strLayerTag);
			m_vObject.push_back(pGameObject);
		}
		else
		{
			MAP_OBJECT_DESC tSrc{};
			tSrc.iTextureIndex = pDesc.iTextureIndex;
			tSrc.vPos = pDesc.vPos + m_ObjectOffset;
			tSrc.vScale = pDesc.vScale;

			m_pGameInstance->Add_GameObject_ToLayer(
				iLayerLevelIndex, strLayerTag,
				iLayerLevelIndex,
				TEXT("Prototype_GameObject_TerrainBox"),
				&tSrc);

			CGameObject* pGameObject = m_pGameInstance->Get_LastGameObject(iLayerLevelIndex, strLayerTag);
			m_pTerrainBox = dynamic_cast<CTerrainBox*>(pGameObject);
		}
	}


	return S_OK;
}

void CRoom::Compute_ObjectOffset(_int x, _int z)
{
	_float fX = 50 * x;
	_float fZ = 50 * z;

	m_ObjectOffset = { fX, 0.f, fZ };
}

void CRoom::Enter()
{
	m_bIsActive = true;
	m_bIsVisited = true;

	for (auto& pMonster : m_vMonster)
	{
		m_pGameInstance->Add_Collider(pMonster->Get_Collider());
	}
}

void CRoom::Exit()
{
	m_bIsActive = false;
}

void CRoom::Activate()
{
	m_bIsActive = true;
}

void CRoom::Deactivate()
{
	m_bIsActive = false;
}

CRoom* CRoom::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRoom* pInstance = new CRoom(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CRoom"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CRoom::Clone(void* pArg)
{
	CRoom* pInstance = new CRoom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CRoom"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CRoom::Free()
{
	__super::Free();

	Safe_Release(m_pTerrainBox);
	for (auto& obj : m_vObject)
	{
		Safe_Release(obj);
	}
	m_vObject.clear();
	for (auto& obj : m_vMonster)
	{
		Safe_Release(obj);
	}
	m_vMonster.clear();
}
