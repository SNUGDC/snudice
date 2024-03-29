#pragma once
#include <windows.h> 

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
	int flag1;
	int flag2;
	int flag3;
	char college[128];
	char building[128];
	char subject[128];
public:
	void init(int xo, int yo);
};