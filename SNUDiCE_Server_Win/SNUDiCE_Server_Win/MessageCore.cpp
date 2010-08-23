#include "MessageCore.h"
#include "PlayerContainer.h"
#include "MainWin.h"
#include "MysqlDB.h"

static gMessageCore s_MessageCore;

gMessageCore *gMessageCore::GetIF()
{
	return &s_MessageCore;
}

gMessageCore::gMessageCore()
{
//	gMysql::GetIF()->init();
}

gMessageCore::~gMessageCore()
{

}

bool gMessageCore::SetUp()
{
	return true;
}

void gMessageCore::Release()
{
	gMysql::GetIF()->release();
}

void gMessageCore::pk_message_ask(PK_DEFAULT *pk, SOCKET sock)
{
	PK_MESSAGE_ASK		ask;		//from client
	gPlayerContainer	*gPC	= gPlayerContainer::GetIF();
	PLAYER*				temp;

	// for print
	SOCKADDR_IN			clientAddr;
	int					addrLen;
	char				buf [1024];

	addrLen = sizeof(clientAddr);
	getpeername(sock, (SOCKADDR*)&clientAddr, &addrLen);

	ask = *((PK_MESSAGE_ASK*)pk->strPacket);

	sprintf(buf,"[PK_MESSAGE_ASK] %s\tid : %s\t message : %s\n", inet_ntoa(clientAddr.sin_addr), ask.szID, ask.szMsg);
	OutputDebugString(buf);

	PK_MESSAGE_REP	rep;

	for(PLAYER_LIST::iterator it = gPC->m_PlayerList.begin(); it != gPC->m_PlayerList.end(); it++) {
			//if(strcmp(it->szID,ask->szID)==0) continue;
		temp = *it;
		if(temp->coreWhere == ECM_BATTLENET) {
			strcpy(rep.szID,ask.szID);
			strcpy(rep.szMsg,ask.szMsg);
			gMainWin::GetIF()->Send(PL_MESSAGE_REP, sizeof(rep), &rep, temp->sock);
		}
	}
}


