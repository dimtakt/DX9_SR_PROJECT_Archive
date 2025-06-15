#include "GameEffect.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
#include "Monster.h"
#include "Player.h"
#include "Erma.h"
#include "Effect_Factory.h"
CGameEffect::CGameEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect(pGraphic_Device)
{

}

CGameEffect::CGameEffect(const CGameEffect& Prototype)
	: CEffect(Prototype)
{
}

HRESULT CGameEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameEffect::Initialize(void* pArg)
{
	// 이펙트 태그 반영


	EFFECT_DESC* pDesc = reinterpret_cast<EFFECT_DESC*>(pArg);
	m_strEffectTag = pDesc->strEffectTag;
	m_isFlippedX = pDesc->isFlippedX;
	m_eObjType = pDesc->eType;
	m_fDeltaAngle = pDesc->fDeltaAngle;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	

	// 위치 반영

	_float4x4 matTransform;

	if (pDesc->isMatWorld)			// 월드 행렬이 주어진 경우
		matTransform = pDesc->matWorld;
	else							// 월드 행렬이 주어지지 않은 경우
	{
		_float4x4 matScale, matRot, matTrans;

		_float3 vPos = pDesc->vPos;
		D3DXQUATERNION qRot = pDesc->qRot;
		_float3 vScale = pDesc->vScale;

		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationQuaternion(&matRot, &qRot);
		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
		
		matTransform = matScale * matRot * matTrans;
	}
	//std::cout << "[GameEffect::Initialize] EffectPos : " << matTransform.m[3][0] << ", " << matTransform.m[3][1] << ", " << matTransform.m[3][2] << std::endl;


	for (int i = 0; i < 3; i++)
		m_pTransformCom->Set_State(STATE(i), *reinterpret_cast<_float3*>(&matTransform.m[i]));
	m_pTransformCom->Set_State(STATE::POSITION, *reinterpret_cast<_float3*>(&matTransform.m[3]));

	m_pFollowTransformCom = nullptr;
	if (pDesc->pFollowTransformCom != nullptr)
	{
		m_pFollowTransformCom = pDesc->pFollowTransformCom;
		_float3 vTargetPos = pDesc->pFollowTransformCom->Get_State(STATE::POSITION);
		m_vFollowOffset = vTargetPos - m_pTransformCom->Get_State(STATE::POSITION);
	}

	m_vEffectStartPos = {	pDesc->matOriginWorld.m[3][0], 
							pDesc->matOriginWorld.m[3][1],
							pDesc->matOriginWorld.m[3][2] };
	m_vThrownDir = pDesc->vThrownDir;
	m_fThrownPower = pDesc->fThrownPower;
	m_fLifeTimeSec = pDesc->fLifeTimeSec;

	m_iStackedFrame = 0;

	Ready_Collision();

	return S_OK;
}

void CGameEffect::Priority_Update(_float fTimeDelta)
{
	if (m_pAnimatorCom->Get_IsLastFrame() &&
		m_iStackedFrame >= m_fLifeTimeSec * 60.f)
	{
		if (m_strEffectTag == TEXT("Prototype_Component_Texture_IceBolt"))
		{
			m_bDead = true;

			_float4x4 matOffset{};
			_float4x4 matTemp{};
			D3DXMatrixIdentity(&matOffset);
			matTemp = *m_pTransformCom->Get_WorldMatrix();

			CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::PLAYER_SKILL,
				L"Prototype_Component_Texture_Effect_IceBolt",
				*m_pTransformCom->Get_WorldMatrix(),         
				matOffset,          
				false);
			m_pGameInstance->Set_Next(true);
		}
		else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Meteor"))
		{
			m_bDead = true;

			_float4x4 matOffset{};
			_float4x4 matTemp{};
			D3DXMatrixIdentity(&matOffset);
			m_pTransformCom->Scaling(4.5f, 2.f, 1.5f);
			matTemp = *m_pTransformCom->Get_WorldMatrix();

			CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::PLAYER_SKILL,
				L"Prototype_Component_Texture_Effect_Meteor",
				*m_pTransformCom->Get_WorldMatrix(),
				matOffset,
				false);
			m_pGameInstance->Set_Next(true);
		}
		else 
		{
			m_bDead = true;
			m_pGameInstance->Set_Next(true);
		}
		
	}
		
}

void CGameEffect::Update(_float fTimeDelta)
{
	

	// fDeltaAngle, fTimeDelta 사용하여 초마다 해당 각도씩 돌아가게
	if (m_fDeltaAngle != 0)
	{
		// 오프셋 적용 전 최초 위치(즉, 이펙트를 소환하는 객체의 초기위치)
		// 를 기준으로, 회전값 시간에 따라 변화
#pragma region Turning Effect
		_float3 vTurnAxis = { 0, 1, 0 };
		// 원점으로 중심축 이동
		_float4x4 matToOrigin;
		D3DXMatrixTranslation(&matToOrigin,
			-m_vEffectStartPos.x,
			-m_vEffectStartPos.y,
			-m_vEffectStartPos.z);
		// 축 기준 회전행렬
		_float4x4 matRot;
		D3DXMatrixRotationAxis(&matRot, &vTurnAxis, m_fDeltaAngle * fTimeDelta);
		// 다시 제자리로
		_float4x4 matFromOrigin;
		D3DXMatrixTranslation(&matFromOrigin,
			m_vEffectStartPos.x,
			m_vEffectStartPos.y,
			m_vEffectStartPos.z);
		// 다 합치기
		_float4x4 matRotationTotal = matToOrigin * matRot * matFromOrigin;
		// 반영
		_float4x4 matEffectPos = *m_pTransformCom->Get_WorldMatrix();
		_float4x4 matResult = matEffectPos * matRotationTotal;
		for (int i = 0; i < 3; i++)
			m_pTransformCom->Set_State(STATE(i), *reinterpret_cast<_float3*>(&matResult.m[i]));
		m_pTransformCom->Set_State(STATE::POSITION, *reinterpret_cast<_float3*>(&matResult.m[3]));
#pragma endregion
	}



	// 생성 위치 기준 해당 객체 따라가도록
	if (m_pFollowTransformCom != nullptr)
	{
		// Initialize 시 타겟과 이펙트 생성 위치의 오프셋을 저장 후
		// 매 업데이트마다 오프셋을 적용해주는 식
		_float3 vTargetPos = m_pFollowTransformCom->Get_State(STATE::POSITION);
		// 타겟에서 m_vFollowOffset 을 뺄셈하면 의도한 이펙트의 좌표가 됨
		_float3 vResultPos = vTargetPos - m_vFollowOffset;
		//std::cout << "vResultPos = " << vResultPos.x << ", " << vResultPos.y << ", " << vResultPos.z << std::endl;
		m_pTransformCom->Set_State(STATE::POSITION, vResultPos);
	}

	// 해당 방향으로 날아가도록
	if (m_fThrownPower != 0)
	{
		_float3 vEffectPos = m_pTransformCom->Get_State(STATE::POSITION);
		_float3 vThrownDir = m_vThrownDir * m_fThrownPower * fTimeDelta;
		vEffectPos += vThrownDir;
		m_pTransformCom->Set_State(STATE::POSITION, vEffectPos);
	}


	m_iStackedFrame++;
}

void CGameEffect::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CGameEffect::Render()
{
	SetUp_RenderState();


	_float fPointY = 0.f;       // 교차 평면의 기준이 될 Y값
	_float3 vRayPoint = {};     // fPointY 값 기준 마우스 Ray와 교차하는 좌표
	m_pGameInstance->Get_IntersectAtY(fPointY, vRayPoint);

	_float3 vPlayerPos = {};    // 플레이어 좌표
	vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);


	m_pTransformCom->Bind_Matrix();

	m_pAnimatorCom->Update_State();

	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();



	Reset_RenderState();
	return S_OK;
}

HRESULT CGameEffect::Ready_Components()
{
	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), m_strEffectTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For Com_Animator */
	CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
	StartAnimStateDesc.strTimerTag = L"Animator_Effect_Main";   // 해당 애니메이터가 타이머에서 사용할 태그 key값

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
		TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
		return E_FAIL; 

	m_pAnimatorCom->Add_State(L"Effect", { m_pTextureCom, 5, false });

	return S_OK;
}

void CGameEffect::Ready_Collision()
{

	if (m_eObjType == GAMEOBJ_TYPE::MONSTER_EFFECT || m_eObjType == GAMEOBJ_TYPE::PLAYER_EFFECT || m_eObjType == GAMEOBJ_TYPE::PLAYER_SKILL)
	{

		CCollider_OBB::OBB_DESC tColliderDesc;
		tColliderDesc.pOwner = this;
		tColliderDesc.pTransform = m_pTransformCom;
		tColliderDesc.eType = m_eObjType;
		
		if (m_eObjType == GAMEOBJ_TYPE::PLAYER_EFFECT)
		{
			//플레이어 공격 관련 이펙트
			if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Blade0_Swing1"))
			{
				tColliderDesc.vScale = _float3(0.8f, 0.7f, 0.9f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Blade0_Swing0"))
			{
				tColliderDesc.vScale = _float3(0.8f, 0.7f, 0.9f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Blade0_NFury"))
			{
				tColliderDesc.vScale = _float3(0.8f, 0.7f, 0.9f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Blade0_NFury_Back"))
			{
				tColliderDesc.vScale = _float3(0.8f, 0.7f, 0.9f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Blade0_Parry"))
			{
				tColliderDesc.vScale = _float3(0.8f, 0.7f, 0.9f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Planet_Bullet_Cycle"))
			{
				tColliderDesc.vScale = _float3(0.8f, 0.7f, 0.9f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Texture_FrozenHammer_Shoot"))
			{
				tColliderDesc.vScale = _float3(0.8f, 0.7f, 0.9f);
			}
		}
		else if (m_eObjType == GAMEOBJ_TYPE::PLAYER_SKILL)
		{
			if (m_strEffectTag == TEXT("Prototype_Component_Texture_IceBolt"))
			{
				tColliderDesc.vScale = _float3(0.8f, 0.7f, 0.9f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Meteor"))
			{
				tColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f); //임의 크기로 일단 설정해둠 추후 테스트 해보고자 함.
			}
		}
		else {
			// 레이저 고스트
			if (m_strEffectTag == TEXT("Prototype_Component_Texture_LaserGhost_D_Effect_Laser_Progress"))
			{
				tColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Texture_LaserGhost_D_Effect_Laser_End"))
			{
				tColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
			}
			// 두더지
			else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Mole_A_Effect_Swing"))
			{
				tColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
			}
			// 돼지
			else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Oink_A_Effect_Swing"))
			{
				tColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Oink_A_Effect_SpinSwing"))
			{
				tColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
			}
			// 에르마
			else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_BigGolem_LaserStart"))
			{
				tColliderDesc.vScale = _float3(0.5f, 1.f, 0.5f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_BigGolem_LaserProgress"))
			{
				tColliderDesc.vScale = _float3(0.5f, 1.f, 0.5f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_BigGolem_Stmap_FX"))
			{
				tColliderDesc.vScale = _float3(0.5f, 1.f, 0.5f);
			}

			// 추후 처리필요 탄막 충돌체 가 안됨..
			else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_Bullet"))
			{
				tColliderDesc.vScale = _float3(0.2f, 0.2f, 0.2f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_Missile_Lower"))
			{
				tColliderDesc.vScale = _float3(0.7f, 1.f, 0.7f);
			}
			else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_Missile_Lower_Light"))
			{
				tColliderDesc.vScale = _float3(0.7f, 1.f, 0.7f);
			}
		}

		CCollider_OBB* pCol = dynamic_cast<CCollider_OBB*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"), &tColliderDesc));
		m_pGameInstance->Add_Collider(pCol);
	}
	
}

void CGameEffect::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CGameEffect::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//m_pGraphic_Device->SetTexture(0, NULL);
}

void CGameEffect::OnCollision(CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
		return;
	if (m_eObjType == GAMEOBJ_TYPE::PLAYER_EFFECT || m_eObjType == GAMEOBJ_TYPE::PLAYER_SKILL) {
		if (pGameObject->Get_ObjType() == GAMEOBJ_TYPE::MONSTER)
		{
			CMonster* pMonster = dynamic_cast<CMonster*>(pGameObject);
			
			if (pMonster->Get_MonsterType() == MONSTER_TYPE::ERMA)
			{
				CAnimator* pAni = dynamic_cast<CAnimator*>((dynamic_cast<CErma*>(pGameObject)->Find_Component(TEXT("Com_Animator"))));

				if (pAni->Get_CurStateTag() == L"Idle" || pAni->Get_CurStateTag() == L"Move" || pAni->Get_CurStateTag() == L"AirBorne" ||
					pAni->Get_CurStateTag() == L"Enter_Progress" || pAni->Get_CurStateTag() == L"Enter_End" || pAni->Get_CurStateTag() == L"Entered")
					return;
			}

			if (!pMonster->Get_IsHit() && pMonster->Get_Summoned() && !pMonster->Get_Dying())
			{
				m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_PLAYER));
				m_pGameInstance->PlaySoundW(L"hitSword02.wav", ENUM_CLASS(CHANNELID::SOUND_PLAYER), g_fEFFECTVolume - 0.6f);
				//플레이어 공격 관련 이펙트
				if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Blade0_Swing1"))
				{
					pMonster->Set_Damage(-(CStat_Manager::GetInstance()->Get_Player_Damage(DAMAGE::NORMAL)));
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Blade0_Swing0"))
				{
					pMonster->Set_Damage(-(CStat_Manager::GetInstance()->Get_Player_Damage(DAMAGE::NORMAL)));
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Blade0_NFury"))
				{
					pMonster->Set_Damage(-(CStat_Manager::GetInstance()->Get_Player_Damage(DAMAGE::SPECIAL)));
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Blade0_NFury_Back"))
				{
					pMonster->Set_Damage(-(CStat_Manager::GetInstance()->Get_Player_Damage(DAMAGE::SPECIAL)));
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Blade0_Parry"))
				{
					pMonster->Set_Damage(-(CStat_Manager::GetInstance()->Get_Player_Damage(DAMAGE::SPECIAL)));
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Planet_Bullet_Cycle"))
				{
					pMonster->Set_Damage(-(CStat_Manager::GetInstance()->Get_Player_Damage(DAMAGE::PLANET)));
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_FrozenHammer_Shoot"))
				{
					pMonster->Set_Damage(-(CStat_Manager::GetInstance()->Get_Player_Damage(DAMAGE::FROZENHAMMER)));
					m_bDead = true;
					_float4x4 tempMat{};
					D3DXMatrixIdentity(&tempMat);
					CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Texture_FrozenHammer_Hit",
						*m_pTransformCom->Get_WorldMatrix(), tempMat, true);
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_IceBolt"))
				{
					pMonster->Set_Damage(-(CStat_Manager::GetInstance()->Get_Player_Damage(DAMAGE::PLANET)));
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Meteor"))
				{
					pMonster->Set_Damage(-(CStat_Manager::GetInstance()->Get_Player_Damage(DAMAGE::FROZENHAMMER)));
				}
				pMonster->Set_IsHit(TRUE);
			}
		}
		else if (pGameObject->Get_ObjType() == GAMEOBJ_TYPE::MONSTER_EFFECT)
		{
			if (m_strEffectTag == TEXT("Prototype_Component_Texture_Effect_Blade0_Parry"))
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Player")));
				pPlayer->Ready_Parry();
			}
		}
	}
	else if (m_eObjType == GAMEOBJ_TYPE::MONSTER_EFFECT)
	{
		if (pGameObject->Get_ObjType() == GAMEOBJ_TYPE::PLAYER)
		{
			_int iDamage{};
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);

			if (!pPlayer->Get_IsHit()&& !pPlayer->Get_IsGodMode())
			{
				m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER));
				m_pGameInstance->PlaySoundW(L"HitPlayer.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER), g_fEFFECTVolume - 0.8f);
				if (m_strEffectTag == TEXT("Prototype_Component_Texture_LaserGhost_D_Effect_Laser_Progress"))
				{
					iDamage = CStat_Manager::GetInstance()->Get_Monster_Damage(8.f);
					CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, float(iDamage) * -1 );
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_LaserGhost_D_Effect_Laser_End"))
				{
					iDamage = CStat_Manager::GetInstance()->Get_Monster_Damage(1.f);
					CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, float(iDamage) * -1);
				}
				// 두더지
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Mole_A_Effect_Swing"))
				{
					iDamage = CStat_Manager::GetInstance()->Get_Monster_Damage(4.f);
					CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, float(iDamage) * -1);
				}
				// 돼지
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Oink_A_Effect_Swing"))
				{
					iDamage = CStat_Manager::GetInstance()->Get_Monster_Damage(4.f);
					CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, float(iDamage) * -1);
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Texture_Oink_A_Effect_SpinSwing"))
				{
					iDamage = CStat_Manager::GetInstance()->Get_Monster_Damage(6.f);
					CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, float(iDamage) * -1);
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_BigGolem_LaserStart"))
				{
					iDamage = CStat_Manager::GetInstance()->Get_Monster_Damage(10.f);
					CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, float(iDamage) * -1);
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_BigGolem_LaserProgress"))
				{
					iDamage = CStat_Manager::GetInstance()->Get_Monster_Damage(10.f);
					CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, float(iDamage) * -1);
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_BigGolem_Stmap_FX"))
				{
					iDamage = CStat_Manager::GetInstance()->Get_Monster_Damage(13.f);
					CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, float(iDamage) * -1);
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_Bullet"))
				{
					iDamage = CStat_Manager::GetInstance()->Get_Monster_Damage(8.f);
					CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, float(iDamage) * -1);
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_Missile_Lower"))
				{
					iDamage = CStat_Manager::GetInstance()->Get_Monster_Damage(8.f);
					CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, float(iDamage) * -1);
				}
				else if (m_strEffectTag == TEXT("Prototype_Component_Boss_Erma_Missile_Lower_Light"))
				{
					iDamage = CStat_Manager::GetInstance()->Get_Monster_Damage(10.f);
					CStat_Manager::GetInstance()->Cal_Stats(STAT_INFO::CULHP, float(iDamage) * -1);
				}
				pPlayer->Hit(iDamage);
			}

		}
		
	}
	
}


CGameObject* CGameEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameEffect* pInstance = new CGameEffect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CGameEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGameEffect::Clone(void* pArg)
{
	CGameEffect* pInstance = new CGameEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CGameEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameEffect::Free()
{
	m_pGameInstance->Remove_Collider_ByOwner(this);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pAnimatorCom);

	__super::Free();
}
