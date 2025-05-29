#include "Animations/Anim_Player_Idle.h"

CAnim_Player_Idle::CAnim_Player_Idle()
{
}

HRESULT CAnim_Player_Idle::Initialize()
{
	//// Setting : 변환용 행렬 선언 및 초기화
	//_float4x4 matTrans, matRot, matScale, matResult;
	//D3DXMatrixIdentity(&matTrans);
	//D3DXMatrixIdentity(&matRot);
	//D3DXMatrixIdentity(&matScale);
	//D3DXMatrixIdentity(&matResult);
	//
	//_float3 vecTrans	= {0, 0, 0};
	//_float	fTurnX = 0,
	//		fTurnY = 0,
	//		fTurnZ = 0;
	//D3DXQUATERNION quatRot		= {0, 0, 0, 1};
	//_float3 vecScale	= {1, 1, 1};
	_int iKeyFrame = 0;
	//// End Setting *********


	// keyStart *********
	
	
	
	// End key *********


	// 최대 프레임 수
	m_iMaxFrame = iKeyFrame;
	return S_OK;
}

CAnim_Player_Idle* CAnim_Player_Idle::Create()
{
	CAnim_Player_Idle* pInstance = new CAnim_Player_Idle();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CAnim_Player_Idle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnim_Player_Idle::Free()
{
	__super::Free();
}

