#include "MainWin.h"
#include "const.h"
#include "Mouse.h"
#include "LoginCore.h"
#include "BattleNetCore.h"
#include "Server.h"
#include "Util.h"
#include "PopUp.h"
#include "Chat.h"

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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
	// Initialize
	memset(this, 0, sizeof(gMainWin));

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

#ifdef FULLSCREEN
	m_hWnd = CreateWindow(GAMENAME, GAMENAME, WS_POPUP, 
						CW_USEDEFAULT, CW_USEDEFAULT, WNDSIZEW, WNDSIZEH,
						NULL, (HMENU)NULL, hInstance, NULL);
#else
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
#endif

	if(!SetUpDirect())
		return false;

	if(!gPopUp::GetIF()->SetUp())
		return false;

	if(!gLoginCore::GetIF()->SetUp())
		return false;

	if(!gBattleNetCore::GetIF()->SetUp())
		return false;

	if(!gChat::GetIF()->SetUp())
		return false;

	gServer::GetIF()->SetUp();

	ShowWindow(m_hWnd, nCmdShow);

	if(!DisableBeep())
		return false;

	// Set Coremode

	m_eCoreMode = ECM_LOGIN;

	return true;
}

void gMainWin::MoveWindow()
{
	SetRect(&m_rcScr, 0, 0, WNDSIZEW, WNDSIZEH);
	ClientToScreen(m_hWnd, (POINT*)&m_rcScr);
	ClientToScreen(m_hWnd, (POINT*)&m_rcScr + 1);
}

void gMainWin::Release()
{
	gServer::GetIF()->Release();
	gPopUp::GetIF()->Release();
	gMouse::GetIF()->Release();
	gLoginCore::GetIF()->Release();
	SAFE_RELEASE(m_lpDDBack);
	SAFE_RELEASE(m_lpDDPrimary);
	SAFE_RELEASE(m_lpDD);
	EnableBeep();
}


int gMainWin::Run()
{
	MSG		Msg;

	// Mainloop of game
	while(true)
	{
		// �޽��� ť�� �޽��� ������ ó��
		if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			// ALT ����
			if(Msg.message == WM_SYSKEYDOWN)
				continue;
			if(Msg.message == WM_QUIT)
				break;

			switch(m_eCoreMode)
			{
				case ECM_LOGIN:
					if(gLoginCore::GetIF()->PreTransMsg(Msg))
						continue;
					break;
				case ECM_BATTLENET:
					if(gBattleNetCore::GetIF()->PreTransMsg(Msg))
						continue;
					break;
			}

			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else	// �޽��� ť�� �޽��� ���� ��
		{
			MainLoop();
		}
	}
	return Msg.wParam;
}

void gMainWin::MainLoop()
{
	//if(!m_bActive)
	//	return;

	switch(m_eCoreMode)
	{
		case ECM_LOGIN:
			gLoginCore::GetIF()->MainLoop();
			break;
		case ECM_BATTLENET:
			gBattleNetCore::GetIF()->MainLoop();
			break;
	}

	// backbuffer �� �׷��� �͵��� ���
#ifdef FULLSCREEN
	m_lpDDPrimary->Flip(NULL, DDFLIP_WAIT);
#else
	m_lpDDPrimary->Blt(&m_rcScr, m_lpDDBack, NULL, DDBLT_WAIT, NULL);
#endif
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	gMainWin	*mw		= gMainWin::GetIF();
	gMouse		*mouse	= gMouse::GetIF();

	switch(iMsg)
	{
#ifndef FULLSCREEN
		case WM_MOVE:
			mw->MoveWindow();
		break;
#endif
		// keyboard
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
		case WM_SOCKET:
			gServer::GetIF()->Receive(lParam);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}

void gMainWin::Exit()
{
	SendMessage(m_hWnd, WM_DESTROY, 0, 0);
}

bool gMainWin::SetUpDirect()
{
	if(FAILED(DirectDrawCreate(NULL, &m_lpDD, NULL)))
	{
		MessageBox(m_hWnd, "DirectDraw : Interface ���� ����", "Error", MB_OK);
		return false;
	}
#ifdef FULLSCREEN
	if(FAILED(m_lpDD->SetCooperativeLevel(m_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)))
	{
		MessageBox(m_hWnd, "DirectDraw : Cooperative Level ���� ����", "Error", MB_OK);
		return false;
	}
	
	if(FAILED(m_lpDD->SetDisplayMode(WNDSIZEW, WNDSIZEH, BIT)))
	{
		MessageBox(m_hWnd, "DirectDraw : ���÷��� ���� ����", "Error", MB_OK);
		return false;
	}
	
	// Primary Buffer
	DDSURFACEDESC	ddsd;
	memset(&ddsd, 0, sizeof(DDSURFACEDESC));
	ddsd.dwSize				= sizeof(DDSURFACEDESC);
	ddsd.dwFlags			= DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps		= DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwBackBufferCount	= 1;
	
	if(FAILED(m_lpDD->CreateSurface(&ddsd, &m_lpDDPrimary, NULL)))
	{
		MessageBox(m_hWnd, "DirectDraw : Primary Surface ���� ����", "Error", MB_OK);
		return false;
	}
	
	// Backbuffer
	DDSCAPS		ddsCaps;
	memset(&ddsCaps, 0, sizeof(DDSCAPS));
	ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	
	m_lpDDPrimary->GetAttachedSurface(&ddsCaps, &m_lpDDBack);
#else
	if(FAILED(m_lpDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL)))
	{
		MessageBox(m_hWnd, "DirectDraw : Cooperative Level ���� ����", "Error", MB_OK);
		return false;
	}
	
	// Primary Buffer
	DDSURFACEDESC	ddsd;
	memset(&ddsd, 0, sizeof(DDSURFACEDESC));
	ddsd.dwSize			= sizeof(DDSURFACEDESC);
	ddsd.dwFlags		= DDSD_CAPS;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_PRIMARYSURFACE;
	
	if(FAILED(m_lpDD->CreateSurface(&ddsd, &m_lpDDPrimary, NULL)))
	{
		MessageBox(m_hWnd, "DirectDraw : Primary Surface ���� ����", "Error", MB_OK);
		return false;
	}
	
	// Back Buffer
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize			= sizeof(ddsd);
	ddsd.dwFlags		= DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY; //DDSCAPS_VIDEOMEMORY
	ddsd.dwWidth		= WNDSIZEW;
	ddsd.dwHeight		= WNDSIZEH;
	
	if(FAILED(m_lpDD->CreateSurface(&ddsd, &m_lpDDBack, NULL)))
	{
		MessageBox(m_hWnd, "DirectDraw : BackBuffer Surface ���� ����", "Error", MB_OK);
		return false;
	}
#endif

	// Create the clipper using the DirectDraw object 

	LPDIRECTDRAWCLIPPER		lpDDClipper;
	m_lpDD->CreateClipper(0, &lpDDClipper, NULL); 

	// Assign your window's HWND to the clipper 
	lpDDClipper->SetHWnd(0, m_hWnd); 

	// Attach the clipper to the primary surface 
	m_lpDDPrimary->SetClipper(lpDDClipper); 
	
	return true;
}

bool gMainWin::DisableBeep()
{
	// sound beep�� ����		- Registry �̿�.
	HKEY		key;
	DWORD		dwDisp;
	
	if(RegCreateKeyEx(HKEY_CURRENT_USER, BEEPHACKREG, 0, NULL,
		REG_OPTION_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp) != ERROR_SUCCESS)
		return false;
	
	if(RegSetValueEx(key, "Beep", 0, REG_SZ, (LPBYTE)"no", 3) != ERROR_SUCCESS)
		return false;
	
	RegCloseKey(key);
	
	// sound beep��... edit control �̳��̴���.
	// %SystemRoot%\media\Windows XP Ding.wav
	
	if(RegCreateKeyEx(HKEY_CURRENT_USER, BEEPHACKREG2, 0, NULL,
		REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &dwDisp) != ERROR_SUCCESS)
		return false;
	
	dwDisp = 128;
	
	if(RegQueryValueEx(key, NULL, 0, NULL, 
		(LPBYTE)m_szTempReg, &dwDisp) != ERROR_SUCCESS)
		return false;
	
	RegCloseKey(key);
	
	if(RegCreateKeyEx(HKEY_CURRENT_USER, BEEPHACKREG2, 0, NULL,
		REG_OPTION_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp) != ERROR_SUCCESS)
		return false;
	
	if(RegSetValueEx(key, NULL, 0, REG_EXPAND_SZ, (LPBYTE)"", 1) != ERROR_SUCCESS)
		return false;
	
	RegCloseKey(key);

	return true;
}

bool gMainWin::EnableBeep()
{
	// sound beep�� ���� ����	- Registry �̿�.
	HKEY		key;
	DWORD		dwDisp;
	
	if(RegCreateKeyEx(HKEY_CURRENT_USER, BEEPHACKREG, 0, NULL,
		REG_OPTION_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp) != ERROR_SUCCESS)
		return false;
	
	if(RegSetValueEx(key, "Beep", 0, REG_SZ, (LPBYTE)"yes", 4) != ERROR_SUCCESS)
		return false;
	
	RegCloseKey(key);
	
	if(RegCreateKeyEx(HKEY_CURRENT_USER, BEEPHACKREG2, 0, NULL,
		REG_OPTION_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp) != ERROR_SUCCESS)
		return false;
	
	if(RegSetValueEx(key, "", 0, REG_EXPAND_SZ, (LPBYTE)m_szTempReg, strlen(m_szTempReg) + 1) != ERROR_SUCCESS)
		return false;
	
	RegCloseKey(key);

	return true;
}