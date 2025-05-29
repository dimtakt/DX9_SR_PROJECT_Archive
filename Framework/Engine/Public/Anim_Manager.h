#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation;

class CAnim_Manager final : public CBase
{
private:
	CAnim_Manager();
	virtual ~CAnim_Manager() = default;

public:
	HRESULT Insert_Animation(const wstring& strAnimTag, CAnimation* anim);
	CAnimation* Find_Animation(const wstring& strAnimTag);

private:
	map<wstring, CAnimation*> m_mapAnim = {};

public:
	static CAnim_Manager* Create();
	virtual void Free() override;

};

END