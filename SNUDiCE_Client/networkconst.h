//------------------------------------------------------------------------------------
//	networkconst.h
//
//	2010. 08. 04	CoderK	
//
//	network 관련 define 및 프로토콜 및 패킷 구조체
//	ws2_32.lib 추가해야 함
//------------------------------------------------------------------------------------
#pragma once
#include <windows.h>

#define WINSOCK_VERSION_2_2		MAKEWORD(2, 2)
#define SERVER_IP				"127.0.0.1"
#define SERVER_PORT				9000
#define BUFFERSIZE				1024

#define WM_SOCKET				WM_USER + 1

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//	통신 프로토콜
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
enum ePROTOCOL
{
	PL_LOGIN_ASK,
	PL_LOGIN_REP,
};

#define PK_HEADER_SIZE			4

struct PK_DEFAULT
{
	WORD	dwSize;
	WORD	dwProtocol;
	char	strPacket[BUFFERSIZE];
};

struct PK_LOGIN_ASK
{
	char	szID[16];
	char	szPW[16];
};


enum eCOREMODE
{
	ECM_NONLOGIN,			// login 안한상태
	ECM_LOGIN,
	ECM_BATTLENET,			// LOGIN 하면 이 모드
	
	
	ECM_TITLE,				// 타이틀
	ECM_PSEL,				// 플레이어 고르자
	ECM_CSEL,				// 캐릭터 고르자
	ECM_SUBMIT,				// 수강신청
	ECM_GAME,				// 게임
};

struct USER
{
	char		szID[16];
	char		szPW[16];
	eCOREMODE	coreWhere;
	int			nCoreFlag;
};
