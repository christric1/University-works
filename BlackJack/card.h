#include <iostream>
#include <cstdlib> /* �üƬ������ */
#include <ctime>   /* �ɶ�������� */

using namespace std;

class card
{
private:
	char suit;
	int number;
	bool cover = false;  // �O�_�\�P

public:
	card* NextCard;   // ���V�U�@�i card
	card(char SetSuit, int SetNumber) :suit(SetSuit), number(SetNumber) { }  // �غc�l

	// �^�� �Ʀr (�u)
	int GetRealNumber() {
		return number;
	}

	// �]�w Cover
	void SetCover(bool set) {
		cover = set;
	}

	// �^�� Cover 
	bool GetCover() {
		return cover;
	}

	// �^�� suit
	char GetSuit() {
		return suit;
	}

	// �^�� �Ʀr
	char GetNumber() {
		if (number == 1)
			return 'A';
		else if ( number >1 && number < 10)
			return number + 48;    // ASCII 
		else if (number == 10)
			return 'T';
		else if (number == 11)
			return 'J';
		else if (number == 12)
			return 'Q';
		else
			return 'K';
	}
};


class deck
{
private:
	card* TopCard;    // �ư�Ĥ@�i�P
	card* EndCard;    // �P��̫�@�i�P
	card *peko[53];   // peko[0] ����F��

public:
	deck(){  // �غc�l
		DeckReset();
	};   

	// ��P
	card PopCard() {
		card* temp = TopCard;
		TopCard = TopCard->NextCard;
		return *temp;
	}

	// ���m�P��
	void DeckReset() {

		peko[1] = new card('S', 1);    // �ŧi deck �̤W�����P �®�A

		// ���ӷs�P����
		for (int i = 2; i <= 52; i++) {     // �ŧi ��L 51�i�P
			if (i <= 13)
				peko[i] = new card('S', hina(i));  // �®�
			else if (14 <= i && i <= 26)
				peko[i] = new card('H', hina(i));  // �R��
			else if (27 <= i && i <= 39)
				peko[i] = new card('D', hina(i));  // �٧�
			else
				peko[i] = new card('C', hina(i));  // ����

			peko[i - 1]->NextCard = peko[i];
		}

		TopCard = peko[1];
		EndCard = peko[52];
		EndCard->NextCard = NULL;
	}

	int hina(int i) {      // �^�� 1~13
		int k = i % 13;
		if (k == 0)
			return 13;
		else
			return k;
	}

	// �L�X deck �W �ƶq�� number ���P
	void PoPCard(int number) {    

		for (int i = 0; i < number; i++) {
			cout << " _____  ";
		}
		cout << endl;

		card* temp = TopCard;       // temp ���V TopCard 
		for (int i = 0; i < number; i++) {
			char y = temp->GetNumber();

			if (y == 'T')   // �T�{�O�_�� 10
				cout << "| 1 0 | ";
			else 
				cout << "|  " << y << "  | ";

			temp = temp->NextCard;
		}
		cout << endl;

		for (int i = 0; i < number; i++) {
			cout << "|  " << TopCard->GetSuit() << "  | ";
			TopCard = TopCard->NextCard;
		}
		cout << endl;

		for (int i = 0; i < number; i++) {
			cout << "|     | ";
		}
		cout << endl;

		for (int i = 0; i < number; i++) {
			cout << " �¡¡¡¡�  ";
		}
		cout << endl;
	}

	// �~�P
	void Shuffle() {

		srand(time(NULL));
		int num;

		for (int i = 0; i < 100; i++) {

			num = (rand() % 51) + 1;  // �üƿ�� 1~51
			card* temp;

			if (num == 1) {   // �Y�Y
				temp = TopCard;
				TopCard = TopCard->NextCard;
			}

			else {  // ����
				card* temp2;
				temp2 = TopCard;
				for (int i = 0; i < num-2; i++) {
					temp2 = temp2->NextCard;
				}
				temp = temp2->NextCard;
				temp2->NextCard = temp->NextCard;
			}

			EndCard->NextCard = temp;
			EndCard = temp;
			EndCard->NextCard = NULL;
		}
	}

};

/*
     _____   _____
    |  5  | | 1 0 |
	|  D  | |  S  |
	|     | |     |
	 �¡¡¡¡�   �¡¡¡¡�
*/