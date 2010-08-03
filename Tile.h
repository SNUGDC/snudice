#pragma once
#include <windows.h> // �ߺ� include (first case) ����?
/*
enum ETILETYPE {
	TY_NONE=0,
		TY_CLASS,
		TY_ITEM,
		TY_STAMINA,
		TY_BUS,
		TY_MAINGATE
};
*/
enum ETILETYPE {
	TY_STAMINA,
		TY_ITEM,
		//TY_GRASS,
		TY_DRINK,
		TY_BUS,
		TY_MAINGATE,
		TY_MOUNTAIN,
		TY_CLASS,
		TY_NONE
};

class TILE {
public:
	ETILETYPE tileType;
	POINT prevTile;
	POINT ptPos;
	POINT nextTile;
	//HBITMAP* img;
	//int img;
	int flag1;
	int flag2;
	int flag3;
	char college[128];
	char building[128];
	char subject[128];
public:
	//TILE();
	void init(int xo, int yo);
};