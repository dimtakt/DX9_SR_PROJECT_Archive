#pragma once

#include "Base.h"

BEGIN(Engine)
class CTransform;

class ENGINE_DLL CAnimation : public CBase
{
	// 각각의 프레임마다 위치가 어떻게 될 것인가의 클립 정보가 필요
	// 클립 정보는 월드 매트릭스 정보를 로컬에 담고 있는 것 만으로 될듯

	// 그렇다면.. 필요한 변수?
	// 컨테이너로, 숫자 인덱스에 월드 매트릭스를 담는 것 필요
public:
	typedef struct tagKeyFrame
	{
		_int iKeyFrame;
		_float4x4 matTransform;
	} KEYFRAME;

protected:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	virtual HRESULT Initialize();
	void Insert_KeyFrames(_int iFrame, const _float3& vScale, const _float4& qRot, const _float3& vPos);
	void Insert_KeyFrames(_int iFrame, _float4x4 matTransform);
	//void Update_KeyFrames();

	void Set_TargetTransform(CTransform* pTargetTransform){ 
		m_pTargetTransform = pTargetTransform; 
	}
	_bool Get_isEnd() { return m_isEnd; }
	_int Get_iMaxFrame() { return m_iMaxFrame; }
	KEYFRAME Get_CurKeyFrame(_int keyframe) { return m_keyFrames[keyframe]; }

	virtual void Free();

protected:
	//void Lerp_Frames();

protected:
	CTransform* m_pTargetTransform;	// 기준이 될 좌표 타겟
	CTransform* m_pLocalTransform;	// 실제로 변환이 적용될 객체
	std::vector<KEYFRAME> m_keyFrames = {};	// keyframe, 
	
	_int m_iCurFrame = 0;
	_int m_iMaxFrame;
	
	_bool m_isLoop = false;
	_bool m_isEnd = false;
};

END