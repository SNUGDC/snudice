//------------------------------------------------------------------------------------
//	gMainWin Class													MainWin -> Server
//
//	2010. 08. 04	CoderK
//
//	윈도우 관련 정보.
//	각종 셋업 및 게임의 주축.
//-------------------i-----------------------------------------------------------------
#pragma once
#include <windows.h>
#include <ddraw.h>
//#include "networkconst.h"
#include "network.h"

//#define FULLSCREEN		//	define 풀었다 해제하면서 fullscreen 체크
#define	WNDSIZEW		640
#define WNDSIZEH		480
#define WNDNAME			"SNU Dice Server"
#define WNDSTYLE		(WS_CAPTION | WS_SYSMENU)
//#define BEEPHACKREG		"Control Panel\\Sound"
//#define BEEPHACKREG2	"AppEvents\\Schemes\\Apps\\.Default\\.Default\\.Current"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI Listen(LPVOID prc);		// 클라의 접속요청 받아 처리하는 쓰레드

class gMainWin  
{
public:
	static gMainWin		*GetIF();			// Use Singleton
	
	gMainWin();
	virtual ~gMainWin();
	SOCKET		m_listen_sock;
	
public:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	BOOL		m_Keys[256];				// keyboard
	RECT		m_rcScr;					// screen 출력 영역
	bool		m_bActive;					// 게임(윈도우)가 활성화 되어있나
	
//	eCOREMODE	m_eCoreMode;				// 게임이 어떤 모드냐
	
	//	direct x
	/*
	LPDIRECTDRAW			m_lpDD;
	LPDIRECTDRAWSURFACE		m_lpDDPrimary;
	LPDIRECTDRAWSURFACE		m_lpDDBack;
	*/
	bool		Send(DWORD type, DWORD size, void *buf, SOCKET sock);
	void		Recv(PK_DEFAULT *pk, SOCKET	sock);

	bool		SetUp(HINSTANCE hInstance, LPSTR lpszCmdParam, int nCmdShow);
	void		Release();
	//DWORD WINAPI	Run(LPVOID prc);
	//void		MoveWindow();
	void		Exit();
	
private:
	char		m_szTempReg[128];	
	
public:
	//bool		DisableBeep();
	//bool		EnableBeep();
	//bool		SetUpDirect();
	bool		MakeListenThread();
	int			Run();
	void		MainLoop();
};

 