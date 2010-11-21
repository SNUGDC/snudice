#pragma once
#include "network.h"
#include "Charinfo.h"

#define MINIMUM_COUPLE_CONDITION 50
#define SAMECLASS_FAVORPOINT 10
#define SAMETILE_FAVORPOINT 20
#define CROSS_FAVORPOINT 10
#define LOVEINITPOINT 40

enum ItemUseState {
	IUS_NONE,
	IUS_NEXTTURN,
	IUS_INFOCHANGE,
	IUS_INVALIDSTATE,
};

enum MovePlayerState {
	MPS_MOVE,
	MPS_BUS,
	MPS_WARP,
};

enum CoupleState {
	CPS_NONE = -1,
	CPS_REJECT,
	CPS_ACCEPT,

	CPS_PROPOSE,
};

//수정, 호감도구조체
struct sFavor 
{
//	char *playerid[ROOMMAXPLAYER];	// 대략 필요 없을듯; RoomCore의 FindPlayerIndexInTheRoom를 이용한다.
	int		point[ROOMMAXPLAYER];	// -1 : 커플상태 , 즉 호감도가 오를리가 업ㅇ다
	int		lvTargetIndex;	//사랑을 받는 상대
	CoupleState		bYes;			//사귐수락여부	//	0,1 : 커플수락동의/거절상태 , -1 : 그런거없ㅇ다.
};


class gGamePlayerContainer
{
public:
	static gGamePlayerContainer *GetIF();

	//CHARINFO	m_CharInfo[CLASS_END];
	gCharinfo	m_CharInfo;

	GAMEPLAYER	m_GamePlayer[MAXROOM][ROOMMAXPLAYER];
	bool		m_isGamePlayer[MAXROOM][ROOMMAXPLAYER];		
	// 깜빡 잊고 호환시키지 않는...
	// 그래도 잘 사용될 수 있는 거니 호환시키도록 하자.
	// (아냐 이거 잘 사용되는 거임)

	bool		m_isNokdu[MAXROOM][ROOMMAXPLAYER];
	// 그 플레이어는 녹두에 있는가?

	int			m_nTurn[MAXROOM];
	int			m_nRound[MAXROOM];	

	int			m_nSubjectCount[MAXROOM][ROOMMAXPLAYER][MAXSUBJECT];
	// 학점 관련 성취도
	// 기본적으로 능력 * 20 만큼 증가한다.
	// grade는 subjectContainer에서 확인할 수 있다.
	
	int			m_ItemCoolTime[MAXROOM][ROOMMAXPLAYER][ITEMNUM];
	// 쿨 타임으로 사용되지만,
	// 100단위에서 flag로 사용되기도 함
	// 예컨대, 3번째 유저의 쿨타임 4는 304 값으로 저장됨
	// releaseItemGlobal 함수와 연동되니 참고
	// 개인적으로 m_ItemCoolTimeSub 를 만드는 것을 추천

	bool		m_bSyncronize[MAXROOM][ROOMMAXPLAYER];	
	// 주사위 던진 뒤, 움직임 같이 모든 클라이언트가 같이 움직여야 하는 경우,
	// m_bSyncornize 값들을 false로 바꾼 뒤, 클라이언트가 완료하는 경우 true로 바꾼다.
	// 보통 user가 없는 값의 경우 항상 true, 아니면 false.

//	bool				m_bCoupleSync[MAXROOM];	//수정, 커플용 동기화작업할때 쓴다.
	
	PK_ITEMUSE_ASK m_struct_itemuse_ask[MAXROOM];
	// pk_itemuse_ask -> pk_itemuse_rep -> pk_itemusestart_ask 로 가는 과정에서 임시 저장되는 패킷.


	sFavor m_favor[MAXROOM][ROOMMAXPLAYER];
	//호감도

	void		pk_maingamestart_rep(int nRoomIndex);
	void		pk_movestart_ask(PK_DEFAULT *pk,SOCKET sock);
	void		pk_moveend_ask(PK_DEFAULT *pk,SOCKET sock);
	void		pk_nextturn_rep(int nRoomIndex);
	void		pk_busmovechoose_rep(int nRoomIndex,char* szID);
	void		pk_busmoveselect_ask(PK_DEFAULT *pk,SOCKET sock);
	void		pk_busmoveend_ask(PK_DEFAULT *pk,SOCKET sock);
	void		pk_gameplayerinfo_rep (int nRoomIndex);
	void		pk_subGameplayerinfo_rep(int nRoomIndex,PK_GAMEPLAYERINFO_REP* rep);

	void		pk_gameend_rep(int nRoomIndex);

	void		pk_itemuse_ask(PK_DEFAULT *pk,SOCKET sock);

	void		pk_exit_ask(char*, SOCKET sock);//수정사항, 부뤸ㅋㅋ
	void		pk_gologin_ask(PK_DEFAULT *pk,SOCKET sock);	

	//	void		pk_warpstart_ask (PK_DEFAULT *pk, SOCKET sock);
	void		pk_warpstart_rep (int nRoomIndex, int nInRoomIndex, int des);
	void		pk_warpend_ask (PK_DEFAULT *pk, SOCKET sock);
	void		pk_warpliststart_rep (int nRoomIndex, bool* bInRoomIndex, int* des);
	void		pk_warplistend_ask (PK_DEFAULT *pk, SOCKET sock);

	void		pk_itemusestart_ask(PK_DEFAULT *pk,SOCKET sock);
	void		pk_infochangeItem_rep(PK_ITEMUSE_ASK ask);								//item사용시 수치변경
	void		pk_infochangeTile_rep(int nRoomIndex, int nInRoomIndex , int stamina,int accomplishment , int coupleIndex = -1);	// coupleIndex가 -1이면 한사람, -1이 아니면 커플인덱스값

	void		pk_infochangeend_ask(PK_DEFAULT *pk, SOCKET sock);


	void		pk_anscouple_ask(PK_DEFAULT *pk , SOCKET sock);
	void		pk_becoupleend_ask(PK_DEFAULT *pk , SOCKET sock);

	void		pk_askcouple_rep( int nRoomIndex , int playerIndex_a, int playerIndex_b);	//	 a, b : 플레이어의 인덱스값	

	bool		SetUp();
	bool		init(int nRoomIndex);
	void		Release();



	void				debuger_move(int nDis,char* szID);
	void				debuger_card(int nIndex,char* szID);

private:

	int					m_rmWalk[MAXROOM];	//remain walk
	
	void				FirstTurn(int nRoomIndex);
	void				NextTurn(int nRoomIndex);

	void				PushbSynAllPlayer(int nRoomIndex,bool bSyn);
	bool				isbSynAllTrue(int nRoomIndex);

	int					meetGrade(int nRoomIndex,int subjectIndex ,int nInRoomIndex);  // subjectIndex = flag1
	int					meetItemCalculator(int nRoomIndex,int nInRoomIndex,int classType,int originalVal);
	void				GradeRankSyncronizer(int nRoomIndex);
	int					Rank(int nRoomIndex,int nInRoomIndex);

	int					staminaConvert(int nRoomIndex,int nInRoomIndex,int nPlusMinus);
	int					WhoIsRankOne(int nRoomIndex);
	void				getItem(int nRoomIndex, int nInRoomIndex,int nItemID = -1);
	void				pushItem(int nRoomIndex, int nInRoomIndex,int nItemID);
	void				releaseItemGlobal(int nRoomIndex);
	ItemUseState		itemUse (PK_ITEMUSE_ASK ask, int nRoomIndex, int nInRoomIndex, int itemIndex);
	// nInRoomIndex는 아이템을 사용한 유저, itemIndex는 0-19 까지의 말그대로의...
	// bool 값은, nextturn을 시전하는 지 여부.
	// int 로 바뀔 가능성.

	void				movePlayer(int nRoomIndex,int nInRoomIndex,int des,MovePlayerState mpState);
	bool				isNokduFromPosByHardCoded(int nPos);

	// void		putTargetTrueForOther(int nRoomIndex,int nInRoomIndex, bool* getBarrInRoomIndex,ITEMTARGET target); 
	void		putTargetIntForOther(int nRoomIndex,int nInRoomIndex, int* getNarrDes,ITEMTARGET target,int nVal);
	// 두 함수는 other일 때만 작동하는 가짜 함수이다.

	int				putFavorCross(int nRoomIndex, int nInRoomIndex, int newDist , int nextDist);	//옷깃만 스쳐도 인연
			// 리턴값 : 지나갈때 사귐신청이면 그 위치 리턴
	bool				favorUpFunc ( int nRoomIndex , int playerIndex_a, int playerIndex_b , int upPoint = CROSS_FAVORPOINT);	//	 a, b : 플레이어의 인덱스값	
			// 리턴값 : 지나가다 만날 때 커플신청이면 true

	
};

