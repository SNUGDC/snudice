#include <windows.h>
#include "gPopUp.h"
#include "gUtil.h"
#include "gMainWin.h"
#include "gMouse.h"


gPopUp::gPopUp()
{

}

gPopUp::~gPopUp()
{

}

static gPopUp	s_PopUp;

gPopUp *gPopUp::GetIF()
{
	return &s_PopUp;
}


bool gPopUp::SetUp()
{
	m_bPopup	= false;
	m_bReturn	= false;
	m_Img		= NULL;
	
	if(FAILED(m_ImgPopup.Load(POPUP_IMG_BACK)))
		return false;
	
	RECT		rc;
	// ��ư ��ġ�� ���Ŀ� ����
	if(FAILED(m_ImgBtn[ECLK_OK].SetUp(POPUP_IMG_OK, false, rc)))
		return false;
	if(FAILED(m_ImgBtn[ECLK_CANCEL].SetUp(POPUP_IMG_CANCEL, false, rc)))
		return false;

	return true;
}

bool gPopUp::SetPopUp(BTNCLK btn, char *szLine1, char *szLine2)
{
	if(m_bPopup)
		return false;
	
	m_eBtnClk = btn;
	m_bReturn = false;

	switch(btn)
	{
		case ECLK_OK:			// just one ok button
			SetRect(&m_ImgBtn[ECLK_OK].m_rcPos,
				POPUP_ONEBTNX,
				POPUP_ONEBTNY,
				POPUP_ONEBTNX + POPUP_BTNW,
				POPUP_ONEBTNY + POPUP_BTNH	);
			break;
		case ECLK_CANCEL:		// two button, ok and cancel button
			SetRect(&m_ImgBtn[ECLK_OK].m_rcPos,
				POPUP_1STBTNX,
				POPUP_1STBTNY,
				POPUP_1STBTNX + POPUP_BTNW,
				POPUP_1STBTNY + POPUP_BTNH	);
			
			SetRect(&m_ImgBtn[ECLK_CANCEL].m_rcPos,
				POPUP_2NDBTNX,
				POPUP_2NDBTNY,
				POPUP_2NDBTNX + POPUP_BTNW,
				POPUP_2NDBTNY + POPUP_BTNH	);
			break;
	}
	if(szLine1)
		strcpy(m_szLine1, szLine1);
	if(szLine2)
		strcpy(m_szLine2, szLine2);
	
	TextPosX();
	
	m_bPopup = true;
	return true;
}

bool gPopUp::SetImgPopUp(BTNCLK btn, gImage *img, char *szLine1, char *szLine2)
{
	m_Img		= img;

	bool ret = SetPopUp(btn, szLine1, szLine2);

	return ret;
}

void gPopUp::Release()
{
	m_ImgPopup.Release();
	m_ImgBtn[ECLK_OK].Release();
	m_ImgBtn[ECLK_CANCEL].Release();
}

void gPopUp::TextPosX()
{
	int		length;
	
	if(m_szLine1 != NULL)
	{
		length = strlen(m_szLine1);
		length *= 6;		// ���� �ϳ��� 6pixel�̶�� ����(�ѱ۱���)

		if(m_Img)
			m_nLine1X = POPUP_X + POPUP_IMG_W + (POPUP_WIDTH - POPUP_IMG_W - length) / 2;
		else
			m_nLine1X = POPUP_X + (POPUP_WIDTH - length) / 2;
	}

	if(m_szLine2 != NULL)
	{
		length = strlen(m_szLine2);
		length *= 6;		// ���� �ϳ��� 6pixel�̶�� ����(�ѱ۱���)
		m_nLine2X = POPUP_X + (POPUP_WIDTH - length) / 2;

		if(m_Img)
			m_nLine2X = POPUP_X + POPUP_IMG_W + (POPUP_WIDTH - POPUP_IMG_W - length) / 2;
		else
			m_nLine2X = POPUP_X + (POPUP_WIDTH - length) / 2;	}
}

bool gPopUp::isPopUp()
{
	return m_bPopup;
}

void gPopUp::MainLoop()
{
	if(!m_bPopup) return;

	if(gMainWin::GetIF()->m_Keys[VK_RETURN])	// ok
	{
		switch(m_eBtnClk)
		{
			case ECLK_OK:
			case ECLK_CANCEL:
				Ok();
				break;
		}
	}
	else if(gMainWin::GetIF()->m_Keys[VK_ESCAPE])	// cancel
	{
		switch(m_eBtnClk)
		{
			case ECLK_OK:
				break;
			case ECLK_CANCEL:
				Cancel();
				break;
		}			
	}
	Draw();
}

void gPopUp::Ok()
{
	m_eBtnClk	= ECLK_OK;
	m_bPopup	= false;
	memset(m_szLine1, 0, 128);
	memset(m_szLine2, 0, 128);
	m_bReturn	= true;
	m_Img		= NULL;
	m_ImgBtn[ECLK_OK].m_eBtnMode = EBM_NONE;
	m_ImgBtn[ECLK_CANCEL].m_eBtnMode = EBM_NONE;
}

void gPopUp::Cancel()
{
	m_eBtnClk	= ECLK_CANCEL;
	m_bPopup	= false;
	memset(m_szLine1, 0, 128);
	memset(m_szLine2, 0, 128);
	m_bReturn	= true;
	m_Img		= NULL;
	m_ImgBtn[ECLK_OK].m_eBtnMode = EBM_NONE;
	m_ImgBtn[ECLK_CANCEL].m_eBtnMode = EBM_NONE;
}

void gPopUp::Draw()
{
	if(!m_bPopup)
		return;

	m_ImgPopup.Draw(POPUP_X, POPUP_Y);

	switch(m_eBtnClk)
	{
		case ECLK_OK:
			m_ImgBtn[ECLK_OK].Draw();
			break;
		case ECLK_CANCEL:
			m_ImgBtn[ECLK_OK].Draw();
			m_ImgBtn[ECLK_CANCEL].Draw();
			break;
	}

	if(m_Img != NULL)
		m_Img->Draw(POPUP_IMG_X, POPUP_IMG_Y);

	gUtil::BeginText();
		if(m_szLine1)
			gUtil::Text(m_nLine1X, POPUP_1STTEXTY, m_szLine1);
		if(m_szLine2)
			gUtil::Text(m_nLine2X, POPUP_2NDTEXTY, m_szLine2);
	gUtil::EndText();
}

void gPopUp::OnLButtonDown()
{
	if(!m_bPopup)
		return;

	gMouse	*mouse = gMouse::GetIF();

	switch(m_eBtnClk)
	{
		case ECLK_OK:
			if(m_ImgBtn[ECLK_OK].PointInButton(mouse->m_nPosX, mouse->m_nPosY))
				Ok();
			break;
		case ECLK_CANCEL:
			if(m_ImgBtn[ECLK_OK].PointInButton(mouse->m_nPosX, mouse->m_nPosY))
				Ok();
			else if(m_ImgBtn[ECLK_CANCEL].PointInButton(mouse->m_nPosX, mouse->m_nPosY))
				Cancel();
			break;
	}
}


void gPopUp::OnMouseMove()
{
	if(!m_bPopup)
		return;
	
	gMouse	*mouse = gMouse::GetIF();
	
	switch(m_eBtnClk)
	{
		case ECLK_OK:
			if(m_ImgBtn[ECLK_OK].PointInButton(mouse->m_nPosX, mouse->m_nPosY))
				m_ImgBtn[ECLK_OK].m_eBtnMode = EBM_HOVER;
			else
				m_ImgBtn[ECLK_OK].m_eBtnMode = EBM_NONE;
			break;
		case ECLK_CANCEL:
			if(m_ImgBtn[ECLK_OK].PointInButton(mouse->m_nPosX, mouse->m_nPosY))
				m_ImgBtn[ECLK_OK].m_eBtnMode = EBM_HOVER;
			else
				m_ImgBtn[ECLK_OK].m_eBtnMode = EBM_NONE;

			if(m_ImgBtn[ECLK_CANCEL].PointInButton(mouse->m_nPosX, mouse->m_nPosY))
				m_ImgBtn[ECLK_CANCEL].m_eBtnMode = EBM_HOVER;
			else
				m_ImgBtn[ECLK_CANCEL].m_eBtnMode = EBM_NONE;
			break;
	}
}

void gPopUp::OnRButtonDown()
{

}

void gPopUp::OnLButtonUp()
{
	
}