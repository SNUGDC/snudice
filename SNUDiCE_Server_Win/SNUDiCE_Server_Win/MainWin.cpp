#include "MainWin.h"
#include "LoginCore.h"
#include "PlayerContainer.h"

/*
#include "const.h"
#include "Mouse.h"
#include "LoginCore.h"
#include "Server.h"
#include "Util.h"
#include "PopUp.h"
*/

DWORD WINAPI ProcessClient(LPVOID arg);				// server <-> client 간 통신. 스레드
static void err_quit(char *msg);					// 소켓 함수 오류 출력후 종료
static void err_display(char *msg);					// 소켓 함수 오류 출력

static gMainWin	s_MainWin;		// for singleton

gMainWin::gMainWin()
{

}

gMainWin::~gMainWin()
{

}

gMainWin* gMainWin::GetIF()
{
	return &s_MainWin;
}

bool gMainWin::SetUp(HINSTANCE hInstance, LPSTR lpszCmdParam, int nCmdShow)
{
		int		retVal;

	// Winsock 초기화
	WSADATA	wsa;

	if(WSAStartup(WINSOCK_VERSION_2_2, &wsa) != 0)	// WSAStartup은 리턴값만으로 오류를 처리한다.
	{
		MessageBox(NULL, "WinSock 초기화 실패", "Error", MB_OK);
		return false;
	}

	// socket()
	m_listen_sock = socket(AF_INET, SOCK_STREAM, 0);	// af ; 주소체계, type ; 소켓타입, protocol ; 프로토콜
													// AF_INET, SOCK_STREAM -> TCP	; 신뢰성, 연결형 프로토콜
													//			SOCK_DGRAM	-> UDP	; 비신뢰, 비연결	"

	if(m_listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	// bind()
	SOCKADDR_IN		serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family		= AF_INET;					//hton ; host bytes to network bytes(Big Endian)
	serverAddr.sin_port			= htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr	= inet_addr(SERVER_IP);
	retVal = bind(m_listen_sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	
	if(retVal == SOCKET_ERROR)
		err_quit("bind()");

	// listen()
	retVal = listen(m_listen_sock, SOMAXCONN);
	if(retVal == SOCKET_ERROR)
		err_quit("listen()");

	if(!gLoginCore::GetIF()->SetUp())
		return false;

	if(!gPlayerContainer::GetIF()->SetUp())
		return false;


	// Initialize
	//memset(this, 0, sizeof(gMainWin));
/*
	m_hInst	= hInstance;
	// Make Window
	WNDCLASS	WndClass;
	memset(&WndClass, 0, sizeof(WNDCLASS));
	WndClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance		= hInstance;
	WndClass.lpfnWndProc	= (WNDPROC)WndProc;
	WndClass.lpszClassName	= GAMENAME;
	WndClass.lpszMenuName	= NULL;
	WndClass.style			= CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	m_hWnd = CreateWindow(GAMENAME, GAMENAME, WNDSTYLE, 
						CW_USEDEFAULT, CW_USEDEFAULT, WNDSIZEW, WNDSIZEH,
						NULL, (HMENU)NULL, hInstance, NULL);

	// ReSize Window
	RECT	rcWin = {0, 0, WNDSIZEW, WNDSIZEH};
	AdjustWindowRect(&rcWin, WNDSTYLE, false);
	SetWindowPos(m_hWnd, NULL, 0, 0,
			rcWin.right - rcWin.left, rcWin.bottom - rcWin.top,
			SWP_NOMOVE | SWP_NOZORDER);

	MoveWindow();
	ShowWindow(m_hWnd, nCmdShow);
	*/

	return true;
}

/*
void gMainWin::MoveWindow()
{
	SetRect(&m_rcScr, 0, 0, WNDSIZEW, WNDSIZEH);
	ClientToScreen(m_hWnd, (POINT*)&m_rcScr);
	ClientToScreen(m_hWnd, (POINT*)&m_rcScr + 1);
}
*/

void gMainWin::Release()
{
	closesocket(m_listen_sock);
	WSACleanup();
	gLoginCore::GetIF()->Release();
}


DWORD WINAPI Run(LPVOID prc)
{
	gMainWin *gmainWin = gMainWin::GetIF();

	
		SOCKET			client_sock;
	SOCKADDR_IN		clientAddr;
	int				addrLen;
//	int				retVal;
	char			szBuf[BUFFERSIZE + 1];		// 왜 + 1인가??
	HANDLE			hThread;
	DWORD			dwThreadID;

	ZeroMemory(&clientAddr, sizeof(SOCKADDR_IN));

		MSG		Msg;
		char	buf[1024];

	OutputDebugString("Server Start\n");

	while(true)
	{
		// accept()

		addrLen = sizeof(clientAddr);
		client_sock = accept(gmainWin->m_listen_sock, (SOCKADDR*)&clientAddr, &addrLen);

		if(client_sock == INVALID_SOCKET)
		{

			err_display("accept()");
			continue;
		}


		sprintf(buf,"\n[TCP Server] Client Connect : IP = %s\t Port = %d\n",
				inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		OutputDebugString(buf);

		// make thread
		hThread = CreateThread(NULL, 0, ProcessClient,
								(LPVOID)client_sock, 0, &dwThreadID);

		if(hThread == NULL) 
			OutputDebugString("Thread failed\n");
		else
			CloseHandle(hThread);

		// 메시지 큐에 메시지 있으면 처리
		if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			// ALT 막기
			if(Msg.message == WM_SYSKEYDOWN)
				continue;
			if(Msg.message == WM_QUIT)
				break;
/*
			switch(m_eCoreMode)
			{
				case ECM_LOGIN:
					if(gLoginCore::GetIF()->PreTransMsg(Msg))
						continue;
					break;
			}
			*/

			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else	// 메시지 큐에 메시지 없을 때
		{
			gmainWin->MainLoop();
		}
	}
	return Msg.wParam;
}

void gMainWin::MainLoop()
{
	if(!m_bActive)
		return;
}

void gMainWin::Recv(PK_DEFAULT *pk, SOCKET sock)
{

	switch(pk->dwProtocol)
	{
		case PL_LOGIN_ASK:
			gLoginCore::GetIF()->pk_login_ask(pk, sock);
		break;

	}

}

bool gMainWin::Send(DWORD type, DWORD size, void *buf, SOCKET sock)
{
	PK_DEFAULT		pk;

	if(!sock)
		return false;
	
	char *temp = (char*)buf;
	
	pk.dwProtocol = type;
	
	if(size)
		memcpy(pk.strPacket, temp, size);
	
	pk.dwSize = PK_HEADER_SIZE + size;
	
	int		r1 = 0, r2 = 0;
	int		fail_count = 0;
	
	while(true)
	{
		r2 = send(sock, (char*)&pk, pk.dwSize, 0);
		
		if(r2 != SOCKET_ERROR)
			r1 += r2;
		
		if(r1 == pk.dwSize)
			break;
		
		fail_count++;
		
		if(fail_count > 10)
			return false;
	}
	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	gMainWin	*mw		= gMainWin::GetIF();

	switch(iMsg)
	{

		/*
		case WM_MOVE:
			mw->MoveWindow();
		break;
*/
		case WM_KEYDOWN:
			if(!mw->m_bActive)
				return 0;

			mw->m_Keys[wParam] = true;
			return 0;
		case WM_KEYUP:
			if(!mw->m_bActive)
				return 0;

			mw->m_Keys[wParam] = false;
			return 0;

		// mouse
			/*
		case WM_LBUTTONDOWN:
			if(!mw->m_bActive)
				return 0;

			mouse->m_nPosX = LOWORD(lParam);
			mouse->m_nPosY = HIWORD(lParam);
			mouse->OnLButtonDown();
			return 0;
		case WM_LBUTTONUP:
			if(!mw->m_bActive)
				return 0;

			mouse->m_nPosX = LOWORD(lParam);
			mouse->m_nPosY = HIWORD(lParam);
			mouse->OnLButtonUp();
			return 0;
		case WM_RBUTTONDOWN:
			if(!mw->m_bActive)
				return 0;

			mouse->m_nPosX = LOWORD(lParam);
			mouse->m_nPosY = HIWORD(lParam);
			mouse->OnRButtonDown();
			return 0;
		case WM_MOUSEMOVE:
			if(!mw->m_bActive)
				return 0;

			mouse->m_nPosX = LOWORD(lParam);
			mouse->m_nPosY = HIWORD(lParam);
			mouse->OnMouseMove();
			return 0;
			*/

		// active
		case WM_ACTIVATE:
			switch(LOWORD(wParam))
			{
				case WA_ACTIVE: case WA_CLICKACTIVE:
					mw->m_bActive = true;
					break;
				case WA_INACTIVE:
					mw->m_bActive = false;
					break;
			}
			return 0;

		// network
			/*
		case WM_SOCKET:
			gServer::GetIF()->Receive(lParam);
			return 0;
			*/
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}

/*
void gMainWin::Exit(HWND hWnd)
{
	SendMessage(hWnd, WM_DESTROY, 0, 0);
}
*/

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//	private
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
void err_quit(char *msg)
{
	
	LPVOID	lpMsgBuf;


	
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
	
}

void err_display(char *msg)
{
	
	LPVOID	lpMsgBuf;
	char	buf [1024];


	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	sprintf(buf,"[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	OutputDebugString(buf);

	LocalFree(lpMsgBuf);
	
}

// client <-> server 간 통신.

DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET			client_sock = (SOCKET)arg;
	SOCKADDR_IN		clientAddr;
	char			szBuf[BUFFERSIZE + 1];
	int				addrLen;
	int				retVal;
	PK_DEFAULT		pkDefault;


	char		buf[1024];

	// 클라이언트 정보 얻기
	addrLen = sizeof(clientAddr);
	getpeername(client_sock, (SOCKADDR*)&clientAddr, &addrLen);

	while(true)
	{
		int		r1 = 0, r2 = 0;
		int		fail_count = 0;
		
		retVal = recv(client_sock, (char*)&pkDefault, PK_HEADER_SIZE, 0);
		if(retVal == SOCKET_ERROR)
			continue;
		if(retVal == 0)
			break;

		r1 += PK_HEADER_SIZE;

		while(true)
		{
			r2 = recv(client_sock, pkDefault.strPacket, pkDefault.dwSize - PK_HEADER_SIZE, 0);
			
			if(r2 != SOCKET_ERROR)
				r1 += r2;
			
			if(r1 == pkDefault.dwSize)
				break;
			
			fail_count++;
			
			if(fail_count > 10)
				break;
		}

		if(fail_count <= 10)
			gMainWin::GetIF()->Recv(&pkDefault, client_sock);
	}

	// closesocket
	gPlayerContainer::GetIF()->DeletePlayer(client_sock);
	closesocket(client_sock);
	
	sprintf(buf,"[TCP Server] Client Exit : IP = %s\t Port = %d\n",
			inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	OutputDebugString (buf);

	return 0;
}