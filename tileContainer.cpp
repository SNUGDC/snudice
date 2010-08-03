#include "tileContainer.h"
#include "TILE.h"
#include "gGameCore.h"
#include "const.h"
#include "gMainWin.h"
#include "gUtil.h"
#include "gMouse.h"
#include "gPlayerManager.h"
#include "gTimer.h"
#include "gInterface.h"
// sangwoo temp
// #include "itemContainer.h"
// temp end

static tileContainer s_tileContainer; // 2

tileContainer *tileContainer::GetIF() // 3
{
	return &s_tileContainer;
}

tileContainer::tileContainer()
{
	
}

tileContainer::~tileContainer()
{

}


// 1. Setup Line

void tileContainer::Setup()
{
	gGameCore *ggameCore = gGameCore::GetIF();
	
	POINT temp;
	for(int i = 0 ; i < LINEX ; i++){
		for (int j = 0 ; j < LINEY ; j++){
			tileMap[i*LINEY+j].init(i,j);
		}
	}
	gimage[0].Load(".\\Data\\Map\\food.img");
	gimage[1].Load(".\\Data\\Map\\card.img");
	gimage[2].Load(".\\Data\\Map\\nokdu.img");
	gimage[3].Load(".\\Data\\Map\\bus.img");
	gimage[4].Load(".\\Data\\Map\\maingate.img");
	gimage[5].Load(".\\Data\\Map\\mountain.img");
	gimage[6].Load(".\\Data\\Map\\lang.img");
	gimage[7].Load(".\\Data\\Map\\math.img");
	gimage[8].Load(".\\Data\\Map\\art.img");
	
	m_wallpaper.Load(TITLE_IMG_TITLE);
	m_wallpaperSubmit.Load(SUBMIT_IMG_TITLE);
	m_buttonSubmit.Load(SUBMIT_IMG_BUTTON);

	m_xSpacePos=-1;
	m_ySpacePos=-1;
	m_Next_xSpacePos = -1;
	m_Next_ySpacePos = -1;
	m_subjectN=0;
	m_selectReadySubjectFlag = -1;


	Load();
	posSpacor();
	temp.x = m_xSpacePos;
	temp.y = m_ySpacePos;
	temp = conToAbs(temp);
	ggameCore->PutScreenPos(temp.x- WNDSIZEW/2 + HALFX,temp.y- WNDSIZEH/2 + HALFY);

}

void tileContainer::LoadFileToBKS()
{
	int i;
		
	hFile=CreateFile("load.xy",GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile!=INVALID_HANDLE_VALUE) {
		ReadFile(hFile,&count,sizeof(int),&dw,NULL);
		for(i = 0 ; i < count ; i++){
			memset(&bmpKindSavor[i],0,sizeof(TILE));
			ReadFile(hFile,&bmpKindSavor[i],sizeof(TILE),&dw,NULL);
		}
	}
	CloseHandle(hFile);
}




void tileContainer::LoadBKSToTM()
{
	gMainWin *gmainWin = gMainWin::GetIF();
	int i;
	m_xInitSpacePos=-1;
	m_yInitSpacePos=-1;

	int localcount = 0;
	for(i = 0 ; i < MAXSAVOR_N ; i++)
		memset(&tileMap[i],0,sizeof(TILE));
	for(i = 0 ; i < LINEX ; i++) {
		for (int j = 0 ; j < LINEY ; j++) {
			if(i == bmpKindSavor[localcount].ptPos.x && j == bmpKindSavor[localcount].ptPos.y){
				tileMap[i*LINEY+j] = bmpKindSavor[localcount];
				localcount++;
				if(tileMap[i*LINEY+j].tileType==TY_MAINGATE){
					m_xInitSpacePos = i;
					m_yInitSpacePos = j;
				}
				if(tileMap[i*LINEY+j].tileType==TY_CLASS){
					if(flagToRow(tileMap[i*LINEY+j].flag2)==-1){
						m_subject[m_subjectN]=tileMap[i*LINEY+j].flag2;
						m_subjectN++;
					}

				}
			}
			else tileMap[i*LINEY+j].init(i,j);
		}
	}
	if(m_xInitSpacePos==-1&&m_yInitSpacePos==-1){
		MessageBox(gmainWin->m_hWnd, "Tile Info Error : There is no MainGate", "Error", MB_OK);
	}
}

void tileContainer::Load()
{
	LoadFileToBKS();
	LoadBKSToTM();
}

// 1. Setup Line End


// 2. Draw Line Start
void tileContainer::DrawSubmitButtonOne (int x0,int y0,int type){
	RECT rcDest = {x0,y0,x0+30,y0+20};
	RECT rcSour = {type*30,0,(type+1)*30,20};
	m_buttonSubmit.Draw(rcDest,rcSour,true); // true나 false나 상관 없겠지. 뭔진 기억 안나지만.
}

void tileContainer::DrawSubmitButton(int io,int tile){
	gPlayerManager *gplayerManager = gPlayerManager::GetIF();
	int flag = rowToFlag(io);
	int i,j;
	int type[3];
	i=0;
	
	//for( i = 0 ; i < 2 /*tileMap[tile].flag3*/ ; ){
		for( j = 0 ; j < MAXPLAYER ; j++){
			if(gplayerManager->m_player[j].m_nNP!=-1&&gplayerManager->m_player[j].isMySubject(flag)){
				type[i] = j;
				i++;
			}
		}
	//} 
	for( ; i < 2 /*tileMap[tile].flag3*/ ; i++){
		type[i] = 4;
	}
	for( ; i < 3 ; i++){
		type[i] = 5;
	}

	for (i = 0 ; i < 3 ; i++){
		DrawSubmitButtonOne(i*32+135,io*22+50-3,type[i]);
	}
}

void tileContainer::DrawSubmit(){	// 좌표 하드코딩의 절정
									// DrawSubmit과 gGameCore::OnLButtonDownSubmit() 이 연동되는데 하드코딩 상황이다.
//	itemContainer* itemcontainer = itemContainer::GetIF();
	char buf[128];
	

	int i;
	gGameCore *gameCore = gGameCore::GetIF();
	gPlayerManager *gplayerManager = gPlayerManager::GetIF();
	int l_selectSubject;
		RECT tempRC;
		int tile;
	for(i = 0 ; i < m_subjectN ; i++){
		tile = rowToFirstTile(i);
		DrawSubmitButton(i,tile);
	}

	gUtil::BeginText();
	// sangwoo temp start
	/*
	for(i = 0 ; i < 4 ; i++){
	wsprintf(buf,"%d",itemcontainer->m_item[i].m_index);
	gUtil::Text(i*100,0,buf);
	wsprintf(buf,"%d",itemcontainer->m_item[i].m_effect);
	gUtil::Text(i*100,20,buf);
	wsprintf(buf,"%d",itemcontainer->m_item[i].m_flagHigh);
	gUtil::Text(i*100,40,buf);
	wsprintf(buf,"%d",itemcontainer->m_item[i].m_flagLow);
	gUtil::Text(i*100,60,buf);
	wsprintf(buf,"%d",itemcontainer->m_item[i].m_activeType);
	gUtil::Text(i*100,80,buf);
	wsprintf(buf,"%d",itemcontainer->m_item[i].m_turn);
	gUtil::Text(i*100,100,buf);	
	}
	*/
	// sangwoo temp end
	
	for (i = 0 ; i < m_subjectN ; i++){
		//m_buttonSubmit.Draw(0,0);
		tile = rowToFirstTile(i);
		if(tile>=0)
			gUtil::Text(15,i*22+50,tileMap[tile].subject);
		else
			gUtil::Text(15,i*22+50,"you are simang");
		
		
	}
	
	l_selectSubject=gameCore->m_selectSubject;
	if(l_selectSubject>=0){
		tile = flagToFirstTile(l_selectSubject);
		if(tile!=-1){
			gUtil::Text(320,0,tileMap[tile].college);
			gUtil::Text(320,20,tileMap[tile].building);
			gUtil::Text(320,40,tileMap[tile].subject);
		}	
	}
	/*	// 이걸 지운다는 것이 무슨 의미인지를 이해하지 못한 한 절대 지우지 말 것
	for(i = 0 ; i < gplayerManager->m_player[gameCore->m_turnPlayer].m_subjectGrader.m_subjectN ; i++) {
		tile=flagToFirstTile(gplayerManager->m_player[gameCore->m_turnPlayer].m_subjectGrader.m_subject[i]);
		if(tile!=-1)
			gUtil::Text(160,i*20,tileMap[tile].subject);
	}
*/
	gUtil::EndText();



	
	DrawHexagon(240,200,6,true);

	if(m_selectReadySubjectFlag!=-1){
		int tile;
		int boole;
		
		if(gameCore->m_frameCount>0)	boole=0;
		else							boole=1;

		tile = flagToFirstTile(m_selectReadySubjectFlag);
		DrawHexagonOne(240,200,tileMap[tile].ptPos.x,tileMap[tile].ptPos.y,6,true,boole);
		tile = flagToSecondTile(m_selectReadySubjectFlag);
		DrawHexagonOne(240,200,tileMap[tile].ptPos.x,tileMap[tile].ptPos.y,6,true,boole);
		
		//DrawHexagonOne(160,160,tileMap[tile].ptPos.x,tileMap[tile].ptPos.y,5,true,0);
		
	}


	tempRC.left = 480;
	tempRC.top = 0;
	tempRC.right = 640;
	tempRC.bottom = 240;	
	gplayerManager->m_player[gameCore->m_turnPlayer].m_charInfo->DrawIllu(tempRC); // 여기서 지금 막혓음 problem10 (푼거 아님?)
		
}

void tileContainer::DrawHexagonOne(int x0,int y0,int i,int j,int n,bool boolo,int type){
	gGameCore *gameCore = gGameCore::GetIF();
	
	int k;
	RECT a;
	RECT b;

	k = tileMap[i*LINEY+j].tileType;
	if(k==TY_NONE) return;
	
	if(i%2==0){
		a.left = WIDEX*i/2;
		a.top = FULLY*j;
	}
	else{
		a.left = LEFTX + MIDDLEX + WIDEX*(i-1)/2;
		a.top = HALFY + FULLY*j;
	}
	if(n==1){
		a.left -= gameCore->m_xPos;
		a.top -= gameCore->m_yPos;
	}
	
	a.left /= n;
	a.top /= n;
	a.left += x0;
	a.top += y0;
	a.right = a.left + FULLX/n;
	a.bottom = a.top + FULLY/n;
	
	
	if(!type) SetRect(&b,0,0,FULLX,FULLY);
	else SetRect(&b,FULLX,0,FULLX*2,FULLY);
	
	if(k==TY_CLASS) gimage[k+tileMap[i*LINEY+j].flag1].Draw(a,b,boolo);
	else gimage[k].Draw(a,b,boolo);
	

}

void tileContainer::DrawHexagon(int x0,int y0,int n,bool boolo){


	int i, j;

	
	for(i = 0 ; i < LINEX ; i++) { 
		for(j = 0 ; j < LINEY ; j++) {
			DrawHexagonOne(x0,y0,i,j,n,boolo);
		}
	}
}

void tileContainer::DrawHexagonBus()
{
	gTimer *gtimer = gTimer::GetIF();
	gGameCore *gameCore = gGameCore::GetIF();

	int i,j;
	
	for(i = 0 ; i < LINEX ; i++) { 
		for(j = 0 ; j < LINEY ; j++) {
			if(tileMap[i*LINEY+j].tileType==TY_BUS)
				DrawHexagonOne(0,0,i,j,1,true,gtimer->frame());
		}
	}
}

void tileContainer::Draw()
{
//	int i,j;


	
	int n = 10; // 축소율
	int start_x =0;
	int start_y = 0;
	
	gGameCore *gameCore = gGameCore::GetIF();
	gTimer *gtimer = gTimer::GetIF();


	

	if(gameCore->m_gMode==EGM_SUBMIT){
		m_wallpaperSubmit.Draw(0,0);
		DrawSubmit();
		return;
	}
	else{	
		m_wallpaper.Draw(0,0);
		DrawHexagon(0,0,1);
		
		if(gameCore->m_busMode==1&&gtimer->m_on)
			DrawHexagonBus();
	
		// 빈 사각형은 어떻게 띄울 것인가?
		if(gameCore->m_minimapOn==1||gameCore->m_minimapOn==2){
			m_wallpaper.Draw(WNDSIZEW - 240,WNDSIZEH - 180);
			DrawHexagon( WNDSIZEW - 240 , WNDSIZEH - 180 , n ,true);
			//minimapDraw(start_x,start_y,n);
		}
		SetUpSubInfo();
	}	
}

void tileContainer::SetUpSubInfo()
{
	gGameCore *ggameCore = gGameCore::GetIF();
	
	POINT Abs;
	POINT Con2;
	Con2 = absToCon();
	if (isExisted(Con2.x,Con2.y))
	{
		gInterface	*inf = gInterface::GetIF();
		int		x, y;
		int		i;

		Abs = conToAbs(Con2);
		x = Abs.x - (ggameCore->m_xPos) + 50;
		y = Abs.y - (ggameCore->m_yPos) + 40;

		if(inf->m_tooltip.nX != x || inf->m_tooltip.nY != y || !inf->m_bTooltip)
		{
			TILE		*tile = &tileMap[Con2.x * LINEY + Con2.y ];
			inf->m_tooltip.szSubject	= tile->subject;
			inf->m_tooltip.szBuilding	= tile->building;
			inf->m_tooltip.szColleage	= tile->college;

			switch(tileMap[Con2.x * LINEY + Con2.y ].tileType)
			{
				case TY_CLASS:
					{
						switch(tile->flag1)
						{
							case 0:							// lang
								inf->m_tooltip.type	= ETIP_LANG;
								break;
							case 1:							// math
								inf->m_tooltip.type	= ETIP_MATH;
								break;
							case 2:							// art
								inf->m_tooltip.type	= ETIP_ART;
								break;
							break;
						}

						int		nCount = 0;

						for(i = 0; i < MAXPLAYER; i++)
						{
							if(gPlayerManager::GetIF()->m_player[i].m_nNP == -1) continue;

							gPlayer		*player = &gPlayerManager::GetIF()->m_player[i];

							if(player->isMySubject(tile->flag2))
							{
								inf->m_tooltip.pPlayer[nCount++]	= player;
								inf->m_tooltip.nIndex				= tile->flag2;
							}
						}
						break;
					}
				case TY_ITEM:
					inf->m_tooltip.type	= ETIP_ITEM;
					break;
				case TY_BUS:
					inf->m_tooltip.type	= ETIP_BUS;
					break;
				case TY_DRINK:
					inf->m_tooltip.type	= ETIP_DRINK;
					break;
				case TY_MAINGATE:
					inf->m_tooltip.type	= ETIP_MAINGATE;
					break;
				case TY_MOUNTAIN:
					inf->m_tooltip.type	= ETIP_MOUNTAIN;
					break;
				case TY_STAMINA:
					inf->m_tooltip.type	= ETIP_FOOD;
					break;
			}
			inf->m_tooltip.nX			= x;
			inf->m_tooltip.nY			= y;
			inf->m_bTooltip				= true;
		}

// 		gUtil::Text(Abs.x-(ggameCore->m_xPos),Abs.y-(ggameCore->m_yPos)+FULLY,tileMap[Con2.x*LINEY+Con2.y].college);
// 		gUtil::Text(Abs.x-(ggameCore->m_xPos),Abs.y-(ggameCore->m_yPos)+FULLY+20,tileMap[Con2.x*LINEY+Con2.y].building);
// 		gUtil::Text(Abs.x-(ggameCore->m_xPos),Abs.y-(ggameCore->m_yPos)+FULLY+40,tileMap[Con2.x*LINEY+Con2.y].subject);
	}
	else
	{
		gInterface::GetIF()->ClearTooltip();
	}
}

// 2. Draw Line End


// 3. pos Line Start
// posSpacor() - start, posMovor() - On, posStoper() - End
void tileContainer::posSpacor() 
{	// 다음 칸으로 움직이라는 신호.
	gGameCore *gameCore = gGameCore::GetIF();
	gPlayerManager *gplayerManager = gPlayerManager::GetIF();

	if(m_xSpacePos==-1&&m_xSpacePos==-1){
		m_xSpacePos=m_xInitSpacePos;
		m_ySpacePos=m_yInitSpacePos;
		m_Next_xSpacePos = m_xSpacePos;
		m_Next_ySpacePos = m_ySpacePos;
	}
	else{
		if(isExisted(m_xSpacePos,m_ySpacePos)){
			if(m_xInitSpacePos == m_xSpacePos && m_yInitSpacePos == m_ySpacePos&&gplayerManager->m_player[gameCore->m_turnPlayer].m_isNokdu){
				m_Next_xSpacePos = tileMap[m_xSpacePos*LINEY+m_ySpacePos].flag1; // gplayerManager->m_player[gameCore->m_turnPlayer]->m_charInfo;
				m_Next_ySpacePos = tileMap[m_xSpacePos*LINEY+m_ySpacePos].flag2; // gplayerManager->m_player[gameCore->m_turnPlayer];
			}
			else {
			m_Next_xSpacePos = tileMap[m_xSpacePos*LINEY+m_ySpacePos].nextTile.x;	// 기본 방침은, Next와 Now가 괴리가 있는 상황은 움직이는 상황인 것이다.
			m_Next_ySpacePos = tileMap[m_xSpacePos*LINEY+m_ySpacePos].nextTile.y;
			}
		}
	}
}

void tileContainer::posSpacor(int xSpacePos,int ySpacePos) 
{	// 다음 칸으로 움직이라는 신호.
	m_Next_xSpacePos = xSpacePos;
	m_Next_ySpacePos = ySpacePos;
	/*
	gGameCore *gameCore = gGameCore::GetIF();
	gPlayerManager *gplayerManager = gPlayerManager::GetIF();
	
	if(m_xSpacePos==-1&&m_xSpacePos==-1){
		m_xSpacePos=m_xInitSpacePos;
		m_ySpacePos=m_yInitSpacePos;
		m_Next_xSpacePos = m_xSpacePos;
		m_Next_ySpacePos = m_ySpacePos;
	}
	else{
		if(isExisted(m_xSpacePos,m_ySpacePos)){
			if(m_xInitSpacePos == m_xSpacePos && m_yInitSpacePos == m_ySpacePos&&gplayerManager->m_player[gameCore->m_turnPlayer].m_isNokdu){
				m_Next_xSpacePos = tileMap[m_xSpacePos*LINEY+m_ySpacePos].flag1; // gplayerManager->m_player[gameCore->m_turnPlayer]->m_charInfo;
				m_Next_ySpacePos = tileMap[m_xSpacePos*LINEY+m_ySpacePos].flag2; // gplayerManager->m_player[gameCore->m_turnPlayer];
			}
			else {
				m_Next_xSpacePos = tileMap[m_xSpacePos*LINEY+m_ySpacePos].nextTile.x;	// 기본 방침은, Next와 Now가 괴리가 있는 상황은 움직이는 상황인 것이다.
				m_Next_ySpacePos = tileMap[m_xSpacePos*LINEY+m_ySpacePos].nextTile.y;
			}
		}
	}
	*/
}

void tileContainer::posMover(int frame)
{
	gGameCore *ggameCore = gGameCore::GetIF();
	gTimer *gtimer = gTimer::GetIF();
	POINT b, a;
	b.x = m_Next_xSpacePos;
	b.y = m_Next_ySpacePos;
	a.x = m_xSpacePos;
	a.y = m_ySpacePos;
	b = conToAbs(b);
	a = conToAbs(a);
	b.x = b.x - WNDSIZEW/2 + HALFX;
	b.y = b.y - WNDSIZEH/2 + HALFY;
	a.x = a.x - WNDSIZEW/2 + HALFX;
	a.y = a.y - WNDSIZEH/2 + HALFY;

	ggameCore->PutScreenPos(a.x + frame*(b.x-a.x)/gtimer->m_frame,a.y + frame*(b.y-a.y)/gtimer->m_frame);
	
}

void tileContainer::posStoper()
{
	gPlayerManager *gplayerManager = gPlayerManager::GetIF();
	gGameCore *gameCore = gGameCore::GetIF();
	m_xSpacePos = m_Next_xSpacePos;	// 기본 방침은 Next와 Now가 같은 상황은 멈춘 상황이라는 것이다.
	m_ySpacePos = m_Next_ySpacePos; // 기본 방침은 Pos는 Con 값이라는 것이다.
	
}

// 3. Pos Line End


// 4. Essential for All Line Start

bool tileContainer::isExisted(int i, int j)
{
	if(i>-1&&j>-1&&i<LINEX&&j<LINEY){
		if(tileMap[i*LINEY+j].tileType!=TY_NONE) return true;
	}
	return false;
}
bool tileContainer::isExisted(int line)
{
	if(line<0||line>=LINEX*LINEY) return false;
	else if(tileMap[line].tileType!=TY_NONE) return true;
	else return false;
}


POINT tileContainer::conToAbs(POINT ij)
{
	POINT res;
	if(ij.x%2==0){
		res.x = WIDEX*(ij.x/2);
		res.y = FULLY*ij.y;
	}
	else{
		res.x = LEFTX+MIDDLEX+WIDEX*(ij.x-1)/2;
		res.y = HALFY+FULLY*ij.y;
	}
	return res;
}

POINT tileContainer::absToCon() 
{
	gGameCore *ggameCore = gGameCore::GetIF();
	gMouse *gmouse = gMouse::GetIF();
	
	POINT res;
	
	int n,m;
	int xo,yo;
	n = (ggameCore->m_xPos + gmouse->m_nPosX) / WIDEX;
	n = n*2;
	m = (ggameCore->m_yPos + gmouse->m_nPosY) / FULLY;
	xo = (ggameCore->m_xPos + gmouse->m_nPosX) % WIDEX;
	yo = (ggameCore->m_yPos + gmouse->m_nPosY) % FULLY;
	

	if(-1 * HALFY * xo + HALFY * LEFTX > LEFTX * yo) { n--; m--; } 
	else if(HALFY * xo + HALFY * LEFTX <= LEFTX * yo ) { n--; }
	else if(yo < HALFY && HALFY * xo + ( HALFY * LEFTX - HALFY * FULLX ) > LEFTX * yo ) { n++; m--; }
	else if(yo >= HALFY && - HALFY * xo + ( HALFY * LEFTX + HALFY * FULLX ) <= LEFTX*yo ) { n++; }
	res.x = n;
	res.y = m;
	return res;
	
}

POINT tileContainer::absToCon(POINT ij) 
{
	/*
	gGameCore *ggameCore = gGameCore::GetIF();
	gMouse *gmouse = gMouse::GetIF();
	*/
	POINT res;
	
	int n,m;
	int xo,yo;
	n = (ij.x) / WIDEX;
	n = n*2;
	m = (ij.y) / FULLY;
	xo = (ij.x) % WIDEX;
	yo = (ij.y) % FULLY;
	
	
	if(-1 * HALFY * xo + HALFY * LEFTX > LEFTX * yo) { n--; m--; } 
	else if(HALFY * xo + HALFY * LEFTX <= LEFTX * yo ) { n--; }
	else if(yo < HALFY && HALFY * xo + ( HALFY * LEFTX - HALFY * FULLX ) > LEFTX * yo ) { n++; m--; }
	else if(yo >= HALFY && - HALFY * xo + ( HALFY * LEFTX + HALFY * FULLX ) <= LEFTX*yo ) { n++; }
	res.x = n;
	res.y = m;
	return res;
	
}

// 4. Essential for All Line End


// 5. Class functions start

int tileContainer::flagToRow (int index)
{
	for(int i = 0 ; i < m_subjectN ; i++){
		if(m_subject[i]==index) return i;
	}
	return -1;
}

int tileContainer::rowToFlag (int row)
{
	for(int i = 0 ; i < m_subjectN ; i++){
		if(m_subject[row]==i) return i;
	}
}

int tileContainer::rowToFirstTile(int row)
{
	for(int i = 0 ; i < LINEX*LINEY ; i++){
		if(tileMap[i].tileType==TY_CLASS&&tileMap[i].flag2==m_subject[row]) return i;
	}
	return -1;
}

int tileContainer::rowToSecondTile(int row)
{
	int i;
	for(i = 0 ; i < LINEX*LINEY ; i++){
		if(tileMap[i].tileType==TY_CLASS&&tileMap[i].flag2==m_subject[row]) break;
	}
	i++;
	for( ; i < LINEX*LINEY ; i++){
		if(tileMap[i].tileType==TY_CLASS&&tileMap[i].flag2==m_subject[row]) return i;
	}
	return -1;
}


int tileContainer::flagToFirstTile(int index)
{
	int row = flagToRow(index);
	return rowToFirstTile(row); // 실은 더 쉬운 방법이 있지만; (이게 뭐하는 짓이냐 당췌. firsttile이 flag를 가지고 있는데.
	//return -1;
}

int tileContainer::flagToSecondTile(int index)
{
	int row = flagToRow(index);
	return rowToSecondTile(row); // 실은 더 쉬운 방법이 있지만; (이게 뭐하는 짓이냐 당췌. firsttile이 flag를 가지고 있는데.
	//return -1;
}


// 5. Class functions end

void tileContainer::Release()
{
	int		i;

	for(i = 0; i < 9; i++)
		gimage[i].Release();

	m_wallpaper.Release();
	m_wallpaperSubmit.Release();
	m_buttonSubmit.Release();
}

int tileContainer::busClickProcessor(int x,int y){	// output은 tile 정보
	gMainWin *gmainWin = gMainWin::GetIF();
	POINT xy;

	xy.x = x;
	xy.y=  y;
	xy = absToCon(xy);
	if(xy.x == 2 && xy.y == 17) {
		MessageBox(gmainWin->m_hWnd, "당신의 이성이 그곳으로 가는 것을 막았습니다", "Error", MB_OK);
		return -1; //if(tileMap[2*LINEY+17])
	}
	if(tileMap[xy.x*LINEY+xy.y].tileType==TY_BUS){
		return xy.x*LINEY+xy.y;
	}	
	return -1;

}

int tileContainer::distance(int mapA,int mapB){	// 치명적인 녹두 문제. 녹두 들어가면 무한루프 때문에 그 오류를 알 수 있음
	int localCount= 0;
	int iter=mapA;
	// sangwoo temp
	
	/*
	do {
		iter = tileMap[iter].nextTile.x*LINEY+tileMap[iter].nextTile.y;
		localCount++;
	} while(iter!=mapB);
	*/
		// sangwoo temp end
	
	
	if(mapA==mapB) return 0;
	
	while(iter!=mapB) {
		iter = tileMap[iter].nextTile.x*LINEY+tileMap[iter].nextTile.y;
		localCount++;
	} 
	
	localCount++;
	return localCount;
}