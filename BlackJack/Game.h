#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdlib> /* �üƬ������ */
#include <ctime>   /* �ɶ�������� */
#include "card.h"

using namespace std;

class player 
{
public:
	string name;  // ���a�W
	int BargainingChip = 10000;   // �w�X
	int Ante = 1000; // ���`
	vector<card> HandCard;  // ��P
	bool BlackJack = false; // �³ǧJ
	bool surrender = false; //�뭰
	bool Insurance = false;  // �O�I
	bool DoubleDown = false;  // �����[�`
	bool Over = false; // �O�_�W�L 21�I �z��
};

class Game
{
private:
	int round; 
	deck dck;   // �P��
	player* banker; // ���a
	player* Spelarens[3]; // ���a
	player TA;  // �U��
	player p[3]; // �T��q�����a

public:
	Game(deck miko) : dck(miko) {   // �غc�l
		p[0].name = "Chris";
		p[1].name = "Scott";
		p[2].name = "John";
	}     

	void DecideSpe() {  // �M�w���a
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

	void GameStart() {  // �}�l����

		bool sur;
		char ans;
		cout << "�w��Ө� ��Ӥ]-21�I!!" << endl << "�аݪ��a�W��?   ";
		cin >> TA.name;
		cout << TA.name << "  �Q���X��?  ";
		cin >> round;
		cout << endl;

		int x = rand() % 3;   // �M�w���a
		banker = &p[x];
		DecideSpe();

		for (int i = 0; i < round; i++) {   // ���� round ����
			sur = false;
			cout << "-------------------------------------------------------------------------------" << endl << endl;
			cout << "��" << i+1 << "�� : " << banker->name <<" �����a" << endl << endl;
			cout << "�z�Q�U�`�h�� ? ";
			cin >> TA.Ante;
			cout << endl;

			dck.DeckReset();  // �P�խ��m
			dck.Shuffle();  // �~�P

			for (int i = 0; i < 3; i++) {   // �w�X��������
				Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip - Spelarens[i]->Ante;
			}
			
			for (int i = 0; i < 3; i++) {   // ���a����i�P
				Spelarens[i]->HandCard.push_back(dck.PopCard());
				Spelarens[i]->HandCard.push_back(dck.PopCard());
			}

			banker->HandCard.push_back(dck.PopCard());    // ���a����i�P
			banker->HandCard.push_back(dck.PopCard());
			banker->HandCard.at(1).SetCover(true);

			PrintDes();   // ��ܵP��
			cout << "-------------------------------------------------------------------------------" << endl << endl;
			cout << "�z����P :�@" << endl;
			PrintCard(TA.HandCard);  // ��ܤ�P
			
			if (banker->HandCard.at(0).GetNumber() == 'A') {    // �T�{���a�Ĥ@�i���P�O�_�� A
				sur = true;
			}
			
			if (Spelarens[0] == &TA) {  // TA�O���a

				if (BlackJack(TA.HandCard)) {  // �³ǧJ�T�{�{��
					TA.BlackJack = true;
					cout << "�z�O�³ǧJ" << endl << endl;
				}
				
				cout << "�O�_�뭰 ? y/n ";  // �뭰�{��
				cin >> ans;
				if (ans == 'y')  
					TA.surrender = true;

				if (sur) {  // �O�I�{��
					cout << "�O�_�n�ʶR�O�I ? y/n ";
					cin >> ans;
					if (ans == 'y') 
						TA.Insurance = true;
				}

				if ((TA.HandCard.at(0).GetNumber() + TA.HandCard.at(1).GetNumber() == 107) && !(TA.surrender)) {  // �����[�`�{��
					cout << "�O�_�n�����[�` ? y/n ";
					cin >> ans;
					if (ans == 'y')
						TA.DoubleDown = true;
				}
				cout << endl;

				while (!OverCheck(TA.HandCard, 21) && !(TA.BlackJack) && !(TA.surrender)) {   // �[�P�{��
					cout << "�O�_�~��[�P ? y/n ";
					cin >> ans;
					if (ans == 'n')
						break;
					TA.HandCard.push_back(dck.PopCard());  // ��P
					PrintCard(TA.HandCard);  // ��ܤ�P
				}

				TA.Over = OverCheck(TA.HandCard, 21);  // �T�{�O�_�z�P

				if (TA.Over)
					cout << "�z�z�P�F!!" << endl << endl;


				AutoPlay(banker);  // �۰ʵ{��
				AutoPlay(Spelarens[1]);
				AutoPlay(Spelarens[2]);

			}

			else {   // TA �O���a

				while (!OverCheck(TA.HandCard, 21) && !(TA.BlackJack)) {   // �[�P�{��
					cout << "�O�_�~��[�P ? y/n ";
					cin >> ans;
					if (ans == 'n')
						break;
					TA.HandCard.push_back(dck.PopCard());  // ��P
				}

				AutoPlay(Spelarens[0]);
				AutoPlay(Spelarens[1]);
				AutoPlay(Spelarens[2]);

			}

			cout << "-------------------------------------------------------------------------------" << endl << endl;
			cout << "�ثe�P�� : " << endl << endl;
			banker->HandCard.at(1).SetCover(false);

			InsuranceHandle(); // �O�I����

			PrintDes();  // ��ܵP��

			ResultCompute();   // ����

			PrintBar();  // ���Ƨe�{

			Reset();  // �k�s
			
			player* ptemp;
			ptemp = banker;
			for (int i = 0; i < 3; i++) {
				if (Spelarens[i]->BargainingChip > ptemp->BargainingChip) {
					ptemp = Spelarens[i];
				}
			}
			banker = ptemp;
			DecideSpe();  // �M�w���a
		}

		cout << "Ĺ�a�� " << banker->name << endl;
	} 
	
	// �k�s
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

	// �O�I�{��
	void InsuranceHandle() {
		bool temp = false;
		if (BlackJack(banker->HandCard))
			temp = true;

		for (int i = 0; i < 3; i++) {
			if (Spelarens[i]->Insurance && temp) {   // ���O�I �B ���a�³ǧJ
				Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip + Spelarens[i]->Ante;
				banker->BargainingChip = banker->BargainingChip - Spelarens[i]->Ante;
			}
			else if (Spelarens[i]->Insurance && !temp){  // ���O�I �B ���a���O�³ǧJ
				Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip - (Spelarens[i]->Ante)/2;
				banker->BargainingChip = banker->BargainingChip + (Spelarens[i]->Ante) / 2;
			}
		}
	}

	// ���Ƨe�{
	void PrintBar() {
		cout << "-------------------------------------------------------------------------------" << endl << endl;
		cout << "���Ƶ��� : " << endl << endl;
		cout << "���a " << banker->name << ", �w�X : " << banker->BargainingChip << endl;
		for (int i = 0; i < 3; i++) 
			cout << "���a " << Spelarens[i]->name << ", �w�X : " << Spelarens[i]->BargainingChip << endl;
		cout << endl;
	}

	// �q���۰�
	void AutoPlay(player* p) {

		p->BlackJack = BlackJack(p->HandCard);

		if (p->BlackJack) 
			return;

		while (!OverCheck(p->HandCard, 17)) 
			p->HandCard.push_back(dck.PopCard());
		p->Over = OverCheck(p->HandCard, 21);

	}

	// ����
	void ResultCompute() {

		bool pat = false;
		bool spe = false;
		if (banker->Over) {  // ���a�z�P
			for (int i = 0; i < 3; i++) {

				if (Spelarens[i]->DoubleDown) {   // �O�_�����U�`
					Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip - Spelarens[i]->Ante;
					Spelarens[i]->Ante = (Spelarens[i]->Ante) * 2;
				}

				if (Spelarens[i]->surrender) {  // �O�_�뭰
					Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip + (Spelarens[i]->Ante) / 2;
					banker->BargainingChip = banker->BargainingChip + (Spelarens[i]->Ante) / 2;
				}

				else if (FiveDragon(Spelarens[i]->HandCard)) {   // �O�_�L����
					Spelarens[i]->BargainingChip = (Spelarens[i]->Ante) * 4 + Spelarens[i]->BargainingChip;
					banker->BargainingChip = banker->BargainingChip - (Spelarens[i]->Ante) * 3;
				}

				else if (Straight(Spelarens[i]->HandCard)) {  // �O�_�����l
					Spelarens[i]->BargainingChip = (Spelarens[i]->Ante) * 4 + Spelarens[i]->BargainingChip;
					banker->BargainingChip = banker->BargainingChip - (Spelarens[i]->Ante) * 3;
				}

				else if (Spelarens[i]->Over) {  // �z��
					Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip + Spelarens[i]->Ante;
				}

				else  {  // �S���z��
					Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip + (Spelarens[i]->Ante) * 2;
					banker->BargainingChip = banker->BargainingChip - Spelarens[i]->Ante;
				}
			}
		}

		else {  // ���a�S�z�P
			if (Straight(banker->HandCard) || FiveDragon(banker->HandCard))  // �O�_���S��P��
				pat = true;

			for (int i = 0; i < 3; i++) {

				if (Straight(Spelarens[i]->HandCard) || FiveDragon(Spelarens[i]->HandCard))
					spe = true;

				if (Spelarens[i]->surrender) {  // �O�_�뭰
					Spelarens[i]->BargainingChip = Spelarens[i]->BargainingChip + (Spelarens[i]->Ante) / 2;
					banker->BargainingChip = banker->BargainingChip + (Spelarens[i]->Ante) / 2;
				}

				else if (Spelarens[i]->Over) {   // �O�_�z��
					banker->BargainingChip = banker->BargainingChip + Spelarens[i]->Ante;
				}

				else if (pat) { // ���a���S��P��
					if (spe) {  // ���a�]�O�S��P��
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

					else {  // ���a���O�S��P��
						banker->BargainingChip = banker->BargainingChip + Spelarens[i]->Ante;
					}
				}

				else {  // ���a���O�S��P��
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

	// �O�_�����l
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

	// �O�_�L���� 
	bool FiveDragon(vector<card> HC) {
		if (HC.size() >= 5) 
			return true;
		return false;
	}
	
	// ���լO�_�� blackjack
	bool BlackJack (vector<card> HC) {

		int sum = HC.at(0).GetNumber() + HC.at(1).GetNumber();
		if (sum == 139 || sum == 146 || sum == 140 || sum == 149)  // ASCII
			return true;
		return false;
	}

	// ���լO�_���W�L 21�I
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

	// �p���P�I��
	int PrintPoint(vector<card> HC) {
		int CardCont = HC.size();
		int sum = 0, temp = 0, ACout = 0;

		for (int i = 0; i < CardCont; i++) {
			temp = HC.at(i).GetRealNumber();
			if (temp == 1)  // ���p�O A
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

	// �L�X�P��
	void PrintCard(vector<card> HC) {
		
		for (int i = 0; i < HC.size(); i++) {
			cout << " _____  ";
		}
		cout << endl;

		for (int i = 0; i < HC.size(); i++) {
			char y = HC.at(i).GetNumber();

			if (HC.at(i).GetCover()) 
				cout << "|     | ";
				
			else if(y == 'T')   // �T�{�O�_�� 10
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
			cout << " �¡¡¡¡�  ";
		}
		cout << endl;
	}

	// ��ܥثe�P��
	void PrintDes() {  

		// ���a
		cout << "���a " << banker->name << ", �w�X : " << banker->BargainingChip << endl;
		PrintCard(banker->HandCard);

		for (int i = 0; i < 3; i++) {   // ���a
			cout << Spelarens[i]->name << ", �w�X : " << Spelarens[i]->BargainingChip << ", �U�` : " << Spelarens[i]->Ante << endl;
			PrintCard(Spelarens[i]->HandCard);
		}	
	}
};

