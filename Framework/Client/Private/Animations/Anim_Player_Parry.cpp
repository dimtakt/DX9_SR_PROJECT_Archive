#include "Animations/Anim_Player_Parry.h"

CAnim_Player_Parry::CAnim_Player_Parry()
{
}

HRESULT CAnim_Player_Parry::Initialize()
{
	// Setting : 변환용 행렬 선언 및 초기화
	_float4x4 matTrans, matRot, matScale, matResult;
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matResult);

	_float3 vecTrans = { 0, 0, 0 };
	_float	fTurnX = 0,
		fTurnY = 0,
		fTurnZ = 0;
	D3DXQUATERNION quatRot = { 0, 0, 0, 1 };
	_float3 vecScale = { 1, 1, 1 };
	_int iKeyFrame = 0;
	// End Setting *********


	// desmos : https://www.desmos.com/calculator/hqkn67hpjk
	// keyStart (0~17) *********
	for (size_t i = 0; i < 18; i++)
	{
		// transform
		vecTrans = {
			static_cast<_float>(-0.5 * cosf( 0.4 * iKeyFrame - 0.7 ) + 0.5),
			static_cast<_float>(0.6 * sinf(- 0.35 * iKeyFrame)),
			0
		};

		// rotation
		_float3 vAxis = { 0, 0, 1 };
		D3DXQuaternionRotationAxis(&quatRot, &vAxis, D3DXToRadian(130 -iKeyFrame * -15));

		// scale
		vecScale = {
			float(7) / 18,
			float(15) / 19,
			1
		};

		// 반영
		Insert_KeyFrames(iKeyFrame, vecScale, quatRot, vecTrans);
		iKeyFrame++;
	}
	// End key *********


	// 최대 프레임 수
	m_iMaxFrame = iKeyFrame;
	return S_OK;
}

CAnim_Player_Parry* CAnim_Player_Parry::Create()
{
	CAnim_Player_Parry* pInstance = new CAnim_Player_Parry();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CAnim_Player_Parry"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnim_Player_Parry::Free()
{
	__super::Free();
}

