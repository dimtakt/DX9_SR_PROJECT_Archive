#include "Room.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
#include "Monster_Factory.h"
#include "Interaction_Normal.h"

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

			for (auto it = m_vFire.begin(); it != m_vFire.end();)
			{
				if ((*it) == nullptr || (*it)->Get_IsDead())
				{
					Safe_Release(*it);
					it = m_vFire.erase(it);
				}
				else
				{
					(*it)->Priority_Update(fTimeDelta);
					++it;
				}
			}

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
	if (m_bParticleRoom)
	{
		m_pGameInstance->Update(fTimeDelta, ENUM_CLASS(m_eRoomParticle));
	}
	if (!m_bDead) {
		if (m_bIsActive)
		{
			if (m_pTerrainBox != nullptr)
				m_pTerrainBox->Update(fTimeDelta);
			else if (m_pTerrainBox == nullptr || m_pTerrainBox->Get_IsDead())
				Safe_Release(m_pTerrainBox);

			for (auto it = m_vFire.begin(); it != m_vFire.end();)
			{
				if ((*it) == nullptr || (*it)->Get_IsDead())
				{
					Safe_Release(*it);
					it = m_vFire.erase(it);
				}
				else
				{
					(*it)->Update(fTimeDelta);
					++it;
				}
			}

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
			if(m_vMonster.size() == 0)
				CStat_Manager::GetInstance()->Set_Battle(FALSE);

		}
	}
}

void CRoom::Late_Update(_float fTimeDelta)
{
	if (m_bParticleRoom)
	{
		m_pGameInstance->Late_Update(fTimeDelta, ENUM_CLASS(m_eRoomParticle));
	}

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
						if (m_vMonster.size() == 0 || m_bForcePotal_Active)
						{
							if (!m_bIsClear && it != m_vPotal.end())
							{
								// collider
								CCollider_OBB::OBB_DESC tColliderDesc;
								tColliderDesc.vScale = _float3(0.5f, 1.f, 0.5f);
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
			for (auto it = m_vFire.begin(); it != m_vFire.end();)
			{
				if ((*it) == nullptr || (*it)->Get_IsDead())
				{
					Safe_Release(*it);
					it = m_vFire.erase(it);
				}
				else
				{
					(*it)->Late_Update(fTimeDelta);
					++it;
				}
			}

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

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);


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
	
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (m_bIsActive)
	{
		for (auto it = m_vFire.begin(); it != m_vFire.end();)
		{
			if ((*it) == nullptr || (*it)->Get_IsDead())
			{
				Safe_Release(*it);
				it = m_vFire.erase(it);
			}
			else
			{
				m_pGraphic_Device->LightEnable((*it)->Get_LightIndex(), FALSE);
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
	if (iLayerLevelIndex == ENUM_CLASS(LEVEL::LEVEL_BOSS1) || iLayerLevelIndex == ENUM_CLASS(LEVEL::LEVEL_BOSS2))
		pDesc.ePotalType = POTAL_TYPE::BOSS_POTAL;
	else
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

	return S_OK;
}

HRESULT CRoom::Ready_Stage_Potal(_uint iLayerLevelIndex, const _wstring& strLayerTag, _float3 vOffset, POTAL_TYPE eType)
{
	OBJECT_INTERACTION_DESC pDesc{};
	pDesc.ePotalType = eType;
	pDesc.iTextureIndex = 0;
	pDesc.vScale = { 5.f, 1.f, 5.f };
	pDesc.vRotate = { 0.f, 0.f, 0.f };
	CTransform* pTransform = static_cast<CTransform*>(m_pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
	pDesc.vPos = pTransform->Get_State(STATE::POSITION) + vOffset;

	CPotal* pPotal = dynamic_cast<CPotal*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iLayerLevelIndex, TEXT("Prototype_GameObject_Potal"), &pDesc));

	m_vPotal.push_back(pPotal);

	return S_OK;
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

HRESULT CRoom::On_Fire()
{
	if (m_bIsActive)
	{
		for (auto it = m_vFire.begin(); it != m_vFire.end();)
		{
			m_pGraphic_Device->SetLight((*it)->Get_LightIndex(), (*it)->Get_Lihgt());
			m_pGraphic_Device->LightEnable((*it)->Get_LightIndex(), TRUE);
			(*it)->Render();
			++it;
		}
	}

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

HRESULT CRoom::Off_Fire()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (m_bIsActive)
	{
		for (auto it = m_vFire.begin(); it != m_vFire.end();)
		{
			if ((*it) == nullptr || (*it)->Get_IsDead())
			{
				Safe_Release(*it);
				it = m_vFire.erase(it);
			}
			else
			{
				m_pGraphic_Device->LightEnable((*it)->Get_LightIndex(), FALSE);
				++it;
			}
		}
	}

	return S_OK;
}

vector<CMonster*> CRoom::Get_MonsterList()
{
	vector<CMonster*> vMonsterList{};
	for (auto& pMonster : m_vMonster) {
		if (pMonster->Get_Summoned())
		{
			vMonsterList.push_back(pMonster);
		}
	}

	return vMonsterList;
}

HRESULT CRoom::Load_From_File(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _tchar* pLoadFileTag, _int iIndex, _int RoomX , _int RoomZ, ROOM_INFO Event)
{
	Compute_ObjectOffset(RoomX, RoomZ);

	if (Event == ROOM_INFO::EVENT_BOSS)
		m_ObjectOffset * 2;

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

	list<CMonster::MONSTERDESC> MonsterDescList;

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
		else if (pDesc.eType == GAMEOBJ_TYPE::MONSTER_MOLE || pDesc.eType == GAMEOBJ_TYPE::MONSTER_OINK || pDesc.eType == GAMEOBJ_TYPE::MONSTER_LASERGHOST)
		{
			
			CMonster::MONSTERDESC tDesc = {};
			tDesc.iLayerLevelIndex = iLayerLevelIndex;
			tDesc.iPrototypeLevelIndex = iLayerLevelIndex;
			tDesc.strLayerTag = strLayerTag;
			tDesc.vPosition = pDesc.vPos + m_ObjectOffset;
			tDesc.vPosition.y = 20.f;
			if (pDesc.eType == GAMEOBJ_TYPE::MONSTER_MOLE) {
				tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_Mole_A");
				tDesc.eType = MONSTER_TYPE_A::MONSTER_MOLE_A;
			}	
			else if (pDesc.eType == GAMEOBJ_TYPE::MONSTER_OINK)
			{
				tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_Oink_A");
				tDesc.eType = MONSTER_TYPE_A::MONSTER_OINK_A;
			}                                                                                                                                                              
			else if (pDesc.eType == GAMEOBJ_TYPE::MONSTER_LASERGHOST)
			{
				tDesc.strPrototypeTag = TEXT("Prototype_GameObject_Monster_LaserGhost_D");
				tDesc.eType = MONSTER_TYPE_A::MONSTER_LASERGHOST_D;
			}
				
			MonsterDescList.push_back(tDesc);
		}
		else if (pDesc.eType == GAMEOBJ_TYPE::FIRE)
		{
			MAP_OBJECT_DESC tSrc{};
			tSrc.eType = pDesc.eType;
			tSrc.iTextureIndex = pDesc.iTextureIndex;
			tSrc.vPos = pDesc.vPos;
			tSrc.vScale = pDesc.vScale;
			tSrc.vRotate = pDesc.vRotate;
			
			CFire* pFire = dynamic_cast<CFire*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iLayerLevelIndex, TEXT("Prototype_GameObject_Fire"), &tSrc));
			m_vFire.push_back(pFire);
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

	for (auto& desc : MonsterDescList)
	{
		desc.pTerrainBox = m_pTerrainBox;
	}

	CMonster_Factory::GetInstance()->Add_MonstersV2(this, MonsterDescList);
	MonsterDescList.clear();

	/*m_pGameInstance->Create_Particle(ENUM_CLASS(PARTICLE_TYPE::RAIN), iLayerLevelIndex, TEXT("Prototype_GameObject_Rain"), _float3(0.5f, 1.f, 1.f));*/
	return S_OK;
}

HRESULT CRoom::Load_Particle(PARTICLE_TYPE eType, const _wstring& strLayerTag, _uint iLayerLevelIndex, _float3 ParticleScaled, _int iOffSetType)
{
	if (eType == PARTICLE_TYPE::RAIN)
		m_pGameInstance->Create_Particle_Fast(ENUM_CLASS(eType), iLayerLevelIndex, strLayerTag, iOffSetType, ParticleScaled );
	else 
		m_pGameInstance->Create_Particle_Low(ENUM_CLASS(eType), iLayerLevelIndex, strLayerTag, iOffSetType);
	
	return S_OK;
}

void CRoom::Compute_ObjectOffset(_int x, _int z)
{
	_float fX = 50 * x;
	_float fZ = 50 * z;

	m_ObjectOffset = { fX, 0.f, fZ };
}

CMonster* CRoom::Find_Monster(MONSTER_TYPE eType)
{

	for (auto& pMonster : m_vMonster)
	{
		if (pMonster->Get_MonsterType() == eType)
		{
			return pMonster;
		}
	}

	return nullptr;
}

void CRoom::Enter()
{
	m_bIsActive = true;
	m_bIsVisited = true;
	CStat_Manager::GetInstance()->Set_Battle(TRUE);
	for (auto& pMonster : m_vMonster)
	{
		if (pMonster != nullptr) {
			if (pMonster->Get_MonsterType() == MONSTER_TYPE::ERMA_HEAD || pMonster->Get_MonsterType() == MONSTER_TYPE::ERMA_HAND_L || pMonster->Get_MonsterType() == MONSTER_TYPE::ERMA_HAND_R)
				continue;
			pMonster->Set_IsActive(true);
			// collider
			CCollider_OBB::OBB_DESC tColliderDesc;
			tColliderDesc.vScale = _float3(0.5f, 1.f, 0.5f);
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
			tColliderDesc.vScale = _float3(0.5f, 1.f, 0.5f);
			tColliderDesc.pOwner = pObject;
			tColliderDesc.pTransform = dynamic_cast<CTransform*>(pObject->Find_Component(TEXT("Com_Transform")));
			tColliderDesc.eType = pObject->Get_ObjType();;
			CCollider_OBB* pCol = dynamic_cast<CCollider_OBB*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"), &tColliderDesc));
			m_pGameInstance->Add_Collider(pCol);

			if (pObject->Get_ObjType() == GAMEOBJ_TYPE::MERCAHNT)
			{
				dynamic_cast<CInteraction_Normal*>(pObject)->ReadyShopItemCollision();
			}
		}
		
	}

	if(m_bParticle)
	{
		CTransform* pTrasnform = static_cast<CTransform*>(m_pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
		_float3 vPos = pTrasnform->Get_State(STATE::POSITION);
		if(m_eRoomParticle == PARTICLE_TYPE::RAIN)
			m_pGameInstance->Play(ENUM_CLASS(PARTICLE_TYPE::RAIN), _float3(vPos.x, 15.f, vPos.z));
		else if(m_eRoomParticle == PARTICLE_TYPE::FIRE)
			m_pGameInstance->Play(ENUM_CLASS(PARTICLE_TYPE::FIRE), _float3(vPos.x, 1.f, vPos.z));
		else if(m_eRoomParticle == PARTICLE_TYPE::DUST)
			m_pGameInstance->Play(ENUM_CLASS(PARTICLE_TYPE::DUST), _float3(vPos.x, 1.f, vPos.z));
		else if (m_eRoomParticle == PARTICLE_TYPE::LIGHT)
			m_pGameInstance->Play(ENUM_CLASS(PARTICLE_TYPE::LIGHT), _float3(vPos.x, 1.f, vPos.z));

		m_bParticleRoom = true;
	}
	m_pGameInstance->Set_Next(true);
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

	m_pGameInstance->Set_Next(true);
	m_pGameInstance->Clear_Colliders();

	m_bIsActive = false;
	m_bParticleRoom = false;
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
