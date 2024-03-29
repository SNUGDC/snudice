//////////////////////////////////////////////////////////////////////////
/// 10.09.21 01:13 
/// class gTile, gMap
/// Coder : 장난죄송
/// tileContainer에는 더 많은 함수들이 있음.
/// 옛날에 m_xPos의 역활을 이제는 gMap class의 m_nAbsDrawlineX 이 맡게 되었다.
//////////////////////////////////////////////////////////////////////////

#pragma once
#include <windows.h> // 중복 include (first case) 성공?
#include "networkconst.h"
#include "Image.h"

#define MAPMAXSTRBUFFER		128

enum ETILETYPE
{
	TY_NONE	= -1,
	TY_STAMINA,		// 식당
	TY_ITEM,		// 아이템
	//TY_GRASS,
	TY_DRINK,		// 녹두
	TY_BUS,			// 셔틀
	TY_MAINGATE,	// 정문
	TY_MOUNTAIN,	// 관악산
	TY_CLASS,		// 수업
	TY_END,
};

class gTile
{
public:
	ETILETYPE		tileType;
	POINT			prevTile;
	POINT			ptPos;
	POINT			nextTile;

	int				flag1;
	int				flag2;
	int				flag3;
	char			college[MAPMAXSTRBUFFER];
	char			building[MAPMAXSTRBUFFER];
	char			subject[MAPMAXSTRBUFFER];
public:
	void			init(int xo, int yo);
};

#define MAXSAVOR_N	100

#define LEFTX		25
#define MIDDLEX		52
#define HALFX		52 // 25+ 52/2 = 52 
#define FULLX		102
#define WIDEX		154
#define HALFY		40
#define FULLY		80

#define LINEX		29
#define LINEY		19


class gMap
{
public:
	static gMap *GetIF();	// 1

public:
	HANDLE	hFile;
	DWORD	dw;
	gImage	m_ImgTile[9];
//	gImage	m_ImgTileBack;
	gImage	m_ImgSmallTile;

	gImage	m_ImgMapBG;

	gImage	m_ImgCurSmall;
	gImage	m_ImgCurBig;

	int		count; // 아마 이것이 tile의 갯수갰지.

	//bool	m_minimapOn;
	gTile	bmpKindSavor[MAXSAVOR_N];	// 임시적으로 Load시 필요한 타일들 (실제 게임에서는 사용되지 않음)
	gTile	tileMap[LINEX*LINEY];		// 실제 타일들

	// 시작점 - (정문의 상대좌표) (예컨대, (5,14))
	int		m_xInitSpacePos;
	int		m_yInitSpacePos;

	// 좌표 관련 변수들 
	// m_xSpacePos , m_ySpacePos 는 현재 좌표를
	// m_Next_xSpacePos, m_Next_ySpacePos 는 진행하려는 좌표를 의미한다.
	int		m_xSpacePos;
	int		m_ySpacePos;
	int		m_Next_xSpacePos;
	int		m_Next_ySpacePos;

//private:
	// 좌표 관련 변수들 네개와 frame값을 이용하여 현재 뿌리는 화면의 좌표를 지정한다.
	int		m_nAbsDrawlineX;
	int		m_nAbsDrawlineY;


public:
	void	Edge();

	bool	Setup();			// setup
	void	Release();	

	bool	Restore();

	void	Clear();


	void	MainLoop();
	void	Draw();				// 그리는 함수

	// Draw Position set colony
	void	posSetter(int xSpacePos, int ySpacePos);
	void	posSpacor(bool isNokdu=false);		// space를 누르면 해당 위치로 이동하고, 캐릭터의 이동을 잡아주는 등의 함수
	void	posSpacor(int xNextSpacePos, int yNextSpacePos); // 다음칸이 아닌 특정 칸으로 이동.
	void	posMover();
	void	posMover(int frame,int frameN);
	void	posStoper();		// 항상 posSpacor -> posMover -> posStoper 순으로 써야함

	// abs,con function colony
	POINT	conToAbs(POINT ij);
	POINT	absToCon(POINT ij); 

	// 상우의 막장 돌진 함수군
	//int		distance(int mapA,int mapB);	
	// 거리를 잰다. mapA는 시작점, mapB는 도착점
	// return 값은 양수일경우 녹두를 거치지 않는 경우, 음수일경우 녹두를 뺑뺑이하는 경우
	// 값은 움직여야 하는 값을 의미한다. 1은 한칸, 0은 0칸, -3은 3칸인데 녹두를 만나면 뺑뺑이.

	void	DrawHexagon(int x0,int y0,int n, int playerIdx, bool boolo=false);	
	// 특정 좌표에 n의 비율로 나누어서 육각형들을 뿌려줌.
	// boolo 필요 없을 것 같은데, boolo setting해두면 막혔을 때, 타일이 쪼그라들음

	int		PositionFor_gPC();
	// 지금 gMap의 nPos 와 nPosNext 의 관계를 비교해 봤을 때, 
	// Player가 찍어야 하는 발은 몇번째 발일까를 리턴하는 함수
	// 즉, 이 함수는 gMap의 함수이지만 gPlayerContainer에서(만) 사용되도록 고안되었다.

	void	DrawSubmit(int x0, int y0, int n, int subjectIndex = -1, int frameOn = 0); // frameOn = false 가 기본 setting
	// subjectIndex 는 선택된 과목 만약, 없다면 -1을 넣어주자.
	// frameOn은 subjectIndex > 0 일 때, 1이면 깜빡이기.


	void	DrawMap();			// 전체맵
	void	DrawMapCursor(int nSizeRate, int posX, int posY);	// 전체맵 플레이어 위치

	void	DrawMinimap(int x0, int y0, int n);
	void	DrawMiniMapCursor(int nSizeRate, int posX, int posY);

	int		viewabsToCon(POINT ij);
	// 마우스 좌표 받아서, nPos return
	POINT	ConToViewabs(int nPos);

	int		Destination(int mapA, int spacor);

	float	GetPosRateX();		// m_nabsX / mapsizew;
	float	GetPosRateY();

	void	SetPosByRateX(float ratex);			// rate기반으로 맵 위치 세팅
	void	SetPosByRateY(float ratey);			// rate기반으로 맵 위치 세팅

private:
	// Setup의 떨거지 (subfunction)
	void	Load();				// 파일을 불러오는 함수
	void	LoadFileToBKS();	// Load의 떨거지들
	void	LoadBKSToTM();

	// Draw의 떨거지
	
	void	DrawHexagonOne(int x0,int y0,int i,int j,int n, int playerIdx, bool boolo,int type=0);	// 0은 기본 1은 선택된 표시
	void	DrawHexagonSubmitOne(int x0, int y0, int i, int j, int n, int classtype, int statetype);
	void	DrawHexagonMinimapOne(int x0, int y0, int i, int j, int n, int classtype, int statetype);
};
