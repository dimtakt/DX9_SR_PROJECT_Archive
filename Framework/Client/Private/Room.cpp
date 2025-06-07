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
	if (!m_bDead) 
	{
		if (m_bIsActive)
		{
			if (m_pTerrainBox != nullptr)
				m_pTerrainBox->Priority_Update(fTimeDelta);
			else if (m_pTerrainBox == nullptr || m_pTerrainBox->Get_IsDead())
				Safe_Release(m_pTerrainBox);

			for (auto it = m_vObject.begin(); it != m_vObject.end(); ) {
				if ((*it) == nullptr || (*it)->Get_IsDead()) {
					Safe_Release(*it);
					it = m_vObject.erase(it);
				}
				else {
					(*it)->Priority_Update(fTimeDelta);
					++it;
				}
			}

			for (auto it = m_vMonster.begin(); it != m_vMonster.end(); ) {
				if ((*it) == nullptr || (*it)->Get_IsDead()) {
					Safe_Release(*it);
					it = m_vMonster.erase(it);
				}
				else {
					(*it)->Priority_Update(fTimeDelta);
					++it;
				}
			}
		}
	}
}

void CRoom::Update(_float fTimeDelta)
{
	if (!m_bDead) {
		if (m_bIsActive)
		{
			if (m_pTerrainBox != nullptr)
				m_pTerrainBox->Update(fTimeDelta);
			else if (m_pTerrainBox == nullptr || m_pTerrainBox->Get_IsDead())
				Safe_Release(m_pTerrainBox);

			for (auto it = m_vObject.begin(); it != m_vObject.end(); ) {
				if ((*it) == nullptr || (*it)->Get_IsDead()) {
					Safe_Release(*it);
					it = m_vObject.erase(it);
				}
				else {
					(*it)->Update(fTimeDelta);
					++it;
				}
			}

			for (auto it = m_vMonster.begin(); it != m_vMonster.end(); ) {
				if ((*it) == nullptr || (*it)->Get_IsDead()) {
					Safe_Release(*it);
					it = m_vMonster.erase(it);
				}
				else {
					(*it)->Update(fTimeDelta);
					++it;
				}
			}
		}
	}
}

void CRoom::Late_Update(_float fTimeDelta)
{
	if (!m_bDead)
	{
		if (m_bIsVisited)
		{
			if (m_pTerrainBox != nullptr)
				m_pTerrainBox->Late_Update(fTimeDelta);

			/*if (m_pFogPlane != nullptr)
				m_pFogPlane->Late_Update(fTimeDelta);*/

			for (auto it = m_vObject.begin(); it != m_vObject.end(); ) {
				if ((*it) != nullptr && (*it)->Get_IsDead()) {
					Safe_Release(*it);
					it = m_vObject.erase(it); // erase는 다음 이터레이터 반환
				}
				else {
					if (nullptr != *it)
						(*it)->Late_Update(fTimeDelta);
					++it;
				}
			}

			for (auto it = m_vPotal.begin(); it != m_vPotal.end(); ) {
				if ((*it) == nullptr || (*it)->Get_IsDead()) { 
					Safe_Release(*it);
					it = m_vPotal.erase(it); 
				}
				else {
					if (nullptr != *it) {
						if (m_vMonster.size() == 0)
						{
							if (!m_bIsClear && it != m_vPotal.end())
							{
								// collider
								CCollider_OBB::OBB_DESC tColliderDesc;
								tColliderDesc.vScale = _float3(1.f, 3.f, 1.f);
								tColliderDesc.pOwner = (*it);
								tColliderDesc.pTransform = dynamic_cast<CTransform*>((*it)->Find_Component(TEXT("Com_Transform")));
								tColliderDesc.eType = (*it)->Get_ObjType();
								CCollider_OBB* pCol = dynamic_cast<CCollider_OBB*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"), &tColliderDesc));
								m_pGameInstance->Add_Collider(pCol);
								if(it == m_vPotal.end() - 1)
									m_bIsClear = true;
							}
							(*it)->Late_Update(fTimeDelta);
						}
					}
						
					++it;
				}
			}

		}
		if (m_bIsActive)
		{
			for (auto it = m_vMonster.begin(); it != m_vMonster.end(); ) {
				if ((*it) == nullptr || (*it)->Get_IsDead()) { 
					Safe_Release(*it);
					it = m_vMonster.erase(it);
				}
				else {
					if (nullptr != *it)
						(*it)->Late_Update(fTimeDelta);
					++it;
				}
			}
		}
	}
	
}

HRESULT CRoom::Render()
{
	if (m_bIsVisited)
	{
		if (m_pTerrainBox != nullptr)
			m_pTerrainBox->Render();
		else if (m_pTerrainBox == nullptr || m_pTerrainBox->Get_IsDead())
			Safe_Release(m_pTerrainBox);

		for (auto it = m_vObject.begin(); it != m_vObject.end(); ) {
			if ((*it) == nullptr || (*it)->Get_IsDead()) {
				Safe_Release(*it);
				it = m_vObject.erase(it);
			}
			else {
				(*it)->Render();
				++it;
			}
		}
	}

	if (m_bIsActive)
	{
		for (auto it = m_vMonster.begin(); it != m_vMonster.end(); ) {
			if ((*it) == nullptr || (*it)->Get_IsDead()) {
				Safe_Release(*it);
				it = m_vMonster.erase(it);
			}
			else {
				(*it)->Render();
				++it;
			}
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

HRESULT CRoom::Ready_Potal(_uint iLayerLevelIndex, const _wstring& strLayerTag, _float3 vOffset, POTAL_TYPE eType)
{
	OBJECT_INTERACTION_DESC pDesc{};
	pDesc.ePotalType = eType;
	pDesc.iTextureIndex = 0;
	pDesc.vScale = { 1.f, 1.f, 1.f };
	pDesc.vRotate = { 0.f, 0.f, 0.f };
	CTransform* pTransform = static_cast<CTransform*>(m_pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
	pDesc.vPos = pTransform->Get_State(STATE::POSITION) + vOffset;

	/*m_pGameInstance->Add_GameObject_ToLayer(
		iLayerLevelIndex, strLayerTag,
		iLayerLevelIndex,
		TEXT("Prototype_GameObject_Potal"),
		&pDesc);*/

	CPotal* pPotal = dynamic_cast<CPotal*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iLayerLevelIndex, TEXT("Prototype_GameObject_Potal"), &pDesc));

	//CPotal* pPotal = static_cast<CPotal*>(m_pGameInstance->Get_LastGameObject(iLayerLevelIndex, strLayerTag));

	m_vPotal.push_back(pPotal);

	return E_NOTIMPL;
}

CPotal* CRoom::Find_Potal(POTAL_TYPE ePotal)
{
	for (auto& pPotal : m_vPotal)
	{
		if (pPotal->Get_PotalType() == ePotal)
			return pPotal;
	}
	return nullptr;
}

HRESULT CRoom::Load_From_File(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _tchar* pLoadFileTag, _int iIndex, _int RoomX , _int RoomZ, ROOM_INFO Event)
{
	Compute_ObjectOffset(RoomX, RoomZ);

	if (Event == ROOM_INFO::EVENT_NORMAL)
		m_eRoomType = ROOM_INFO::EVENT_NORMAL;
	else if (Event == ROOM_INFO::EVENT_SHOP)
		m_eRoomType = ROOM_INFO::EVENT_SHOP;
	else if (Event == ROOM_INFO::EVENT_HP)
		m_eRoomType = ROOM_INFO::EVENT_HP;
	else if (Event == ROOM_INFO::EVENT_ARTEFACT)
		m_eRoomType = ROOM_INFO::EVENT_ARTEFACT;
	else if (Event == ROOM_INFO::EVENT_EXP)
		m_eRoomType = ROOM_INFO::EVENT_EXP;
	else if (Event == ROOM_INFO::EVENT_STONE)
		m_eRoomType = ROOM_INFO::EVENT_STONE;


	/*m_iID = iIndex;*/
	m_iRoomX = RoomX;
	m_iRoomZ = RoomZ;

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

	_bool bIsTerrain = false;

	for (auto& pDesc : m_Object_Desc)
	{
		if (pDesc.eType == GAMEOBJ_TYPE::OBJECT || pDesc.eType == GAMEOBJ_TYPE::OBJECT_DECO)
		{
			MAP_OBJECT_DESC  tSrc{};
			tSrc.iTextureIndex = pDesc.iTextureIndex;
			tSrc.vPos = pDesc.vPos + m_ObjectOffset;
			tSrc.vScale = pDesc.vScale;
			tSrc.vRotate = pDesc.vRotate;
			if (pDesc.eType == GAMEOBJ_TYPE::OBJECT_DECO)
				tSrc.eType = GAMEOBJ_TYPE::OBJECT_DECO;

			CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iLayerLevelIndex, TEXT("Prototype_GameObject_Tree"), &tSrc));

			m_vObject.push_back(pGameObject);
		}
		else if (pDesc.eType == GAMEOBJ_TYPE::TERRAIN)
		{
			if (!bIsTerrain) {
				MAP_OBJECT_DESC tSrc{};
				tSrc.iTextureIndex = pDesc.iTextureIndex;
				tSrc.vPos = pDesc.vPos + m_ObjectOffset;
				tSrc.vScale = pDesc.vScale;
				tSrc.eType = pDesc.eType;

				m_pTerrainBox = dynamic_cast<CTerrainBox*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iLayerLevelIndex, TEXT("Prototype_GameObject_TerrainBox"), &tSrc));

				//m_pFogPlane = dynamic_cast<CFogPlane*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_FogPlane"), &tSrc));

				bIsTerrain = true;
			}
		}
		else
		{
			MAP_OBJECT_DESC tSrc{};
			tSrc.eType = pDesc.eType;
			tSrc.vPos = pDesc.vPos + m_ObjectOffset;
			tSrc.vScale = pDesc.vScale;
			tSrc.vRotate = pDesc.vRotate;


			CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Interaction_Normal"), &tSrc));
	/*		CGameObject* pGameObject = m_pGameInstance->Get_LastGameObject(iLayerLevelIndex, strLayerTag);*/
			m_vObject.push_back(pGameObject);
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
		if (pMonster != nullptr) {
			pMonster->Set_IsActive(true);
			// collider
			CCollider_OBB::OBB_DESC tColliderDesc;
			tColliderDesc.vScale = _float3(1.f, 3.f, 1.f);
			tColliderDesc.pOwner = pMonster;
			tColliderDesc.pTransform = pMonster->Get_Transform();
			tColliderDesc.eType = pMonster->Get_ObjType();
			CCollider_OBB* pCol = dynamic_cast<CCollider_OBB*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"), &tColliderDesc));
			m_pGameInstance->Add_Collider(pCol);
		}
	}

	for (auto& pObject : m_vObject)
	{
		if (pObject != nullptr)
		{
			pObject->Set_IsActive(true);
			// collider
			CCollider_OBB::OBB_DESC tColliderDesc;
			tColliderDesc.vScale = _float3(1.0f, 3.f, 1.0f);
			tColliderDesc.pOwner = pObject;
			tColliderDesc.pTransform = dynamic_cast<CTransform*>(pObject->Find_Component(TEXT("Com_Transform")));
			tColliderDesc.eType = pObject->Get_ObjType();;
			CCollider_OBB* pCol = dynamic_cast<CCollider_OBB*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"), &tColliderDesc));
			m_pGameInstance->Add_Collider(pCol);
		}
		
	}
}

void CRoom::Exit()
{
	for (auto& pMonster : m_vMonster)
	{
		if (pMonster != nullptr)
			pMonster->Set_IsActive(false);
	}

	for (auto& pObject : m_vObject)
	{
		if(pObject != nullptr)
			pObject->Set_IsActive(false);
	}

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
	Safe_Release(m_pTerrainBox);

	for (auto& obj : m_vObject)
	{
		obj->Set_IsDead(true);
		Safe_Release(obj);
	}
	m_vObject.clear();

	for (auto& obj : m_vMonster)
	{
		obj->Set_IsDead(true);
		Safe_Release(obj);
	}
	m_vMonster.clear();

	for (auto& obj : m_vPotal)
	{
		obj->Set_IsDead(true);
		Safe_Release(obj);

	}
	m_vPotal.clear();

	m_Object_Desc.clear();

	__super::Free();
}
