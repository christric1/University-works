#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */
#include "card.h"

using namespace std;

class player 
{
public:
	string name;  // 玩家名
	int BargainingChip = 10000;   // 籌碼
	int Ante = 1000; // 底注
	vector<card> HandCard;  // 手牌
	bool BlackJack = false; // 黑傑克
	bool surrender = false; //投降
	bool Insurance = false;  // 保險
	bool DoubleDown = false;  // 雙倍加注
	bool Over = false; // 是否超過 21點 爆掉
};

class Game
{
private:
	int round; 
	deck dck;   // 牌堆
	player* banker; // 莊家
	player* Spelarens[3]; // 閒家
	player TA;  // 助教
	player p[3]; // 三位電腦玩家

public:
	Game(deck miko) : dck(miko) {   // 建構子
		p[0].name = "Chris";
		p[1].name = "Scott";
		p[2].name = "John";
	}     

	void DecideSpe() {  // 決定閒家
		if (banker == &TA ) {  
			Spelarens[0] = &p[0];
			Spelarens[1] = &p[1];
			Spelarens[2] = &p[2];
		}
		else if (banker == &p[0]) {
			Spelarens[0] = &TA;
			Spelarens[1] = &p[1];
			Spelarens[2] = &p[2];
		}
		else if (banker == &p[1]) {
			Spelarens[0] = &TA;
			Spelarens[1] = &p[0];
			Spelarens[2] = &p[2];
		}
		else if (banker == &p[2]) {
			Spelarens[0] = &TA;
			Spelarens[1] = &p[0];
			Spelarens[2] = &p[1];
		}
	}

	void GameStart() {  // 開始執行

		bool sur;
		char ans;
		cout << "歡迎來到 佛來也-21點!!" << endl << "請問玩家名稱?   ";
		cin >> TA.name;
		cout << TA.name << "  想玩幾局?  ";
		cin >> round;
		cout << endl;

		int x = rand() % 3;   // 決定莊家
		banker = &p[x];
		DecideSpe();

		for (int i = 0; i < round; i++) {   // 執行 round 局數
			sur = false;
			cout << "-------------------------------------------------------------------------------" << endl << endl;
			cout << "第" << i+1 << "局 : " << banker->name <<" 為莊家" << endl << endl;
			cout << "您想下注多少 ? ";
			cin >> TA.Ante;
			cout << endl;

			dck.DeckReset();  // 牌組重置
			dck.Shuffle();  // 洗牌

			for (int i = 0; i < 3; i++) {   // 籌碼扣掉本金
				Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip - Spelarens[i]->Ante;
			}
			
			for (int i = 0; i < 3; i++) {   // 閒家拿兩張牌
				Spelarens[i]->HandCard.push_back(dck.PopCard());
				Spelarens[i]->HandCard.push_back(dck.PopCard());
			}

			banker->HandCard.push_back(dck.PopCard());    // 莊家拿兩張牌
			banker->HandCard.push_back(dck.PopCard());
			banker->HandCard.at(1).SetCover(true);

			PrintDes();   // 顯示牌桌
			cout << "-------------------------------------------------------------------------------" << endl << endl;
			cout << "您的手牌 :　" << endl;
			PrintCard(TA.HandCard);  // 顯示手牌
			
			if (banker->HandCard.at(0).GetNumber() == 'A') {    // 確認莊家第一張明牌是否為 A
				sur = true;
			}
			
			if (Spelarens[0] == &TA) {  // TA是閒家

				if (BlackJack(TA.HandCard)) {  // 黑傑克確認程序
					TA.BlackJack = true;
					cout << "您是黑傑克" << endl << endl;
				}
				
				cout << "是否投降 ? y/n ";  // 投降程序
				cin >> ans;
				if (ans == 'y')  
					TA.surrender = true;

				if (sur) {  // 保險程序
					cout << "是否要購買保險 ? y/n ";
					cin >> ans;
					if (ans == 'y') 
						TA.Insurance = true;
				}

				if ((TA.HandCard.at(0).GetNumber() + TA.HandCard.at(1).GetNumber() == 107) && !(TA.surrender)) {  // 雙倍加注程序
					cout << "是否要雙倍加注 ? y/n ";
					cin >> ans;
					if (ans == 'y')
						TA.DoubleDown = true;
				}
				cout << endl;

				while (!OverCheck(TA.HandCard, 21) && !(TA.BlackJack) && !(TA.surrender)) {   // 加牌程序
					cout << "是否繼續加牌 ? y/n ";
					cin >> ans;
					if (ans == 'n')
						break;
					TA.HandCard.push_back(dck.PopCard());  // 抽牌
					PrintCard(TA.HandCard);  // 顯示手牌
				}

				TA.Over = OverCheck(TA.HandCard, 21);  // 確認是否爆牌

				if (TA.Over)
					cout << "您爆牌了!!" << endl << endl;


				AutoPlay(banker);  // 自動程序
				AutoPlay(Spelarens[1]);
				AutoPlay(Spelarens[2]);

			}

			else {   // TA 是莊家

				while (!OverCheck(TA.HandCard, 21) && !(TA.BlackJack)) {   // 加牌程序
					cout << "是否繼續加牌 ? y/n ";
					cin >> ans;
					if (ans == 'n')
						break;
					TA.HandCard.push_back(dck.PopCard());  // 抽牌
				}

				AutoPlay(Spelarens[0]);
				AutoPlay(Spelarens[1]);
				AutoPlay(Spelarens[2]);

			}

			cout << "-------------------------------------------------------------------------------" << endl << endl;
			cout << "目前牌桌 : " << endl << endl;
			banker->HandCard.at(1).SetCover(false);

			InsuranceHandle(); // 保險結算

			PrintDes();  // 顯示牌桌

			ResultCompute();   // 結算

			PrintBar();  // 分數呈現

			Reset();  // 歸零
			
			player* ptemp;
			ptemp = banker;
			for (int i = 0; i < 3; i++) {
				if (Spelarens[i]->BargainingChip > ptemp->BargainingChip) {
					ptemp = Spelarens[i];
				}
			}
			banker = ptemp;
			DecideSpe();  // 決定閒家
		}

		cout << "贏家為 " << banker->name << endl;
	} 
	
	// 歸零
	void Reset() {

		banker->BlackJack = false;
		banker->DoubleDown = false;
		banker->Insurance = false;
		banker->Over = false;
		banker->surrender = false;
		banker->HandCard.clear();

		for (int i = 0; i < 3; i++) {
			Spelarens[i]->BlackJack = false;
			Spelarens[i]->DoubleDown = false;
			Spelarens[i]->Insurance = false;
			Spelarens[i]->Over = false;
			Spelarens[i]->surrender = false;
			Spelarens[i]->HandCard.clear();
		}

		
	}

	// 保險程序
	void InsuranceHandle() {
		bool temp = false;
		if (BlackJack(banker->HandCard))
			temp = true;

		for (int i = 0; i < 3; i++) {
			if (Spelarens[i]->Insurance && temp) {   // 有保險 且 莊家黑傑克
				Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip + Spelarens[i]->Ante;
				banker->BargainingChip = banker->BargainingChip - Spelarens[i]->Ante;
			}
			else if (Spelarens[i]->Insurance && !temp){  // 有保險 且 莊家不是黑傑克
				Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip - (Spelarens[i]->Ante)/2;
				banker->BargainingChip = banker->BargainingChip + (Spelarens[i]->Ante) / 2;
			}
		}
	}

	// 分數呈現
	void PrintBar() {
		cout << "-------------------------------------------------------------------------------" << endl << endl;
		cout << "分數結算 : " << endl << endl;
		cout << "莊家 " << banker->name << ", 籌碼 : " << banker->BargainingChip << endl;
		for (int i = 0; i < 3; i++) 
			cout << "閒家 " << Spelarens[i]->name << ", 籌碼 : " << Spelarens[i]->BargainingChip << endl;
		cout << endl;
	}

	// 電腦自動
	void AutoPlay(player* p) {

		p->BlackJack = BlackJack(p->HandCard);

		if (p->BlackJack) 
			return;

		while (!OverCheck(p->HandCard, 17)) 
			p->HandCard.push_back(dck.PopCard());
		p->Over = OverCheck(p->HandCard, 21);

	}

	// 結算
	void ResultCompute() {

		bool pat = false;
		bool spe = false;
		if (banker->Over) {  // 莊家爆牌
			for (int i = 0; i < 3; i++) {

				if (Spelarens[i]->DoubleDown) {   // 是否雙倍下注
					Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip - Spelarens[i]->Ante;
					Spelarens[i]->Ante = (Spelarens[i]->Ante) * 2;
				}

				if (Spelarens[i]->surrender) {  // 是否投降
					Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip + (Spelarens[i]->Ante) / 2;
					banker->BargainingChip = banker->BargainingChip + (Spelarens[i]->Ante) / 2;
				}

				else if (FiveDragon(Spelarens[i]->HandCard)) {   // 是否過五關
					Spelarens[i]->BargainingChip = (Spelarens[i]->Ante) * 4 + Spelarens[i]->BargainingChip;
					banker->BargainingChip = banker->BargainingChip - (Spelarens[i]->Ante) * 3;
				}

				else if (Straight(Spelarens[i]->HandCard)) {  // 是否有順子
					Spelarens[i]->BargainingChip = (Spelarens[i]->Ante) * 4 + Spelarens[i]->BargainingChip;
					banker->BargainingChip = banker->BargainingChip - (Spelarens[i]->Ante) * 3;
				}

				else if (Spelarens[i]->Over) {  // 爆掉
					Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip + Spelarens[i]->Ante;
				}

				else  {  // 沒有爆掉
					Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip + (Spelarens[i]->Ante) * 2;
					banker->BargainingChip = banker->BargainingChip - Spelarens[i]->Ante;
				}
			}
		}

		else {  // 莊家沒爆牌
			if (Straight(banker->HandCard) || FiveDragon(banker->HandCard))  // 是否有特殊牌型
				pat = true;

			for (int i = 0; i < 3; i++) {

				if (Straight(Spelarens[i]->HandCard) || FiveDragon(Spelarens[i]->HandCard))
					spe = true;

				if (Spelarens[i]->surrender) {  // 是否投降
					Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip + (Spelarens[i]->Ante) / 2;
					banker->BargainingChip = banker->BargainingChip + (Spelarens[i]->Ante) / 2;
				}

				else if (Spelarens[i]->Over) {   // 是否爆掉
					banker->BargainingChip = banker->BargainingChip + Spelarens[i]->Ante;
				}

				else if (pat) { // 莊家為特殊牌型
					if (spe) {  // 閒家也是特殊牌型
						if (PrintPoint(Spelarens[i]->HandCard) == PrintPoint(banker->HandCard)) {
							banker->BargainingChip = banker->BargainingChip + Spelarens[i]->Ante;
						}
						else if(PrintPoint(Spelarens[i]->HandCard) > PrintPoint(banker->HandCard)){
							Spelarens[i]->BargainingChip = (Spelarens[i]->Ante) * 4 + Spelarens[i]->BargainingChip;
							banker->BargainingChip = banker->BargainingChip - (Spelarens[i]->Ante) * 3;
						}
						else {
							banker->BargainingChip = banker->BargainingChip + Spelarens[i]->Ante;
						}
					}

					else {  // 閒家不是特殊牌型
						banker->BargainingChip = banker->BargainingChip + Spelarens[i]->Ante;
					}
				}

				else {  // 莊家不是特殊牌型
					if (spe) {
						Spelarens[i]->BargainingChip = (Spelarens[i]->Ante) * 4 + Spelarens[i]->BargainingChip;
						banker->BargainingChip = banker->BargainingChip - (Spelarens[i]->Ante) * 3;
					}

					else { 
						if (PrintPoint(Spelarens[i]->HandCard) == PrintPoint(banker->HandCard)) {
							banker->BargainingChip = banker->BargainingChip + Spelarens[i]->Ante;
						}
						else if (PrintPoint(Spelarens[i]->HandCard) > PrintPoint(banker->HandCard)) {
							Spelarens[i]->BargainingChip = (Spelarens[i]->Ante) * 4 + Spelarens[i]->BargainingChip;
							banker->BargainingChip = banker->BargainingChip - (Spelarens[i]->Ante) * 3;
						}
						else {
							banker->BargainingChip = banker->BargainingChip + Spelarens[i]->Ante;
						}
					}
				}
				spe = false;
			}
		}
	}  

	// 是否有順子
	bool Straight(vector<card> HC) {
		if (HC.size() != 3)
			return false;
		for (int i = 6; i < 9; i++) {
			for (int j = 0; j < 3; j++) {
				if (HC.at(j).GetRealNumber() == i)
					break;
				if (j == 2)
					return false;
			}
		}
		return true;
	}

	// 是否過五關 
	bool FiveDragon(vector<card> HC) {
		if (HC.size() >= 5) 
			return true;
		return false;
	}
	
	// 測試是否有 blackjack
	bool BlackJack (vector<card> HC) {

		int sum = HC.at(0).GetNumber() + HC.at(1).GetNumber();
		if (sum == 139 || sum == 146 || sum == 140 || sum == 149)  // ASCII
			return true;
		return false;
	}

	// 測試是否有超過 21點
	bool OverCheck(vector<card> HC, int limit) {

		int sum1 = 0, sum2 = 0, temp, A = 10;
		for (int i = 0; i < HC.size(); i++) {

			temp = HC.at(i).GetRealNumber();
			if (temp >= 10)
				temp = 10;
			sum1 = sum1 + temp;

			if (sum1 > limit)
				return true;
		}
		return false;
	}

	// 計算手牌點數
	int PrintPoint(vector<card> HC) {
		int CardCont = HC.size();
		int sum = 0, temp = 0, ACout = 0;

		for (int i = 0; i < CardCont; i++) {
			temp = HC.at(i).GetRealNumber();
			if (temp == 1)  // 假如是 A
				ACout++;
			if (temp >= 10)
				temp = 10;
			sum = sum + temp;
		}
		
		int sum2 = sum;
		int res = sum;
		for (int i = 0; i < ACout; i++) {
			sum2 = sum2 + 10;
			if (sum2 <= 21 && sum2 > sum)
				res = sum2;
		}
		return res;
	}

	// 印出牌組
	void PrintCard(vector<card> HC) {
		
		for (int i = 0; i < HC.size(); i++) {
			cout << " _____  ";
		}
		cout << endl;

		for (int i = 0; i < HC.size(); i++) {
			char y = HC.at(i).GetNumber();

			if (HC.at(i).GetCover()) 
				cout << "|     | ";
				
			else if(y == 'T')   // 確認是否為 10
				cout << "| 1 0 | ";
			else
				cout << "|  " << y << "  | ";

		}
		cout << endl;

		for (int i = 0; i < HC.size(); i++) {
			if (HC.at(i).GetCover())
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

	// 顯示目前牌桌
	void PrintDes() {  

		// 莊家
		cout << "莊家 " << banker->name << ", 籌碼 : " << banker->BargainingChip << endl;
		PrintCard(banker->HandCard);

		for (int i = 0; i < 3; i++) {   // 閒家
			cout << Spelarens[i]->name << ", 籌碼 : " << Spelarens[i]->BargainingChip << ", 下注 : " << Spelarens[i]->Ante << endl;
			PrintCard(Spelarens[i]->HandCard);
		}	
	}
};

