#include "game_setup.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Windows.h>
using namespace std;
using namespace cv;

string game_name = "次ゴ块墓";
#define Screen_Cols 820
#define Screen_Rows 1433
#define dpi_pxl 118
#define Height_Trival 70
#define First_Point_h 30
#define Line_1_w 40
#define Line_2_w 158
#define Line_3_w 275
#pragma warning( disable : 4996 )
Mat img_NormalFox;
Mat img_CuteFox;
Mat img_Baseball;
Mat img_PlayingBG;
Mat img_BG;
Mat img_Result;
Mat img_Selecting;
Mat number[10];
void Init();
void Enterance_mode();
void Playing_mode();
void Result_Screen(int,int);
int Nor_ScoreTable[] = {1000,1103,1207,1296,1316,1348,1369,1384,1400,1437,1494,
						1538,1585,1610,1639,1689,1726,1769,1802,1843,1856,1889,
						1945,1978,2007,2108,2543,2756,2850};
int Cut_ScoreTable[] = {1008,1146,1252,1300,1325,1352,1375,1392,1426,1449,1512,
						1549,1596,1648,1652,1704,1754,1790,1815,1822,1842,1879,
						1902,2100,2200,2400,2592,2800,2850};

inline void overlayImage(Mat* src, Mat* overlay, const Point& location)
{
	for (int y = max(location.y, 0); y < src->rows; ++y)
	{
		int fY = y - location.y;

		if (fY >= overlay->rows)
			break;

		for (int x = max(location.x, 0); x < src->cols; ++x)
		{
			int fX = x - location.x;

			if (fX >= overlay->cols)
				break;

			double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;

			for (int c = 0; opacity > 0 && c < src->channels(); ++c)
			{
				unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
				unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
				src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
			}
		}
	}
}

int ReturnLocate(int Line){
	switch (Line)
	{
	case Line_1:
		return Line_1_w;
	case Line_2:
		return Line_2_w;
	case Line_3:
		return Line_3_w;
	default:
		return Line_1_w;
		break;
	}
}

inline void PresentPrior(Mat &temp_image, vector<Target_obj> &Obj_Array, int index){
	// ㄧΑ程篊 0.103
	temp_image = img_PlayingBG.clone();
	int count = 0;
	for (int Iter_Array = index + 6; Iter_Array >= index; --Iter_Array){
		int temp_Line = ReturnLocate(Obj_Array[Iter_Array].LocateLine);
		int Position = 30 + (count * 70);
		if (Obj_Array[Iter_Array].Property == NORMALFOX){
			overlayImage(&temp_image, &img_NormalFox, Point(temp_Line, Position));
		}
		else{
			overlayImage(&temp_image, &img_CuteFox, Point(temp_Line, Position));
		}
		count++;
	}
	imshow(game_name, temp_image);
	/*overlayImage(&temp_image, &img_NormalFox, Point(Line_1, 30));
	overlayImage(&temp_image, &img_NormalFox, Point(Line_2, 100));
	overlayImage(&temp_image, &img_CuteFox, Point(Line_1, 170));
	overlayImage(&temp_image, &img_NormalFox, Point(Line_1, 240));
	overlayImage(&temp_image, &img_NormalFox, Point(Line_3, 310));
	overlayImage(&temp_image, &img_CuteFox, Point(Line_3, 380));
	overlayImage(&temp_image, &img_NormalFox, Point(Line_2, 450));*/
	// ㊣ㄧΑ 7皌竚Ч(for癹伴代刚) キА程е0.031 キА程篊0.103 (0.040瞷Ω计蔼笷15Ω) 伐Ω计:588
	// 獶ㄧΑ 7皌竚Ч(for癹伴代刚) キА程е0.031 キА程篊0.047(0.040瞷Ω计ぶ5Ω) 伐Ω计:624
	// ︽ずㄧΑ 7皌竚Ч(for癹伴代刚) キА程е0.031 キА程篊0.047(0.040瞷Ω计ぶ5Ω) 伐Ω计:621
	// 度overlayImage︽ずㄧΑ 7皌竚Ч(for癹伴代刚) キА程е0.031 キА程篊0.047(0.040瞷Ω计ぶ5Ω) 伐Ω计:623
	// ㄢ︽ずㄧΑ 7皌竚Ч(for癹伴代刚) キА程е0.031 キА程篊0.047(0.040瞷Ω计ぶ5Ω) 伐Ω计:624
}

void BeforeStart(Mat &temp_image, vector<Target_obj> &Obj_Array, int index){
	clock_t Wait_Begin = clock();
	clock_t Wait_End = clock();
	double timecount = double(Wait_End - Wait_Begin) / CLOCKS_PER_SEC;
	int nowcount = 0;
	while (timecount < 4){
		if ((int)timecount > nowcount){
			temp_image = img_PlayingBG.clone();
			for (int i = 0; i < nowcount+1; ++i){
				int temp_Line = ReturnLocate(Obj_Array[i].LocateLine);
				int Position = 30 + ((6-i) * 70);
				if (Obj_Array[i].Property == NORMALFOX){
					overlayImage(&temp_image, &img_NormalFox, Point(temp_Line, Position));
				}
				else{
					overlayImage(&temp_image, &img_CuteFox, Point(temp_Line, Position));
				}
			}
			overlayImage(&temp_image, &number[3 - nowcount], Point(temp_image.cols / 2 - 20, temp_image.rows / 2 - 50));
			imshow(game_name, temp_image);
			waitKey(1);
			++nowcount;
		}
		Wait_End = clock();
		timecount = double(Wait_End - Wait_Begin) / CLOCKS_PER_SEC;
	}
	/*for (int timecount = 3; timecount >= 1; --timecount){
		temp_image = img_PlayingBG.clone();
		overlayImage(&temp_image, &number[timecount], Point(0, 0));
		imshow(game_name, temp_image);
		waitKey(1000);
	}*/
	return;
}

void RedFade(Mat &temp_image){
	Mat Red(Screen_Rows / 2, Screen_Cols / 2, CV_8UC4, Scalar(0, 0, 255));
	for (int FadeIndex = 0; FadeIndex <= 10; ++FadeIndex){
		double Fade = static_cast<double>(FadeIndex) / 10.0;
		Mat dst;
		addWeighted(temp_image,  Fade, Red, 1.0 - Fade, 0, dst);
		imshow(game_name, dst);
		if (FadeIndex == 10)
			return;
		else
			waitKey(50);
	}
	return;
}

void smootheffect(){
	int mask[3][3] = { { 1, 2, 1 }, { 2, 5, 2 }, { 1, 2, 1 } };
	double row1, row2, row3;
	Mat temp = img_BG.clone();
	Mat image(temp.rows, temp.cols, CV_8UC3);
	for (int row = 1; row < temp.rows-1; ++row){
		for (int col = 1; col < temp.cols-1; ++col){
			for (int channel = 0; channel < 3; ++channel){
				row1 = mask[0][0] * temp.at<Vec4b>(row - 1, col - 1)[channel] + mask[0][1] * temp.at<Vec4b>(row - 1, col)[channel] + mask[0][2] * temp.at<Vec4b>(row - 1, col + 1)[channel];
				row2 = mask[1][0] * temp.at<Vec4b>(row, col - 1)[channel] + mask[1][1] * temp.at<Vec4b>(row, col)[channel] + mask[1][2] * temp.at<Vec4b>(row, col + 1)[channel];
				row3 = mask[2][0] * temp.at<Vec4b>(row + 1, col - 1)[channel] + mask[2][1] * temp.at<Vec4b>(row + 1, col)[channel] + mask[2][2] * temp.at<Vec4b>(row + 1, col + 1)[channel];
				image.at<Vec3b>(row, col)[channel] = (row1 + row2 + row3) / 17.0;
			}
		}
	}
	img_Result = image.clone();
}

int main(){
	// ------------Init------------ Start
	Init();
	srand(unsigned int(time(NULL)));
	// ------------Init------------ End

	Enterance_mode();


	/* 丁璸衡竟
	clock_t Wait_Begin = clock();
	clock_t Wait_End = clock();
	double Update_secs = 0.0;
	Update_secs = double(Wait_End - Wait_Begin) / CLOCKS_PER_SEC;
	*/
	destroyAllWindows();
	system("pause");
	return 0;
}

void Init(){
	img_NormalFox = imread("images/NormalFox.png", IMREAD_UNCHANGED);
	img_CuteFox = imread("images/CuteFox.png", IMREAD_UNCHANGED);
	img_Baseball = imread("images/baseball-icon.png", IMREAD_UNCHANGED);
	img_PlayingBG = imread("images/demo_bg.png", IMREAD_UNCHANGED);
	img_BG = imread("images/background.png", IMREAD_UNCHANGED);
	img_Selecting = imread("images/Selecting.png", IMREAD_UNCHANGED);
	resize(img_BG, img_BG, Size(Screen_Cols / 2, Screen_Rows / 2));
	resize(img_PlayingBG, img_PlayingBG, Size(Screen_Cols / 2, Screen_Rows / 2));
	resize(img_NormalFox, img_NormalFox, Size(int(img_NormalFox.cols / 1.8), int(img_NormalFox.rows / 1.8)));
	resize(img_CuteFox, img_CuteFox, Size(int(img_CuteFox.cols / 1.8), int(img_CuteFox.rows / 1.8)));
	resize(img_Selecting, img_Selecting, Size(int(img_Selecting.cols / 4.5), int(img_Selecting.rows / 4.5)));
	for (int i = 0; i < 10; ++i){
		string s = to_string(i);
		number[i] = imread("fonts/" + s + ".png", IMREAD_UNCHANGED);
	}
	smootheffect();
	return;
}

void Enterance_mode(){
	int Enterance_Status = 0;
	Mat enterance_game = imread("fonts/enterance_game.png", IMREAD_UNCHANGED);
	Mat enterance_exit = imread("fonts/enterance_exit.png", IMREAD_UNCHANGED);
	resize(enterance_game, enterance_game, Size(int(enterance_game.cols / 1.8), int(enterance_game.rows / 1.8)));
	resize(enterance_exit, enterance_exit, Size(int(enterance_exit.cols / 1.8), int(enterance_exit.rows / 1.8)));
	while (true)
	{
		int key = waitKey(0);
		switch (key)
		{
		case Upkey:
			if (Enterance_Status == 0)
				Enterance_Status = 1;
			else
				--Enterance_Status;
			break;
		case Downkey:
			if (Enterance_Status == 1)
				Enterance_Status = 0;
			else
				++Enterance_Status;
			break;
		case Enterkey:
			if (Enterance_Status == 0)
				Playing_mode();
			else
				return;
			break;
		default:
			break;
		}

		Mat Enterance = img_BG.clone();
		rectangle(Enterance, Point(95, 400), Point(325, 460), Scalar(255, 255, 255), CV_FILLED); // Scalar(237, 241, 242)
		overlayImage(&Enterance, &enterance_game, Point(120, 410));
		rectangle(Enterance, Point(95, 500), Point(325, 560), Scalar(255, 255, 255), CV_FILLED);
		overlayImage(&Enterance, &enterance_exit, Point(120, 510));
		switch (Enterance_Status)
		{
		case 0:
			overlayImage(&Enterance, &img_Selecting, Point(100, 430 - (img_Selecting.rows / 2)));
			break;
		case 1:
			overlayImage(&Enterance, &img_Selecting, Point(100, 530 - (img_Selecting.rows / 2)));
			break;
		default:
			break;
		}
		imshow(game_name, Enterance);
	}
	return;
}

void Playing_mode(){
	int combo = 0, score = 0, maxcombo = 0;
	Mat temp_image = img_PlayingBG.clone();
	vector<Target_obj> Obj_Array;
	for (int Insert_Array = 0; Insert_Array < 7; ++Insert_Array){
		Target_obj temp_obj;
		Obj_Array.push_back(temp_obj);
	}
	int count = 0;
	BeforeStart(temp_image, Obj_Array, count);
	PresentPrior(temp_image, Obj_Array, count);

	clock_t Wait_Begin = clock();
	double Time_secs = 0.0; int timecount = 0;
	while (timecount < 30) {
		int key = waitKey(10);
		if (key == Obj_Array[count].ClickStandard){
			Target_obj temp_obj;
			Obj_Array.push_back(temp_obj);


			++combo;
			if (combo > maxcombo)
				maxcombo = combo;

			if ((combo / 10)>28)
				score += 2850;
			else
			{
				if (Obj_Array[count].Property == NORMALFOX)
					score += Nor_ScoreTable[combo / 10];
				else
					score += Cut_ScoreTable[combo / 10];
			}

			++count;
			PresentPrior(temp_image, Obj_Array, count);

		}
		else{
			if (key == Upkey || key == Downkey || key == Leftkey || key == Rightkey){
				combo = 0;
				RedFade(temp_image);
			}
			else{
				if (key == Esckey){
					return;
				}
			}
		}
		clock_t Wait_End = clock();
		Time_secs = double(Wait_End - Wait_Begin) / CLOCKS_PER_SEC;
		if ((int)Time_secs > timecount){
			if (Time_secs < 10)
				cout << "筁计: 0" << (int)Time_secs << "ゴ: " << count << " 程蔼硈阑: " << maxcombo << " ヘ玡硈阑: " << combo << " だ计: " << score << endl;
			else
				cout << "筁计: " << (int)Time_secs << "ゴ: " << count << " 程蔼硈阑: " << maxcombo << " ヘ玡硈阑: " << combo << " だ计: " << score << endl;
			++timecount;
		}
	}
	Result_Screen(score, maxcombo);
	return;
}

void Result_Screen(int score,int combo){
	Mat Result = img_Result.clone();
	Mat result_score = imread("fonts/result_score.png", IMREAD_UNCHANGED);
	Mat result_combo = imread("fonts/result_combo.png", IMREAD_UNCHANGED);
	Mat all_return = imread("fonts/all_return.png", IMREAD_UNCHANGED);
	resize(result_score, result_score, Size(int(result_score.cols / 1.8), int(result_score.rows / 1.8)));
	resize(result_combo, result_combo, Size(int(result_combo.cols / 1.8), int(result_combo.rows / 1.8)));
	resize(all_return, all_return, Size(int(all_return.cols / 1.8), int(all_return.rows / 1.8)));
	rectangle(Result, Point(50, 30), Point(190, 80), Scalar(255, 255, 255), CV_FILLED);
	overlayImage(&Result, &result_score, Point(65, 38));
	int score_temp = score, combo_temp = combo;
	for (int i = 5; i >= 0; --i){
		overlayImage(&Result, &number[score_temp% 10], Point(65 + i * 40, 102));
		score_temp /= 10;
	}

	rectangle(Result, Point(50, 230), Point(260, 280), Scalar(255, 255, 255), CV_FILLED);
	overlayImage(&Result, &result_combo, Point(50, 238));
	if (combo_temp >= 100){
		for (int i = 2; i >= 0; --i){
			overlayImage(&Result, &number[combo_temp % 10], Point(65 + i * 40, 302));
			combo_temp /= 10;
		}
	}
	else{
		for (int i = 1; i >= 0; --i){
			overlayImage(&Result, &number[combo_temp % 10], Point(65 + i * 40, 302));
			combo_temp /= 10;
		}
	}

	cout << endl;
	rectangle(Result, Point(50, 530), Point(220, 580), Scalar(255, 255, 255), CV_FILLED);
	overlayImage(&Result, &all_return, Point(95, 538));
	overlayImage(&Result, &img_Selecting, Point(55, 536));


	imshow(game_name, Result);
	fstream file;
	file.open("score.txt", ios::app);
	if (!file)
		cout << "岿粇块ア毖\n";
	else{
		time_t t = time(0);   // get time now
		struct tm * now = localtime(&t);
		stringstream ss;
		ss << (now->tm_year + 1900) << "-" << (now->tm_mon + 1) << "-" << now->tm_mday << " " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << " B" << " 程蔼硈阑:" << combo << " だ计:" << score << "\n";
		string str = ss.str();
		const char *cstr = str.c_str();
		file.write(cstr, strlen(cstr));
	}
	file.close();
	while (true)
	{
		int key = waitKey(0);
		if (key == Enterkey)
			break;
	}
	return;
}