#include "Animation.h"
#include "Transform.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize()
{
	return S_OK;
}

void CAnimation::Insert_KeyFrames(_int iFrame, const _float3& vScale, const _float4& qRot, const _float3& vPos)
{
	_float4x4 matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixRotationQuaternion(&matRot, reinterpret_cast<const D3DXQUATERNION*>(&qRot));
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	_float4x4 matTransform = matScale * matRot * matTrans;

	m_keyFrames.push_back(KEYFRAME{ iFrame, matTransform });
}

void CAnimation::Insert_KeyFrames(_int iFrame, _float4x4 matTransform)
{
	m_keyFrames.push_back(KEYFRAME{ iFrame, matTransform });
}


//void CAnimation::Update_KeyFrames()
//{
//	_float4x4 matTrackTarget = *m_pTargetTransform->Get_WorldMatrix();
//	_float4x4 matLocal = m_keyFrames[m_iCurFrame].matTransform;
//
//	// °è»ê ÈÄ »ðÀÔ
//	_float4x4 matResult = matLocal * matTrackTarget;;
//
//	for (int i = 0; i < 3; i++)
//		m_pLocalTransform->Set_State(STATE(i), *reinterpret_cast<_float3*>(&matResult.m[i]));
//
//	m_pLocalTransform->Set_State(STATE::POSITION, *reinterpret_cast<_float3*>(&matResult.m[3]));
//
//
//	m_iCurFrame++;
//
//	if (m_isLoop && m_iCurFrame + 1 >= m_iMaxFrame)
//		m_iCurFrame = 0;
//	
//	if (!m_isLoop && m_iCurFrame + 1 >= m_iMaxFrame)
//		m_isEnd = true;
//
//	return;
//}

//void CAnimation::Lerp_Frames()
//{
//	
//}

void CAnimation::Free()
{
	__super::Free();
}