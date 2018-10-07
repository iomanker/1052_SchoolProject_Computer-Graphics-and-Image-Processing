#include <iostream>
#include <cstdlib>
#include <ctime>

// --------Windows 方向鍵值--------
#define Upkey 2490368
#define Downkey 2621440
#define Leftkey 2424832
#define Rightkey 2555904
#define Enterkey 13
#define Spacekey 32
#define Esckey 27

// ------------方向定義------------
#define Direction_Up 0
#define Direction_Left 1
#define Direction_Down 2
#define Direction_Right 3

// ------------軌道定義------------
#define Line_1 1
#define Line_2 2
#define Line_3 3

// ------------屬性定義------------
#define NORMALFOX 0
#define CUTEFOX 1

class Target_obj{
public:
	int LocateLine; // 位於軌道
	int ClickStandard; // 得分準則
	int Property; // 何種狐狸

	// function 計算下一個移動位置

	// function 建構函式
	Target_obj(){
		LocateLine = (rand() % 3) + 1;
		int Property_cal = rand() % 100;
		if (Property_cal < 90)
			Property = NORMALFOX;
		else
			Property = CUTEFOX;
		switch (LocateLine)
		{
		case Line_1:
			switch (Property)
			{
			case CUTEFOX:
				ClickStandard = Upkey;
				break;
			default:
				ClickStandard = Leftkey;
				break;
			}
			break;
		case Line_2:
			switch (Property)
			{
			case CUTEFOX:
				ClickStandard = Upkey;
				break;
			default:
				ClickStandard = Downkey;
				break;
			}
			break;
		case Line_3:
			switch (Property)
			{
			case CUTEFOX:
				ClickStandard = Upkey;
				break;
			default:
				ClickStandard = Rightkey;
				break;
			}
			break;
		default:
			// Should not appear here.
			break;
		}
	}
};