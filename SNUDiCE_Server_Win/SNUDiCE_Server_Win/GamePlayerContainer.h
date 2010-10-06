#pragma once
#include "network.h"

class gGamePlayerContainer
{
public:
	static gGamePlayerContainer *GetIF();

	CHARINFO	m_CharInfo[CLASS_END];

	GAMEPLAYER	m_GamePlayer[MAXROOM][ROOMMAXPLAYER];
	bool		m_isGamePlayer[MAXROOM][ROOMMAXPLAYER];		
	// 깜빡 잊고 호환시키지 않는...
	// 그래도 잘 사용될 수 있는 거니 호환시키도록 하자.
	bool		m_isNokdu[MAXROOM][ROOMMAXPLAYER];
	int			m_nTurn[MAXROOM];
	int			m_nRound[MAXROOM];	

	int			m_nSubjectCount[MAXROOM][ROOMMAXPLAYER][MAXSUBJECT];
	
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

	void		pk_maingamestart_rep(int nRoomIndex);
	void		pk_movestart_ask(PK_DEFAULT *pk,SOCKET sock);
	void		pk_moveend_ask(PK_DEFAULT *pk,SOCKET sock);
	void		pk_nextturn_rep(int nRoomIndex);
	void		pk_busmovechoose_rep(int nRoomIndex,char* szID);
	void		pk_busmoveselect_ask(PK_DEFAULT *pk,SOCKET sock);
	void		pk_busmoveend_ask(PK_DEFAULT *pk,SOCKET sock);
	void		pk_gameplayerinfo_rep (int nRoomIndex);
	void		pk_subGameplayerinfo_rep(int nRoomIndex,PK_GAMEPLAYERINFO_REP* rep);
	void		pk_popinfo_rep(int nRoomIndex,int stamina,int accomplishment);

	void		pk_gameend_rep(int nRoomIndex);

	void		pk_itemuse_ask(PK_DEFAULT *pk,SOCKET sock);

	bool		SetUp();
	bool		init(int nRoomIndex);
	void		Release();

private:
	void		FirstTurn(int nRoomIndex);
	void		NextTurn(int nRoomIndex);

	void		PushbSynAllPlayer(int nRoomIndex,bool bSyn);
	bool		isbSynAllTrue(int nRoomIndex);

	int			meetGrade(int nRoomIndex,int subjectIndex);  // subjectIndex = flag1
	int			meetItemCalculator(int nRoomIndex,int nInRoomIndex,int classType,int originalVal);
	void		GradeRankSyncronizer(int nRoomIndex);
	int			Rank(int nRoomIndex,int nInRoomIndex);

	int			staminaConvert(int nRoomIndex,int nInRoomIndex,int nPlusMinus);
	int			WhoIsRankOne(int nRoomIndex);
	void		getItem(int nRoomIndex, int nInRoomIndex);
	void		pushItem(int nRoomIndex, int nInRoomIndex,int nItemID);
	void		releaseItemGlobal(int nRoomIndex);
	bool		itemUse (PK_ITEMUSE_ASK ask, int nRoomIndex, int nInRoomIndex, int itemIndex);
	// nInRoomIndex는 아이템을 사용한 유저, itemIndex는 0-19 까지의 말그대로의...
	// bool 값은, nextturn을 시전하는 지 여부.
	// int 로 바뀔 가능성.
	
};