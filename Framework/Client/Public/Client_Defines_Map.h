#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
namespace Client
{
	//필요한 타입은 추가 후 말해주세요~
	enum class MAP_EVENT { MAP_SHOP, MAP_GOLD, MAP_EXP, MAP_SLATE, 
							MAP_ARTEFACT, MAP_TREE, MAP_END };
	enum class MAP_BOSS { MAP_MIDDLE_BOSS, MAP_MAIN_BOSS, MAP_BOSS_END };

	struct MapData
	{
	public:
		_uint				m_iMapID;		
		LEVEL				m_eMapIndex;	//맵 스테이지 레벨 인덱스 (0번 부터 시작)
		_uint				m_iLine;		//맵 열 번호 (같은 열은 최대 2개)
											//플레이어 시작 지점: 0, 보스방: 4
											//똑같은 열 번호 입력 시 옆으로 추가 됨 									

		_uint				m_iImage;		//맵 이미지 0번 ~ 4번까지 있음 

		vector<MAP_EVENT>	m_vecEvent;		//맵 발생 이벤트 (최대 2개)
											//넣은 순서대로 이미지 출력됨
		
		MAP_BOSS			m_eBossType;

	public:
		MapData(_uint iMapID, LEVEL eMapIndex, _uint iLine, _uint iImage, vector<MAP_EVENT> vecType, MAP_BOSS eBossType) :
			m_iMapID(iMapID), m_eMapIndex(eMapIndex), m_iLine(iLine), m_iImage(iImage), m_vecEvent(vecType), m_eBossType(eBossType) { }
	};

	const vector<MapData> g_MapDB
	{
		/* 맵 ID, 맵 레벨 인덱스(스테이지 이넘), 지도 열 번호, 지도에 표시될 맵 이미지, 맵에 발생할 이벤트  */

		//테스트용 데이터
		MapData(0, LEVEL::LEVEL_STAGE1,	0,	0, {MAP_EVENT::MAP_END },MAP_BOSS::MAP_BOSS_END),

		MapData(1, LEVEL::LEVEL_STAGE2,	1,	0, {MAP_EVENT::MAP_SHOP }, MAP_BOSS::MAP_BOSS_END),
		MapData(2, LEVEL::LEVEL_STAGE2,	1,	1, {MAP_EVENT::MAP_SHOP,		MAP_EVENT::MAP_GOLD }, MAP_BOSS::MAP_BOSS_END),

		MapData(3, LEVEL::LEVEL_LOGO,	2,	2, {MAP_EVENT::MAP_SHOP,		MAP_EVENT::MAP_GOLD }, MAP_BOSS::MAP_BOSS_END),
		MapData(4, LEVEL::LEVEL_STAGE3,	2,	1, {MAP_EVENT::MAP_ARTEFACT,	MAP_EVENT::MAP_SLATE }, MAP_BOSS::MAP_BOSS_END),

		MapData(5, LEVEL::LEVEL_LOGO,	3,	4, {MAP_EVENT::MAP_SHOP,		MAP_EVENT::MAP_GOLD }, MAP_BOSS::MAP_BOSS_END),
		MapData(6, LEVEL::LEVEL_LOGO,	3,	3, {MAP_EVENT::MAP_TREE,		MAP_EVENT::MAP_GOLD }, MAP_BOSS::MAP_MIDDLE_BOSS),
		MapData(7, LEVEL::LEVEL_LOGO,	4,	3, {MAP_EVENT::MAP_END}, MAP_BOSS::MAP_MAIN_BOSS),
	};

}