#include "PlayerContainer.h"
#include "MainWin.h"

static gPlayerContainer s_PlayerContainer;

gPlayerContainer *gPlayerContainer::GetIF()
{
	return &s_PlayerContainer;
}

gPlayerContainer::gPlayerContainer()
{

}

gPlayerContainer::~gPlayerContainer()
{

}

bool gPlayerContainer::SetUp()
{
	return true;
}

void gPlayerContainer::Release()
{
	PLAYER*		temp;

	for(PLAYER_LIST::iterator it = m_PlayerList.begin();
		it != m_PlayerList.end(); it++)
	{
		temp = *it;
		delete temp;
		temp = NULL;
	}
	m_PlayerList.clear();
}

void gPlayerContainer::AddPlayer(PLAYER* player)
{
	PLAYER*		add;
	add = new PLAYER;

	memcpy(add, player, sizeof(PLAYER));

	m_PlayerList.push_back(add);
}

bool gPlayerContainer::isExistedPlayer(char* id)
{
	PLAYER*		temp;

	for(PLAYER_LIST::iterator it = m_PlayerList.begin();
		it != m_PlayerList.end(); it++)
	{
		temp = *it;

		if(strcmp(temp->szID, id) == 0)
			return true;
	}
	return false;
}

bool gPlayerContainer::DeletePlayer(SOCKET sock,char clientID[IDLENGTH])
{
	PLAYER*		temp;

	for(PLAYER_LIST::iterator it = m_PlayerList.begin();
		it != m_PlayerList.end(); it++)
	{
		temp = *it;

		if(temp->sock == sock)
		{
			strcpy(clientID,temp->szID);

			delete temp;
			temp = NULL;
			m_PlayerList.erase(it);
			
			return true;
			//break;
		}
	}

	return false;
}

bool gPlayerContainer::FindSocketFromID(char* id, SOCKET* getSock)
{
	PLAYER*		temp;

	for(PLAYER_LIST::iterator it = m_PlayerList.begin();
		it != m_PlayerList.end(); it++)
	{
		temp = *it;
		
		if(strcmp(temp->szID,id)==0)
		{
			*getSock = temp->sock;
			return true;
		}
	}

	return false;

}

bool gPlayerContainer::PutMode(char* id,eCOREMODE mode)
{
	PLAYER*		temp;

	for(PLAYER_LIST::iterator it = m_PlayerList.begin();
		it != m_PlayerList.end(); it++)
	{
		temp = *it;

		if(strcmp(temp->szID,id)==0)
		{
			temp->coreWhere = mode;
			return true;
		}
	}

	return false;
}

bool gPlayerContainer::PutCoreFlag (char* id,int flag)
{
	PLAYER*		temp;

	for(PLAYER_LIST::iterator it = m_PlayerList.begin();
		it != m_PlayerList.end(); it++)
	{
		temp = *it;

		if(strcmp(temp->szID,id)==0)
		{
			temp->nCoreFlag = flag;
			return true;
		}
	}

	return false;
}

void gPlayerContainer::SendSelect (ePROTOCOL prot,int pkSize,void *pk,eCOREMODE mode,int nPage)
{
	PLAYER*		temp;

	for(PLAYER_LIST::iterator it = m_PlayerList.begin();
		it != m_PlayerList.end(); it++)
	{
		temp = *it;

		if(temp->coreWhere==mode && temp->nCoreFlag == nPage)
		{
			gMainWin::GetIF()->Send(prot, pkSize, pk, temp->szID);
		}
	}
}