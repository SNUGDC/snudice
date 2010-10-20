#include <stdio.h>
#include "utilconst.h"
#include "Util.h"
#include "MainWin.h"

HDC		s_hdc;
HFONT	s_font;
HFONT	s_ofont;
int		s_nSize			= UTIL_DEFAULT_FONTSIZE;
char	s_szFont[32]	= UTIL_DEFAULT_FONT;

void gUtil::BeginText()
{
	gMainWin::GetIF()->m_lpDDBack->GetDC(&s_hdc);
	SetBkMode(s_hdc, TRANSPARENT);
	s_font = CreateFont(s_nSize, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, s_szFont);
	s_ofont = (HFONT)SelectObject(s_hdc, s_font);
}

void gUtil::Text(int x, int y, char *str, bool newline, int lineterm)
{
	if(s_hdc == NULL) return;

	int		len = strlen(str);
	char	szBuf[256];
	int		k = 0;
	int		line = 0;

	if(newline)
	{
		for(int i = 0; i < len; i++)
		{
			if(str[i] != '_')
			{
				szBuf[k++] = str[i];
			}
			else
			{
				szBuf[k] = NULL;
				TextOut(s_hdc, x, y + lineterm * line, szBuf, strlen(szBuf));
				k = 0;
				line++;
			}
		}
		szBuf[k] = '\0';
		TextOut(s_hdc, x, y + lineterm * line, szBuf, strlen(szBuf));
	}
	else
		TextOut(s_hdc, x, y, str, strlen(str));
}

void gUtil::EndText()
{
	SelectObject(s_hdc, s_ofont);
	DeleteObject(s_font);
	gMainWin::GetIF()->m_lpDDBack->ReleaseDC(s_hdc);
}

void gUtil::SetFont(char *font)
{
	strcpy(s_szFont, font);
}

void gUtil::SetSize(int size)
{
	s_nSize = size;
}

void gUtil::SetColor()
{

}

bool gUtil::PointInRect(int x, int y, RECT rc)
{
	if(rc.left <= x && x <= rc.right
		&& rc.top <= y && y <= rc.bottom)
		return true;
	else
		return false;
}

void gUtil::DebugMsg(char *msg)
{
	OutputDebugString(msg);
}

void gUtil::SetDefaultFont()
{
	s_nSize = UTIL_DEFAULT_FONTSIZE;
	strcpy(s_szFont, UTIL_DEFAULT_FONT);
}

int gUtil::TextLength(char *sz)
{
	int		length;
	int		i;

	if(sz != NULL)
	{
		length = strlen(sz);
		length *= (s_nSize/2);		// 글자 하나당 6pixel이라고 가정(한글기준)

		for(i = 0; i < (int)strlen(sz); i++)
			if(sz[i] == ' ' || sz[i] == ',' || sz[i] == '.')
				length -= 2;

		return length;
	}
	return -1;
}