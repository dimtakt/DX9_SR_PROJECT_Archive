#include "Animator.h"
#include "GameInstance.h" // 이래도 되나

CAnimator::CAnimator(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CAnimator::CAnimator(const CAnimator& Prototype)
	: CComponent{ Prototype }
{
}

HRESULT CAnimator::Initialize_Prototype()
{
	std::cout << "[CAnimator::Initialize_Prototype] Prototype Initialized!" << std::endl;

	return S_OK;
}

HRESULT CAnimator::Initialize(void* pArg)
{
	if (nullptr == pArg)
	{
		std::cout << "[CAnimator::Initialize] Initialize Failed. Argument was nullptr." << std::endl;
		return E_FAIL;
	}

	// 타이머 태그 Arg로부터 가져와서 반영
	ANIMSTATE_DESC* pDesc = static_cast<ANIMSTATE_DESC*>(pArg);
	m_strTimerTag = pDesc->strTimerTag;

	// 타이머 생성
	m_pGameInstance->Compute_TimeDelta(m_strTimerTag);
	m_fElapsedTimesCheck = 0;
	m_fElapsedTimes = 0;

	// 애니메이션 반영할 Transform(Child) 과 기준점(Parent)이 될 객체 초기 설정
	m_pParentTransform	= nullptr != pDesc->pParentTransform? pDesc->pParentTransform : nullptr;
	m_pChildTransform	= nullptr != pDesc->pChildTransform? pDesc->pChildTransform : nullptr;
	

	std::cout << "[CAnimator::Initialize] Initialized!" << std::endl;

	return S_OK;
}

void CAnimator::Update_State()
{
	CTexture* pCurTextureCom;
	
	if (m_pCurState->pTextureCom != nullptr)
	{
		pCurTextureCom = m_pCurState->pTextureCom;

		_uint iImageMaxIndex = pCurTextureCom->Get_NumTextures();
		_uint iImageCurIndex = m_iStackedFrames / m_pCurState->iFramePerImage;
		iImageCurIndex %= iImageMaxIndex;
		pCurTextureCom->Bind_Texture(iImageCurIndex);
	}

	if (m_pCurState->pAnimation != nullptr)
		Update_Keyframes();

	m_iStackedFrames++;
	m_fElapsedTimes += 1 / 60.f;
}

HRESULT CAnimator::Add_State(const _wstring strStateTag, ANIMSTATE _state)
{
	if (nullptr != Find_State(strStateTag))		// 중복 검사
	{
		std::wcout << "[CAnimator::Add_State] Adding State Failed. \"" << strStateTag << "\" state already exist." << std::endl;
		return E_FAIL;
	}
	
	if (_state.pTextureCom == nullptr)
		std::wcout << "[CAnimator::Add_State]  \"" << strStateTag << "\" State Added. but Texture Component was nullptr." << std::endl;

	m_pStates.emplace(strStateTag, _state);
	std::wcout << "[CAnimator::Add_State] \"" << strStateTag << "\" State Added." << std::endl;

	// 최초 삽입이면 해당 State 를 현재 State로 
	if (m_pCurState == nullptr)
	{
		m_pCurState = Find_State(strStateTag);
		m_strCurStateTag = strStateTag;
		std::wcout << "[CAnimator::Add_State] Start state set to \"" << strStateTag << "\"." << std::endl;
	}

	return S_OK;
}

_bool CAnimator::Change_State(const _wstring strStateTag, _bool isChangeCurFrame, _float fLoopTime)
{
	if (!(m_fElapsedTimes >= m_fElapsedTimesCheck))
		return false;

	// 이미 해당 State라면 return
	if (strStateTag == m_strCurStateTag)
		return false;

	ANIMSTATE* pTmpState = Find_State(strStateTag);

	// 즉시 전이가 가능한 State인 경우 전이,
	// 즉시 전이가 불가능한 State인 경우 마지막으로 전환된 지 충분한 시간이 지난 경우에만 전이

	if (nullptr == pTmpState)
	{
		std::wcout << "[CAnimator::Change_State] Failed to Change State. Can't find State : \"" << strStateTag << "\"." << std::endl;
		return false;
	}

	_uint iTextureMaxFrame;
	if (m_pCurState->pTextureCom != nullptr)
		iTextureMaxFrame = m_pCurState->pTextureCom->Get_NumTextures();
	else
		iTextureMaxFrame = 1;

	if (!(m_iStackedFrames / m_pCurState->iFramePerImage >= iTextureMaxFrame) &&	// 프레임이 충분히 지났는지
		!m_pCurState->isExitable)													// 도중 전이가 가능한지
		return false;
	


	m_pPrevState = m_pCurState;
	m_strPrevStateTag = m_strCurStateTag;

	m_pCurState = pTmpState;
	m_strCurStateTag = strStateTag;

	m_fElapsedTimes = 0;
	m_fElapsedTimesCheck = fLoopTime;

	std::wcout << "[CAnimator::Change_State] State Changed to \""<< strStateTag << "\"." << std::endl;

	// 프레임 순서 유지 불필요시에만 갱신
	if (isChangeCurFrame)
		m_iStackedFrames = 0;


	return true;
}

CAnimator::ANIMSTATE* CAnimator::Find_State(const _wstring& strStateTag)
{
	auto iter = m_pStates.find(strStateTag);

	if (iter == m_pStates.end())
		return nullptr;

	return &(iter->second);
}

void CAnimator::Update_Keyframes()
{
	// 현재 상태에 애니메이션이 없거나, 움직일 객체가 없으면 실행 X
	if (m_pCurState->pAnimation == nullptr ||
		m_pChildTransform == nullptr)
		return;

	// 사용할 애니메이션
	CAnimation* pAnim = m_pCurState->pAnimation;

	// 객체가 움직일 애니메이션의 기준점 행렬 설정. 없으면 원점 기준.
	_float4x4 matTrackTarget;
	if (m_pParentTransform != nullptr)
		matTrackTarget = *m_pParentTransform->Get_WorldMatrix();
	else
		D3DXMatrixIdentity(&matTrackTarget);
	
	_int iMaxFrame = pAnim->Get_iMaxFrame();
	if (iMaxFrame == 0) return;						// 빈 애니메이션이면 리턴 



	_int iCurFrame = m_iStackedFrames % iMaxFrame;	// 인덱스 넘기면 그냥 루프하게

	_float4x4 matLocal = pAnim->Get_CurKeyFrame(iCurFrame).matTransform; // 변환용 행렬 가져옴

	// 실제로 애니메이션의 움직임이 반영될 행렬 계산..
	_float4x4 matResult = matLocal * matTrackTarget;

	for (int i = 0; i < 3; i++)
		m_pChildTransform->Set_State(STATE(i), *reinterpret_cast<_float3*>(&matResult.m[i]));

	m_pChildTransform->Set_State(STATE::POSITION, *reinterpret_cast<_float3*>(&matResult.m[3]));

	//m_iCurFrame++;
	//if (m_isLoop && m_iCurFrame + 1 >= m_iMaxFrame)
	//	m_iCurFrame = 0;
	//if (!m_isLoop && m_iCurFrame + 1 >= m_iMaxFrame)
	//	m_isEnd = true;

	return;
}


CAnimator* CAnimator::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAnimator* pInstance = new CAnimator(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAnimator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CAnimator::Clone(void* pArg)
{
	CAnimator* pInstance = new CAnimator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to cloned : CAnimator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimator::Free()
{
	__super::Free();
}
