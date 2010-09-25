//------------------------------------------------------------------------------------
//	gGameCore Class
//
//	2010. 09. 23	CoderK
//
//	메인 게임모드
//------------------------------------------------------------------------------------
#pragma once
#include "networkconst.h"
#include "Core.h"

class gGameCore : Core
{
public:
	static gGameCore *GetIF();

	gGameCore(void);
	~gGameCore(void);

public:
	int			m_nTurn;		// 누구 턴
	int			m_spacor;
	bool		m_bMoved;		// 해당 턴에 움직엿냐, 즉 movestart_ask 보냇으면 true
	bool		m_bMoving;		// 움직이는 중이냐

public:

	bool		PreTransMsg(MSG &msg);

	bool		SetUp();
	void		MainLoop();
	void		Draw();
	void		Release();

	void		OnLButtonDown();
	void		OnLButtonUp();
	void		OnMouseMove();
	void		OnRButtonDown();

	void		pk_movestart_rep(PK_MOVESTART_REP *rep);
	void		pk_nextturn_rep(PK_NEXTTURN_REP *rep);

	void		Start(int spacor);
	void		Start(int spacor,int conPosX,int conPosY);
	void		StepStart();
	void		StepOn();
	void		StepEnd();
	void		End();

	void		SendMoveAsk();
private:
	void		MainLoopMouse();

};
