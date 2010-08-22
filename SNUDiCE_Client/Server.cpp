#include "Server.h"
#include "MainWin.h"
#include "Util.h"
#include "stringconst.h"
#include "PopUp.h"
#include "LoginCore.h"
#include "Chat.h"

static SOCKET		s_sock = NULL;
static SOCKADDR_IN	s_serverAddr;

gServer::gServer()
{
	
}

gServer::~gServer()
{
	
}

static gServer	s_Server;

gServer *gServer::GetIF()
{
	return &s_Server;
}

bool gServer::SetUp()
{
	char		szBuf[256];
	int			retVal;

	m_bConnect = false;

	WSADATA		wsa;
	if(WSAStartup(WINSOCK_VERSION_2_2, &wsa) != 0)
		return false;

	s_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(s_sock == INVALID_SOCKET)
	{
		gUtil::DebugMsg("socket ���� ����\n");
		return false;
	}

 	retVal = WSAAsyncSelect(s_sock, gMainWin::GetIF()->m_hWnd, WM_SOCKET,
 								FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
 	if(retVal == SOCKET_ERROR)
 	{
 		gUtil::DebugMsg("WSAAsyncSelect() Failed\n");
 		EndSocket();
 		return false;
 	}

	s_serverAddr.sin_family			= AF_INET;
	s_serverAddr.sin_port			= htons(SERVER_PORT);
	s_serverAddr.sin_addr.s_addr	= inet_addr(SERVER_IP);
	
	retVal = connect(s_sock, (SOCKADDR*)&s_serverAddr, sizeof(s_serverAddr));

	// �񵿱� connect�� �ٷ� return �Ǹ鼭 WSAEWOULDBLOCK ���� �߻���Ų��.
	if( retVal == SOCKET_ERROR && WSAEWOULDBLOCK != WSAGetLastError() )
	{
		wsprintf(szBuf, "[Connect Error!] %s : %d\n", SERVER_IP, SERVER_PORT);
		gUtil::DebugMsg(szBuf);
		EndSocket();
		return false;
	}

// 	wsprintf(szBuf, "[Connect] %s : %d\n",
// 				inet_ntoa(s_serverAddr.sin_addr), ntohs(s_serverAddr.sin_port));
// 	gUtil::DebugMsg(szBuf);

	return m_bConnect;
}

bool gServer::Send(DWORD type, DWORD size, void *buf)
{
	if(!s_sock)
		return false;

	char *temp = (char*)buf;

	m_pkDefault.dwProtocol = type;

	if(size)
		memcpy(m_pkDefault.strPacket, temp, size);

	m_pkDefault.dwSize = PK_HEADER_SIZE + size;

	int		r1 = 0, r2 = 0;
	int		fail_count = 0;

	while(true)
	{
		r2 = send(s_sock, (char*)&m_pkDefault, m_pkDefault.dwSize, 0);

		if(r2 != SOCKET_ERROR)
			r1 += r2;

		if(r1 == m_pkDefault.dwSize)
			break;

		fail_count++;

		if(fail_count > 10)
			return false;
	}
	return true;
}

void gServer::Receive(LPARAM lParam)
{
	if(!SocketErrorMessage(lParam))
		return;

	char	szBuf[256];
	int		retVal;

	switch( WSAGETSELECTEVENT(lParam) )
	{
		case FD_CONNECT:
			wsprintf(szBuf, "[Connect] %s : %d\n",
				inet_ntoa(s_serverAddr.sin_addr), ntohs(s_serverAddr.sin_port));
			gUtil::DebugMsg(szBuf);
			m_bConnect = true;
			break;
		case FD_READ:
			ReadPacket();
			break;
		case FD_WRITE:
			break;
		case FD_CLOSE:
			wsprintf(szBuf, "[Close] %s : %d\n",
				inet_ntoa(s_serverAddr.sin_addr), ntohs(s_serverAddr.sin_port));
			gUtil::DebugMsg(szBuf);
			m_bConnect = false;
			break;
	}
}

void gServer::ReadPacket()
{
	int		r1 = 0, r2 = 0;
	int		fail_count = 0;

	r2 = recv(s_sock, (char*)&m_pkDefault, PK_HEADER_SIZE, 0);
	if(r2 == SOCKET_ERROR)
		return;
	if(r2 == 0)
		return;

	r1 += PK_HEADER_SIZE;

	while(true)
	{
		r2 = recv(s_sock, m_pkDefault.strPacket, m_pkDefault.dwSize - PK_HEADER_SIZE, 0);

		if(r2 != SOCKET_ERROR)
			r1 += r2;

		if(r1 == m_pkDefault.dwSize)
			break;

		fail_count++;

		if(fail_count > 10)
			break;
	}

	if(fail_count <= 10)
		Recv();
}

void gServer::Recv()
{
	switch(m_pkDefault.dwProtocol)
	{
		case PL_LOGIN_REP:
			gLoginCore::GetIF()->pk_login_rep((PK_LOGIN_REP*)m_pkDefault.strPacket);
		break;
		case PL_MESSAGE_REP:
			gChat::GetIF()->pk_message_rep((PK_MESSAGE_REP*)m_pkDefault.strPacket);
		break;
	}
}

void gServer::ReConnect()
{
	char	szBuf[256];
	int		retVal;

	retVal = connect(s_sock, (SOCKADDR*)&s_serverAddr, sizeof(s_serverAddr));
	
	if( retVal == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK )
	{
		EndSocket();
		wsprintf(szBuf, "socket connect faild: %d\n", WSAGetLastError());
		gUtil::DebugMsg(szBuf);
		return ;
	}
	wsprintf(szBuf, "socket [re]connect because: %d\n", WSAGetLastError());
	gUtil::DebugMsg(szBuf);
}

void gServer::Release()
{
	EndSocket();
}

//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
//	Private
//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
void gServer::EndSocket()
{
	if(s_sock)
	{
		shutdown(s_sock, 0x02);		// SD_BOTH �� ����?
		closesocket(s_sock);
		s_sock = NULL;
	}
}

bool gServer::SocketErrorMessage(LPARAM lParam)
{
	char	szBuf[256];
	int		retVal;
	
	if( WSAGETSELECTERROR(lParam) )
	{
		m_bConnect = false;

		if( ( WSAGETSELECTERROR(lParam) == WSAECONNREFUSED ) ||		// 10061 :Connection refused. ���� ���α׷��� ���� X
			( WSAGETSELECTERROR(lParam) == WSAENETUNREACH ) ||		// 10051 :Network is unreachable.
			( WSAGETSELECTERROR(lParam) == WSAETIMEDOUT ) )//||		// 10060 :Connection timed out.
		{
			gPopUp::GetIF()->SetPopUp(ECLK_CANCEL, EPOP_RECONNECT, STR_1, STR_2);

 			return false;
		}
		EndSocket();
		if( WSAGETSELECTERROR(lParam) == WSAECONNABORTED)			// 10053 :������ �����.. �Ƹ���
		{
			gUtil::DebugMsg("�������� ������ ����\n");
			gPopUp::GetIF()->SetPopUp(ECLK_OK, EPOP_DISCONNECT, STR_3);
			return false;
		}
		else
			wsprintf(szBuf, "WM_SOCKET error: %d\n", WSAGETSELECTERROR(lParam));
		gUtil::DebugMsg(szBuf);
		return false;
	}
	return true;
}