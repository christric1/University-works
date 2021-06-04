#include <vector>
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */
#include <random>
#include <math.h>

using namespace std;

enum Attribute {Origin, End, Null};   // 起點、終點
enum Color {Red, Blue, Yellow, Green, All, None};  // 顏色
enum Direction {Right, Left, Up, Dowm}; // 移動方位

random_device rd;
default_random_engine gen = std::default_random_engine(rd());
uniform_int_distribution<int> dis(1, );

static const char* enum_Direction[] =
{ "Right", "Left", "Up", "Dowm" };

static const char* enum_Color[] =
{ "Red", "Blue", "Yellow", "Green", "All", "None" };

Color DetermineColor(int c) {  // 決定顏色
	if (c == 2 || c == 7 || c == 10 || c == 16 || c == 20)
		return Red;
	else if (c == 3 || c == 13 || c == 15 || c == 18 || c == 21)
		return Yellow;
	else if (c == 4 || c == 9 || c == 11 || c == 14 || c == 19)
		return Green;
	else if (c == 5 || c == 6 || c == 8 || c == 12 || c == 17)
		return Blue;
	else if (c == 1)  // 彩色終點
		return All;

}

void Change(char* buffer, int* trob) {
	if (buffer[1] == ' ') {
		trob[0] = buffer[0] - 48;
		for (int i = 1; i < 5; i++) {
			trob[i] = buffer[i + 1] - 48;
		}
	}

	else {
		int temp = (buffer[0] - 48) * 10 + buffer[1] - 48;
		trob[0] = temp;
		for (int i = 1; i < 5; i++) {
			trob[i] = buffer[i + 2] - 48;
		}
	}
}

class Grid 
{
public:

	Attribute at;
	Color cr;
	int Row = 0;
	int Column = 0;
	bool barrier[4] = { false };  // 上、右、下、左

	Grid(Attribute  at = Null, Color co = None) : at(at), cr(co){}  // 建構子

	void SetAttribute(Attribute a) {
		at = a;
	}

	void SetColor(Color c) {
		cr = c;
	}
};

class Board
{
public:
	vector <Grid>  Bo[256];  // 棋盤

	vector <Grid> OriginPointer;  // 指向起點指標 -- 紅 綠 藍 黃
	vector <Grid> PresentPosition;  // 指向現在位置的指標
	vector <Grid> RedEndPointer;  // 指向紅色終點指標
	vector <Grid> GreenEndPointer;  // 指向綠色終點指標
	vector <Grid> BlueEndPointer;  //指向藍色終點指標
	vector <Grid> YellowEndPointer;  //指向黃色終點指標
	Grid* MulticolorEndPointer;  // 彩色終點指標

	void AddBoard(Grid &gr, int i) {  // 放入棋盤
		Bo[i].push_back(gr);
	}

	void SetOriginPointer( Grid &gr) {  
		OriginPointer.push_back(gr);
	}
	
	vector<Grid> GetEndPointer(Color co) {  // 回傳顏色終點vector
		if (co == Red)
			return RedEndPointer;
		else if (co == Green)
			return GreenEndPointer;
		else if (co == Blue)
			return BlueEndPointer;
		else if (co == Yellow)
			return YellowEndPointer;
	}

	void SetEndPointer(Color co, Grid &gr) {
		if (co == Red)
			RedEndPointer.push_back(gr);
		else if (co == Green)
			GreenEndPointer.push_back(gr);
		else if (co == Blue)
			BlueEndPointer.push_back(gr);
		else if (co == Yellow)
			YellowEndPointer.push_back(gr);
		else 
			MulticolorEndPointer = &gr;   // 彩色終點
	}

	Direction GetDirection(Grid PresentLocation) {    // 得到方向

		// cout << PresentLocation.Row << " " << PresentLocation.Column << endl;

		Direction DirectionQueue[4] = { Right, Left, Up, Dowm };  // 優先次序

		int a = dis(gen) % 4;
		while(CheckHaveBlock(PresentLocation, DirectionQueue[a]))      // 看方向是否立刻有障礙物   
			a = dis(gen) % 4;
		return DirectionQueue[a];
	}

	bool FourCheck(Grid ori) {

		Grid temp;
		for (int i = 0; i < PresentPosition.size(); i++) {
			temp = PresentPosition.at(i);
			if (ori.Row == temp.Row && ori.Column == temp.Column)
				return true;
		}
		return false;
	}

	bool CheckHitOtherPiece(Grid gr, Direction di) {  // 確認是否撞到其他棋子

		switch (di){

		case Right:
			if (FourCheck(Bo[gr.Row].at(gr.Column+1)))
				return true;
			break;

		case Left:
			if (FourCheck(Bo[gr.Row].at(gr.Column - 1)))
				return true;
			break;

		case Up:
			if (FourCheck(Bo[gr.Row-1].at(gr.Column)))
				return true;
			break;

		case Dowm:
			if (FourCheck(Bo[gr.Row+1].at(gr.Column)))
				return true;
			break;
		}

		return false;
	}

	bool CheckHaveBlock(Grid gr, Direction di) {  // 確認移動的方向是否有牆壁

		if (di == Right) 
			if (gr.barrier[1] || CheckHitOtherPiece(gr, di)) 
				return true;
		
		if (di == Dowm)
			if (gr.barrier[2] || CheckHitOtherPiece(gr, di))
				return true;

		if (di == Left)
			if (gr.barrier[3] || CheckHitOtherPiece(gr, di))
				return true;

		if (di == Up)
			if (gr.barrier[0] || CheckHitOtherPiece(gr, di))
				return true;
		
		return false;
	}

	Grid KeepGoingBro(Direction di, Grid PresentLocation, Color co) {  // 持續前進
		while (!ArriveEndCheck(PresentLocation, co) && !CheckHaveBlock(PresentLocation, di)) {  // 到終點 or 撞到牆壁 or 撞到其他棋子
			PresentLocation = MoveOneStep(di, PresentLocation);  // 移動一步
			// cout << PresentLocation.Row << " " << PresentLocation.Column << endl;
		}
		return PresentLocation;
	}

	Grid MoveOneStep(Direction di, Grid PresentLocation) {  // 移動一步
		switch (di)
		{
		case Right:
			PresentLocation = Bo[PresentLocation.Row].at(PresentLocation.Column + 1);
			return PresentLocation;
		case Left:
			PresentLocation = Bo[PresentLocation.Row].at(PresentLocation.Column - 1);
			return PresentLocation;
		case Up:
			PresentLocation = Bo[PresentLocation.Row - 1].at(PresentLocation.Column);
			return PresentLocation;
		case Dowm:
			PresentLocation = Bo[PresentLocation.Row + 1].at(PresentLocation.Column);
			return PresentLocation;
		default:
			return PresentLocation;
		}
	}

	bool ArriveEndCheck(Grid Origin, Color co) {  // 確認是否到達終點

		vector<Grid> temp = GetEndPointer(co);

		if (Origin.Row == MulticolorEndPointer->Row && Origin.Column == MulticolorEndPointer->Column)
			return true;

		for (int i = 0; i < temp.size(); i++) {
			if (Origin.Row == temp.at(i).Row && Origin.Column == temp.at(i).Column)
				return true;
		}
		return false;
	}

	void ClearAll() {   // 初始化
		Color co = Bo[7].at(7).cr;  // 中間終點顏色
		for (int i = 0; i < 4; i++)    // 把中間終點拔除
			GetEndPointer(co).pop_back();  
		PresentPosition.clear();  // 目前棋子位置全部清除
	}
};
