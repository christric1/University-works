#include <iostream>
#include <string>
#include <vector>
#include "Deck.h"
#include "Player.h"

using namespace std;

class Game
{
private:
	Deck dck;   // �P��
	Player TA;  // �U��
	vector <Player> CP;  // �q�����a
	vector <Card> TableCard;  // ��W�������P
	int PlayerCnt;   // ���a�H��

public:

	void GameStart() {   // �C���}�l

		cout << "�w��Ө��Ӥ] -- �߬��I !! " << endl;
		cout << "�аݪ��a�m�W : ";
		cin >> TA.name;
		cout << "�аݷQ�n�X�ӤH�� ?  2~4�H ";
		cin >> PlayerCnt;

		cout << endl << "-----------------------------------------------" << endl;

		CreateComputerPlayer();  // �Ыعq�����a
		dck.Shuffle();   // �~�P

		for (int i = 0; i < 24 / PlayerCnt; i++)   // �U�Щ�P
			TA.HandCard.push_back(dck.PopCard());

		for (int i = 0; i < CP.size(); i++)    // �q�����a��P
			for (int j = 0; j < 24 / PlayerCnt; j++)
				CP.at(i).HandCard.push_back(dck.PopCard());
			
		for (int i = 0; i < 4; i++)    // ��W�o4�i�P
			TableCard.push_back(dck.PopCard());


		Player** PriorityQueue = new Player*[PlayerCnt];   // �إ߽��y����
		PriorityQueue[0] = &TA;
		for (int i = 1; i < PlayerCnt; i++)
			PriorityQueue[i] = &CP.at(i - 1);    // TA -> John -> Mike -> Mao

		int serial = 0;  

		while (dck.CheckDeckHaveCard()) {

			cout << "��" << (serial / PlayerCnt) + 1 << "�� -- " << PriorityQueue[serial % PlayerCnt]->name << "���^�X" << endl;
			PrintDes();   // �L�X��W�P��
			cout << endl << "-----------------------------------------------" << endl;

			if ((serial % PlayerCnt) != 0)  // �O�_����q��
				AutoPlay(*PriorityQueue[serial % PlayerCnt]);

			else if (CheckCanEat(TA)) {   // �P�����o�Y

				int opt;
				cout << "1. ��ܦY  2. ��ܥ�P ?  1 or 2 " ;
				cin >> opt;

				if (opt == 1) {   // ��ܦY
					int cho;
					vector <Card> Ca = CanEatCard(TA);
					PrintCard(Ca);
					cout << "�аݷQ�Y��W���@�i ? ";
					cout << " 1 ~ " << Ca.size() << "  ";
					cin >> cho;
					CardEat(Ca.at(cho - 1), TA);   // �Y�P
				}

				else {  // ��ܥ�
					int cho;
					PrintCard(TA.HandCard);
					cout << "�аݷQ����@�i ? " << " 1 ~ " << TA.HandCard.size() << " ";
					cin >> cho;
					ThrowCard(TA.HandCard.at(cho - 1), TA);  // ��P
				}

				DrawCard(TA);  // ½�P
			}

			else {   // �P���S�o�Y

				int cho;
				PrintCard(TA.HandCard);
				cout << "�S�o�Y�A�аݷQ����@�i ? " << " 1 ~ " << TA.HandCard.size() <<  " " ;
				cin >> cho;
				ThrowCard(TA.HandCard.at(cho - 1), TA);  // ��P

				DrawCard(TA);  // ½�P
			}

			if ((serial % PlayerCnt) == 0) {
				cout << "TA ����P" << endl;
				PrintCard(TA.HandCard);
				cout << "TA �Y������P" << endl;
				PrintCard(TA.EatCard);
			}

			cout << endl << "-----------------------------------------------" << endl;
			serial++;
		}

		ResultCompute();   // ���Ƶ���
		PrintPoint();   // �L�X����

	}

	void PrintPoint() {   // �L�X����

		cout << endl << "�̲פ��� " << endl << endl;
		cout << TA.name << " : " << TA.ResultPoint << endl;
		for (int i = 0; i < PlayerCnt - 1; i++)
			cout << CP.at(i).name << " : " << CP.at(i).ResultPoint << endl;
		cout << endl;
	}

	int EatCardPoint(Player pl) {   // �Y�P���Ƶ���
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
			if (pl.EatCard.at(i).GetSuit() == 'S' && temp == 1)  // �®�A
				sum = sum + 10;
		}
		return sum;
	}

	void ResultCompute() {  // ���Ƶ���

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
		else if (y == 'T')   // �T�{�O�_�� 10
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

		cout << " �¡¡¡¡�  ";
		cout << endl;
	}

	void PrintCard(vector<Card> HC, bool cover=false) {  	// �L�X�P��

		for (int i = 0; i < HC.size(); i++) {
			cout << " _____  ";
		}
		cout << endl;

		for (int i = 0; i < HC.size(); i++) {
			char y = HC.at(i).GetNumber();

			if (cover)
				cout << "|     | ";

			else if (y == 'T')   // �T�{�O�_�� 10
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
			cout << " �¡¡¡¡�  ";
		}
		cout << endl;
	}

	void PrintDes() {   // ��ܥثe�P��

		cout << "-----------------------------------------------" << endl;

		// ��W���P
		cout << "��W���P : " << endl;
		PrintCard(TableCard);

		cout << endl << "-----------------------------------------------" << endl;

		// TA���P
		cout << TA.name << " : " << endl;
		PrintCard(TA.HandCard);

		// �q�����P
		for (int i = 0; i < CP.size(); i++) {
			cout << CP.at(i).name << " : " << endl;
			PrintCard(CP.at(i).HandCard, true);
		}

	}

	bool CheckCanEat(Player pl) {   // �T�{�O�_��Y
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

	vector<Card> CanEatCard(Player pl) {  // ��Y���P

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

	void DrawCard(Player &pl) {  // ½�P

		Card Ca = dck.PopCard();
		int temp = Ca.GetRealNumber();

		cout << pl.name << " ½�P~" << endl;
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
			pl.EatCard.push_back(RD.at(0));   // �Y����W���P
			pl.EatCard.push_back(Ca);   // �Y��½�}���P
			TakeCardOff(RD.at(0), TableCard);   // ������W���P
		}

		else if (RD.size() > 1 && !(pl != TA)){    // ��W����i�H�W�@�˼Ʀr���P
			int cho;
			PrintCard(RD);
			cout << "�A�n��ܦY��W���i ? " << "1 ~ " << RD.size();
			cin >> cho;

			pl.EatCard.push_back(RD.at(cho - 1));   // �Y����W���P
			pl.EatCard.push_back(Ca);   // �Y��½�}���P
			TakeCardOff(RD.at(cho - 1), TableCard);   // ������W���P

		}

		else {   // ��W�����i�Y�����P
			TableCard.push_back(Ca);
		}

	}

	void CardEat(Card ca, Player &pl) {  // �Y�P

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

		pl.EatCard.push_back(ca);  // �Y���⤤���P
		TakeCardOff(ca, pl.HandCard);  // ������W���P

		if (RD.size() == 1 || pl != TA){
			pl.EatCard.push_back(RD.at(0));   // �Y����W���P
			TakeCardOff(RD.at(0), TableCard);   // ������W���P
		}

		else {    // ��W����i�H�W�@�˼Ʀr���P
			int cho;
			PrintCard(RD);
			cout << "�A�n��ܦY��W���i ? " << "1 ~ " << RD.size() << " ";
			cin >> cho;

			pl.EatCard.push_back(RD.at(cho - 1));   // �Y����W���P
			TakeCardOff(RD.at(cho-1), TableCard);   // ������W���P
			
		}
		cout << endl;
	}

	void ThrowCard(Card ca, Player &pl) {   // ��P���W

		TakeCardOff(ca, pl.HandCard);  // ������W���P
		TableCard.push_back(ca);  // �W�[��W���P
		cout << endl;
	}

	void TakeCardOff(Card ca, vector<Card> &cd) {   // �����P
		
		for (int i = 0; i < cd.size(); i++) {
			if (ca == cd.at(i))
				cd.erase(cd.begin() + i);
		}

	}

	void CreateComputerPlayer() {   // �إ߹q�����a�H��
		string name[] = { "John", "Mike", "Mao" };
		for (int i = 0; i < PlayerCnt-1; i++) {
			Player temp;
			temp.name = name[i];
			CP.push_back(temp);
		}
	}

	void AutoPlay(Player &pl) {  // �q���۰ʾާ@

		if (CheckCanEat(pl)) {
			vector <Card> Ca = CanEatCard(pl);
			CardEat(Ca.at(0), pl);   // �Y�P
			cout << pl.name << " �Y�P~" << endl;
			PrintCard(Ca.at(0));
		}

		else {
			Card temp = pl.HandCard.at(0);
			ThrowCard(temp, pl);  // ��P
			cout << pl.name << " ��P~" << endl;
			PrintCard(temp);
		}

		DrawCard(pl);  // ½�P
	}
};