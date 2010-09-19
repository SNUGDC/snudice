#include "RoomCore.h"
#include "MainWin.h"
#include "PlayerContainer.h"
#include "ChannelContainer.h"

static gRoomCore s_RoomCore;

gRoomCore *gRoomCore::GetIF()
{
	return &s_RoomCore;
}

bool gRoomCore::SetUp()
{
//	for(int i = 0 ; i < MAXROOMNUM ; i++)
//		m_isRoom[i] = false;
	memset(m_isRoom,0,sizeof(bool)*MAXROOM);
	memset(m_rooms,0,sizeof(ROOM)*MAXROOM);
	return true;
}

void gRoomCore::Release()
{

}

//////////////////////////////////////////////////////////////////////////
/// private function
//////////////////////////////////////////////////////////////////////////

int gRoomCore::FindFirstEmptyRoom ()
{
	for(int i = 0 ; i < MAXROOM ; i++)
	{
		if(!m_isRoom[i]) return i;
	}
	return -1;
}

int gRoomCore::FindTheRoom(char* szRoomName)
{
	for(int i = 0 ;  i < MAXROOM ; i++)
	{
		if(m_isRoom[i]) {
			if(strcmp(szRoomName,m_rooms[i].szRoomName)==0) return i;
		}
	}
	return -1;
}

bool gRoomCore::isFullRoom (int index)
{
	if(m_isRoom[index] && m_rooms[index].nNowPlayer >= m_rooms[index].nMaxPlayer) return true;
	else return false;
}

void gRoomCore::EnterTheRoom (int index,char* szID)
{
	for(int i = 0 ; i < ROOMMAXPLAYER ; i++)
	{
		if(m_rooms[index].szRoomMaxPlayer[i][0] != '\0') 
			continue;
		strcpy(m_rooms[index].szRoomMaxPlayer[i],szID);
		m_rooms[index].nNowPlayer++;
		break;
	}
	// 방에 못들어가도 에러처리는 없으니. isFullRoom을 애용하길.
}	

void gRoomCore::ExitTheRoom (char* szID)	// 방장을 빼내면 방이 파괴됨.
{
	
	for(int i = 0 ; i < MAXROOM ; i++) {
		for(int j = 0 ; j < ROOMMAXPLAYER ; j++)
		{
			if(strcmp(m_rooms[i].szRoomMaxPlayer[j],szID)!=0) continue;
			if(m_rooms[i].nNowPlayer<=1) {	// 이상하게 유연한 타이핑
				DeleteTheRoom(i);
				SendRoomListCauseChange(i/MAXROOMFORPAGE);
				return ;
			}
			else {
				memset(m_rooms[i].szRoomMaxPlayer[j],0,sizeof(char)*IDLENGTH);
				m_rooms[i].nNowPlayer--;

				if(j==m_rooms[i].nMakerIndex) {
					ChangeMakerToFirstUser(i);
				}

				PK_ROOMREFRESH_REP rep;
				rep.room = m_rooms[i];

				PLAYER l_playerlist[ROOMMAXPLAYER];
				FindPlayersFromIDs_RMP(i,l_playerlist);
				memcpy(&rep.playerlist,&l_playerlist,sizeof(PLAYER)*ROOMMAXPLAYER);
		
				gPlayerContainer::GetIF()->SendSelect(
					PL_ROOMREFRESH_REP,sizeof(PK_ROOMREFRESH_REP),&rep,ECM_ROOM,i);
				SendRoomListCauseChange(i/MAXROOMFORPAGE);

				return;
			}
		}
	}
}

void gRoomCore::FindPlayersFromIDs_RMP(int roomIndex,PLAYER* getPlayerlist) {
	gPlayerContainer *gPC = gPlayerContainer::GetIF();
	for(int j = 0 ; j < ROOMMAXPLAYER ; j++) {
		if(m_rooms[roomIndex].szRoomMaxPlayer[j][0]=='\0') {
			memset(&getPlayerlist[j],0,sizeof(PLAYER));
		}
		else {
			getPlayerlist[j] = gPC->GetPlayerFromID(m_rooms[roomIndex].szRoomMaxPlayer[j]);
		}
	}
}

bool gRoomCore::ChangeMakerToFirstUser(int roomIndex) 
{
	for(int i = 0 ; i < ROOMMAXPLAYER ; i++) {
		if(m_rooms[roomIndex].szRoomMaxPlayer[i][0]!='\0') {
			m_rooms[roomIndex].nMakerIndex = i;
			return true;
		}
	}
	return false;
}


void gRoomCore::DeleteTheRoom (int index)
{
	m_isRoom[index] = false;
	memset(&(m_rooms[index]),0,sizeof(ROOM));
//	SendRoomListCauseChange(i/MAXROOMFORPAGE);
}


//////////////////////////////////////////////////////////////////////////
/// connection function
//////////////////////////////////////////////////////////////////////////

void gRoomCore::pk_roommaker_ask(PK_DEFAULT *pk, SOCKET sock)
{
	PK_ROOMMAKER_ASK		ask;		//from client
	gPlayerContainer *gPC = gPlayerContainer::GetIF();
	gChannelContainer *gCCt = gChannelContainer::GetIF();

	SOCKADDR_IN			clientAddr;
	int					addrLen;
	char				buf [1024];

	addrLen = sizeof(clientAddr);
	getpeername(sock, (SOCKADDR*)&clientAddr, &addrLen);

	ask = *((PK_ROOMMAKER_ASK*)pk->strPacket);

	wsprintf(buf,"[PK_ROOMMAKER_ASK] %s\t message : %s\n", inet_ntoa(clientAddr.sin_addr), ask.room.szRoomName);
	OutputDebugString(buf);



	PK_ROOMMAKER_REP	rep;
	

	int emptyRoom = FindFirstEmptyRoom();

	if(FindTheRoom(ask.room.szRoomName)!=-1) {
		rep.result = ERM_USINGNAME;
		OutputDebugString("[PK_ROOMMAKER_REP] 중복이네요.\n");
	}
	else if(emptyRoom == -1) {	// 정적으로 할당된 방이 모두 사용되고 있는 상황
		OutputDebugString("많이 사랑해주셔서 감사합니다. \n");
		rep.result = ERM_USINGNAME;
	}
	else {
		// 이것이 방 만들기 함수로 들어가야 좀 더 깔끔한 함순데 일단 기억이나 하고 있자.
		// Put 함수로 넣었다가 지운 흔적이 있네 ㅇㅇ
		m_isRoom[emptyRoom] = true;
		m_rooms[emptyRoom] = ask.room;
		gPC->PutMode(ask.szID,ECM_ROOM);
		gPC->PutCoreFlag(ask.szID,emptyRoom);
		gCCt->DeletePlayer(ask.szID); // 추가 코드
		rep.result = ERM_SUCCESS;
		rep.room = m_rooms[emptyRoom];
		wsprintf(buf,"[PK_ROOMMAKER_REP] Index : %d 성공! \n",emptyRoom);
		OutputDebugString(buf);

		SendRoomListCauseChange(emptyRoom/MAXROOMFORPAGE);
	}
	gMainWin::GetIF()->Send(PL_ROOMMAKER_REP, sizeof(rep), &rep, sock);

}




void gRoomCore::pk_roomlist_ask(PK_DEFAULT *pk, SOCKET sock)
{
	gPlayerContainer *gPC = gPlayerContainer::GetIF();

	PK_ROOMLIST_ASK ask;

	SOCKADDR_IN			clientAddr;
	int					addrLen;
	char				buf [1024];

	addrLen = sizeof(clientAddr);
	getpeername(sock, (SOCKADDR*)&clientAddr, &addrLen);

	ask = *((PK_ROOMLIST_ASK*)pk->strPacket);

	wsprintf(buf,"[PK_ROOMLIST_ASK] %s\t player : %s\n", inet_ntoa(clientAddr.sin_addr), ask.szID);
	OutputDebugString(buf);

	if(isRoomInPage(ask.nPage)) {
		gPC->PutMode(ask.szID,ECM_ROOMJOIN);
		gPC->PutCoreFlag(ask.szID,ask.nPage);
		SendRoomListCauseChange(ask.nPage);
	}
}


void gRoomCore::pk_roomjoin_ask(PK_DEFAULT *pk, SOCKET sock)
{
	gPlayerContainer *gPC = gPlayerContainer::GetIF();
	gChannelContainer *gCC = gChannelContainer::GetIF();

	PK_ROOMJOIN_ASK ask;

	SOCKADDR_IN			clientAddr;
	int					addrLen;
	char				buf [1024];

	addrLen = sizeof(clientAddr);
	getpeername(sock, (SOCKADDR*)&clientAddr, &addrLen);

	ask = *((PK_ROOMJOIN_ASK*)pk->strPacket);

	wsprintf(buf,"[PK_ROOMJOIN_ASK] %s\t nPage : %d nIdx : %d\n", inet_ntoa(clientAddr.sin_addr), ask.nPage, ask.nIdx);
	OutputDebugString(buf);

	PK_ROOMJOIN_REP rep;
	int nRoomIndex;
	
	nRoomIndex = ask.nPage*MAXROOMFORPAGE + ask.nIdx;
	if (!m_isRoom[nRoomIndex]) {
		rep.result = ERJ_NOROOM;
		gMainWin::GetIF()->Send(PL_ROOMJOIN_REP, sizeof(rep), &rep, sock);
	}
	else if(m_rooms[nRoomIndex].isPass && strcmp(m_rooms[nRoomIndex].szRoomPass,ask.szPass)!=0) // 너무 대충한 것 같은 느낌의 pass확인
	{
		rep.result = ERJ_PASSWRONG;
		gMainWin::GetIF()->Send(PL_ROOMJOIN_REP, sizeof(rep), &rep, sock);
	}
	//else if(m_rooms[nPage].nNowPlayer == m_rooms[nPage].nMaxPlayer) // 방이 꽉 차면
	else if(isFullRoom(nRoomIndex))
	{
		rep.result = ERJ_FULL;
		gMainWin::GetIF()->Send(PL_ROOMJOIN_REP, sizeof(rep), &rep, sock);
	}
	else 
	{

		EnterTheRoom(nRoomIndex,ask.szID); 

		PK_ROOMREFRESH_REP rep2;
		rep2.room = m_rooms[nRoomIndex];

		// l_playerlist code (정말로 함수로 만들고 싶은..)
		PLAYER l_playerlist[ROOMMAXPLAYER];
		FindPlayersFromIDs_RMP(nRoomIndex,l_playerlist);
		memcpy(&rep2.playerlist,&l_playerlist,sizeof(PLAYER)*ROOMMAXPLAYER);

		gPC->SendSelect(PL_ROOMREFRESH_REP,sizeof(PK_ROOMREFRESH_REP),&rep2,ECM_ROOM,nRoomIndex);

		rep.result = ERJ_SUCCESS;
		gPC->PutMode(ask.szID,ECM_ROOM);
		gPC->PutCoreFlag(ask.szID,nRoomIndex);
		gCC->DeletePlayer(ask.szID);
		
		// rep.playerlist = l_playerlist; // 왼쪽 피연산자는 l-value이어야 합니다. (?)
		rep.joinroom = m_rooms[nRoomIndex]; 
		memcpy(&rep.playerlist,&l_playerlist,sizeof(PLAYER)*ROOMMAXPLAYER);

		wsprintf(buf,"[PK_ROOMJOIN_REP] %s\t \n", inet_ntoa(clientAddr.sin_addr));
		OutputDebugString(buf);
		gMainWin::GetIF()->Send(PL_ROOMJOIN_REP, sizeof(rep), &rep, sock);

	}
}

//////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////


void gRoomCore::SendRoomListCauseChange(int nPage) {

	PK_ROOMLIST_REP		rep;
	gPlayerContainer *gPC = gPlayerContainer::GetIF();

	rep.nPage = nPage;

	for(int i = 0 ; i < MAXROOMFORPAGE ; i++)
	{
		rep.roomlist[i] = m_rooms[i+nPage*MAXROOMFORPAGE];
	}

	gPC->SendSelect(PL_ROOMLIST_REP,sizeof(PK_ROOMLIST_REP),&rep,ECM_ROOMJOIN,nPage);
}

bool gRoomCore::isRoomInPage(int nPage) {
	for(int i = 0 ; i < MAXROOMFORPAGE ; i++) {
		if(m_isRoom[i+nPage*MAXROOMFORPAGE]) 
			return true;
	}
	return false;
}
