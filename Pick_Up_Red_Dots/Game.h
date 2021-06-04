#include <iostream>
#include <string>
#include <vector>
#include "Deck.h"
#include "Player.h"

using namespace std;

class Game
{
private:
	Deck dck;   // 牌堆
	Player TA;  // 助教
	vector <Player> CP;  // 電腦玩家
	vector <Card> TableCard;  // 桌上的海底牌
	int PlayerCnt;   // 玩家人數

public:

	void GameStart() {   // 遊戲開始

		cout << "歡迎來到佛來也 -- 撿紅點 !! " << endl;
		cout << "請問玩家姓名 : ";
		cin >> TA.name;
		cout << "請問想要幾個人玩 ?  2~4人 ";
		cin >> PlayerCnt;

		cout << endl << "-----------------------------------------------" << endl;

		CreateComputerPlayer();  // 創建電腦玩家
		dck.Shuffle();   // 洗牌

		for (int i = 0; i < 24 / PlayerCnt; i++)   // 助教抽牌
			TA.HandCard.push_back(dck.PopCard());

		for (int i = 0; i < CP.size(); i++)    // 電腦玩家抽牌
			for (int j = 0; j < 24 / PlayerCnt; j++)
				CP.at(i).HandCard.push_back(dck.PopCard());
			
		for (int i = 0; i < 4; i++)    // 桌上發4張牌
			TableCard.push_back(dck.PopCard());


		Player** PriorityQueue = new Player*[PlayerCnt];   // 建立輪流順序
		PriorityQueue[0] = &TA;
		for (int i = 1; i < PlayerCnt; i++)
			PriorityQueue[i] = &CP.at(i - 1);    // TA -> John -> Mike -> Mao

		int serial = 0;  

		while (dck.CheckDeckHaveCard()) {

			cout << "第" << (serial / PlayerCnt) + 1 << "輪 -- " << PriorityQueue[serial % PlayerCnt]->name << "的回合" << endl;
			PrintDes();   // 印出桌上牌面
			cout << endl << "-----------------------------------------------" << endl;

			if ((serial % PlayerCnt) != 0)  // 是否輪到電腦
				AutoPlay(*PriorityQueue[serial % PlayerCnt]);

			else if (CheckCanEat(TA)) {   // 牌面有得吃

				int opt;
				cout << "1. 選擇吃  2. 選擇丟牌 ?  1 or 2 " ;
				cin >> opt;

				if (opt == 1) {   // 選擇吃
					int cho;
					vector <Card> Ca = CanEatCard(TA);
					PrintCard(Ca);
					cout << "請問想吃手上哪一張 ? ";
					cout << " 1 ~ " << Ca.size() << "  ";
					cin >> cho;
					CardEat(Ca.at(cho - 1), TA);   // 吃牌
				}

				else {  // 選擇丟
					int cho;
					PrintCard(TA.HandCard);
					cout << "請問想丟哪一張 ? " << " 1 ~ " << TA.HandCard.size() << " ";
					cin >> cho;
					ThrowCard(TA.HandCard.at(cho - 1), TA);  // 丟牌
				}

				DrawCard(TA);  // 翻牌
			}

			else {   // 牌面沒得吃

				int cho;
				PrintCard(TA.HandCard);
				cout << "沒得吃，請問想丟哪一張 ? " << " 1 ~ " << TA.HandCard.size() <<  " " ;
				cin >> cho;
				ThrowCard(TA.HandCard.at(cho - 1), TA);  // 丟牌

				DrawCard(TA);  // 翻牌
			}

			if ((serial % PlayerCnt) == 0) {
				cout << "TA 的手牌" << endl;
				PrintCard(TA.HandCard);
				cout << "TA 吃掉的手牌" << endl;
				PrintCard(TA.EatCard);
			}

			cout << endl << "-----------------------------------------------" << endl;
			serial++;
		}

		ResultCompute();   // 分數結算
		PrintPoint();   // 印出分數

	}

	void PrintPoint() {   // 印出分數

		cout << endl << "最終分數 " << endl << endl;
		cout << TA.name << " : " << TA.ResultPoint << endl;
		for (int i = 0; i < PlayerCnt - 1; i++)
			cout << CP.at(i).name << " : " << CP.at(i).ResultPoint << endl;
		cout << endl;
	}

	int EatCardPoint(Player pl) {   // 吃牌分數結算
		int sum = 0;
		for (int i = 0; i < pl.EatCard.size(); i++) {

			int temp = pl.EatCard.at(i).GetRealNumber();
			if (pl.EatCard.at(i).GetSuit() == 'H' || pl.EatCard.at(i).GetSuit() == 'D') {
				if (temp == 1)
					sum = sum + 20;
				else if (temp >= 9)
					sum = sum + 10;
				else
					sum = sum + temp;
			}
			if (pl.EatCard.at(i).GetSuit() == 'S' && temp == 1)  // 黑桃A
				sum = sum + 10;
		}
		return sum;
	}

	void ResultCompute() {  // 分數結算

		TA.ResultPoint = EatCardPoint(TA);
		for (int i = 0; i < PlayerCnt - 1; i++)
			CP.at(i).ResultPoint = EatCardPoint(CP.at(i));
	}

	void PrintCard(Card Ca) {

		cout << " _____  " ;

		cout << endl;

		char y = Ca.GetNumber();

		if (Ca.GetCover())
			cout << "|     | ";
		else if (y == 'T')   // 確認是否為 10
			cout << "| 1 0 | ";
		else
			cout << "|  " << y << "  | ";

		cout << endl;

		if (Ca.GetCover())
			cout << "|  " << "?" << "  | ";
		else
			cout << "|  " << Ca.GetSuit() << "  | ";

		cout << endl;

		cout << "|     | ";
		cout << endl;

		cout << " ¯¯¯¯¯  ";
		cout << endl;
	}

	void PrintCard(vector<Card> HC, bool cover=false) {  	// 印出牌組

		for (int i = 0; i < HC.size(); i++) {
			cout << " _____  ";
		}
		cout << endl;

		for (int i = 0; i < HC.size(); i++) {
			char y = HC.at(i).GetNumber();

			if (cover)
				cout << "|     | ";

			else if (y == 'T')   // 確認是否為 10
				cout << "| 1 0 | ";
			else
				cout << "|  " << y << "  | ";

		}
		cout << endl;

		for (int i = 0; i < HC.size(); i++) {
			if (cover)
				cout << "|  " << "?" << "  | ";
			else
				cout << "|  " << HC.at(i).GetSuit() << "  | ";
		}
		cout << endl;

		for (int i = 0; i < HC.size(); i++) {
			cout << "|     | ";
		}
		cout << endl;

		for (int i = 0; i < HC.size(); i++) {
			cout << " ¯¯¯¯¯  ";
		}
		cout << endl;
	}

	void PrintDes() {   // 顯示目前牌桌

		cout << "-----------------------------------------------" << endl;

		// 桌上的牌
		cout << "桌上的牌 : " << endl;
		PrintCard(TableCard);

		cout << endl << "-----------------------------------------------" << endl;

		// TA的牌
		cout << TA.name << " : " << endl;
		PrintCard(TA.HandCard);

		// 電腦的牌
		for (int i = 0; i < CP.size(); i++) {
			cout << CP.at(i).name << " : " << endl;
			PrintCard(CP.at(i).HandCard, true);
		}

	}

	bool CheckCanEat(Player pl) {   // 確認是否能吃
		for (int i = 0; i < pl.HandCard.size(); i++) {

			int temp = pl.HandCard.at(i).GetRealNumber();

			if (temp >= 10) {
				for (int j = 0; j < TableCard.size(); j++) 
					if(temp == TableCard.at(j).GetRealNumber())
						return true;
			}

			else {
				for (int j = 0; j < TableCard.size(); j++) 
					if ((temp + TableCard.at(j).GetRealNumber()) == 10) 
						return true;
			}
		}
		return false;
	}

	vector<Card> CanEatCard(Player pl) {  // 能吃的牌

		vector <Card> RD;

		for (int i = 0; i < pl.HandCard.size(); i++) {

			int temp = pl.HandCard.at(i).GetRealNumber();

			if (temp >= 10) {
				for (int j = 0; j < TableCard.size(); j++)
					if (temp == TableCard.at(j).GetRealNumber()) {
						RD.push_back(pl.HandCard.at(i));
						break;
					}
			}

			else {
				for (int j = 0; j < TableCard.size(); j++)
					if ((temp + TableCard.at(j).GetRealNumber()) == 10) {
						RD.push_back(pl.HandCard.at(i));
						break;
					}
			}
		}

		return RD;
	}

	void DrawCard(Player &pl) {  // 翻牌

		Card Ca = dck.PopCard();
		int temp = Ca.GetRealNumber();

		cout << pl.name << " 翻牌~" << endl;
		PrintCard(Ca);

		vector <Card> RD;

		if (temp >= 10) {
			for (int j = 0; j < TableCard.size(); j++)
				if (temp == TableCard.at(j).GetRealNumber())
					RD.push_back(TableCard.at(j));
		}

		else {
			for (int j = 0; j < TableCard.size(); j++)
				if ((temp + TableCard.at(j).GetRealNumber()) == 10)
					RD.push_back(TableCard.at(j));
		}

		if ((RD.size() == 1 || pl != TA) && RD.size() != 0) {
			pl.EatCard.push_back(RD.at(0));   // 吃掉桌上的牌
			pl.EatCard.push_back(Ca);   // 吃掉翻開的牌
			TakeCardOff(RD.at(0), TableCard);   // 移除桌上的牌
		}

		else if (RD.size() > 1 && !(pl != TA)){    // 桌上有兩張以上一樣數字的牌
			int cho;
			PrintCard(RD);
			cout << "你要選擇吃桌上哪張 ? " << "1 ~ " << RD.size();
			cin >> cho;

			pl.EatCard.push_back(RD.at(cho - 1));   // 吃掉桌上的牌
			pl.EatCard.push_back(Ca);   // 吃掉翻開的牌
			TakeCardOff(RD.at(cho - 1), TableCard);   // 移除桌上的牌

		}

		else {   // 桌上未找到可吃掉的牌
			TableCard.push_back(Ca);
		}

	}

	void CardEat(Card ca, Player &pl) {  // 吃牌

		vector <Card> RD;

		int temp = ca.GetRealNumber();

		if (temp >= 10) {
			for (int j = 0; j < TableCard.size(); j++)
				if (temp == TableCard.at(j).GetRealNumber())
					RD.push_back(TableCard.at(j));
		}

		else {
			for (int j = 0; j < TableCard.size(); j++)
				if ((temp + TableCard.at(j).GetRealNumber()) == 10)
					RD.push_back(TableCard.at(j));
		}

		pl.EatCard.push_back(ca);  // 吃掉手中的牌
		TakeCardOff(ca, pl.HandCard);  // 移除手上的牌

		if (RD.size() == 1 || pl != TA){
			pl.EatCard.push_back(RD.at(0));   // 吃掉桌上的牌
			TakeCardOff(RD.at(0), TableCard);   // 移除桌上的牌
		}

		else {    // 桌上有兩張以上一樣數字的牌
			int cho;
			PrintCard(RD);
			cout << "你要選擇吃桌上哪張 ? " << "1 ~ " << RD.size() << " ";
			cin >> cho;

			pl.EatCard.push_back(RD.at(cho - 1));   // 吃掉桌上的牌
			TakeCardOff(RD.at(cho-1), TableCard);   // 移除桌上的牌
			
		}
		cout << endl;
	}

	void ThrowCard(Card ca, Player &pl) {   // 丟牌到桌上

		TakeCardOff(ca, pl.HandCard);  // 移除手上的牌
		TableCard.push_back(ca);  // 增加桌上的牌
		cout << endl;
	}

	void TakeCardOff(Card ca, vector<Card> &cd) {   // 拿掉牌
		
		for (int i = 0; i < cd.size(); i++) {
			if (ca == cd.at(i))
				cd.erase(cd.begin() + i);
		}

	}

	void CreateComputerPlayer() {   // 建立電腦玩家人數
		string name[] = { "John", "Mike", "Mao" };
		for (int i = 0; i < PlayerCnt-1; i++) {
			Player temp;
			temp.name = name[i];
			CP.push_back(temp);
		}
	}

	void AutoPlay(Player &pl) {  // 電腦自動操作

		if (CheckCanEat(pl)) {
			vector <Card> Ca = CanEatCard(pl);
			CardEat(Ca.at(0), pl);   // 吃牌
			cout << pl.name << " 吃牌~" << endl;
			PrintCard(Ca.at(0));
		}

		else {
			Card temp = pl.HandCard.at(0);
			ThrowCard(temp, pl);  // 丟牌
			cout << pl.name << " 丟牌~" << endl;
			PrintCard(temp);
		}

		DrawCard(pl);  // 翻牌
	}
};