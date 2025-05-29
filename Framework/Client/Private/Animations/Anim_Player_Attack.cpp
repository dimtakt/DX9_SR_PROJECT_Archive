#include "Animations/Anim_Player_Attack.h"

CAnim_Player_Attack::CAnim_Player_Attack()
{
}

HRESULT CAnim_Player_Attack::Initialize()
{
	// Setting : 변환용 행렬 선언 및 초기화
	_float4x4 matTrans, matRot, matScale, matResult;
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matResult);

	_float3 vecTrans	= {0, 0, 0};
	_float	fTurnX = 0,
			fTurnY = 0,
			fTurnZ = 0;
	D3DXQUATERNION quatRot		= {0, 0, 0, 1};
	_float3 vecScale	= {1, 1, 1};
	_int iKeyFrame = 0;
	// End Setting *********


	// keyStart (0~7) *********
	for (size_t i = 0; i < 8; i++)
	{
		// transform
		vecTrans = {
			static_cast<_float>(-0.1 * pow((iKeyFrame - 4), 2) + 1.8),
			//static_cast<_float>((iKeyFrame - 4) * -0.05),
			static_cast<_float>(sinf(iKeyFrame / 1.5) * 0.5),
			0
		};
		
		// rotation
		_float3 vAxis = { 0, 0, 1 };
		D3DXQuaternionRotationAxis(&quatRot, &vAxis, D3DXToRadian(iKeyFrame * -20));
		
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

CAnim_Player_Attack* CAnim_Player_Attack::Create()
{
	CAnim_Player_Attack* pInstance = new CAnim_Player_Attack();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CAnim_Player_Attack"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnim_Player_Attack::Free()
{
	__super::Free();
}

