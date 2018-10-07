#include <iostream>
#include <cstdlib>
#include <ctime>

// --------Windows ��V���--------
#define Upkey 2490368
#define Downkey 2621440
#define Leftkey 2424832
#define Rightkey 2555904
#define Enterkey 13
#define Spacekey 32
#define Esckey 27

// ------------��V�w�q------------
#define Direction_Up 0
#define Direction_Left 1
#define Direction_Down 2
#define Direction_Right 3

// ------------�y�D�w�q------------
#define Line_1 1
#define Line_2 2
#define Line_3 3

// ------------�ݩʩw�q------------
#define NORMALFOX 0
#define CUTEFOX 1

class Target_obj{
public:
	int LocateLine; // ���y�D
	int ClickStandard; // �o���ǫh
	int Property; // ��ت��W

	// function �p��U�@�Ӳ��ʦ�m

	// function �غc�禡
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