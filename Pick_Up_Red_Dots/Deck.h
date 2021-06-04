#include <iostream>
#include <cstdlib> /* �üƬ������ */
#include <ctime>   /* �ɶ�������� */
#include "Card.h"

using namespace std;

class Deck
{
private:
	Card* TopCard;    // �ư�Ĥ@�i�P
	Card* EndCard;    // �P��̫�@�i�P
	Card* peko[53];   // peko[0] ����F��

public:
	Deck() {  // �غc�l
		DeckReset();
	};

	// ��P
	Card PopCard () {
		Card* temp = TopCard;
		TopCard = TopCard->NextCard;
		return *temp;
	}

	// ���m�P��
	void DeckReset() {

		peko[1] = new Card('S', 1);    // �ŧi Deck �̤W�����P �®�A

		// ���ӷs�P����
		for (int i = 2; i <= 52; i++) {     // �ŧi ��L 51�i�P
			if (i <= 13)
				peko[i] = new Card('S', hina(i));  // �®�
			else if (14 <= i && i <= 26)
				peko[i] = new Card('H', hina(i));  // �R��
			else if (27 <= i && i <= 39)
				peko[i] = new Card('D', hina(i));  // �٧�
			else
				peko[i] = new Card('C', hina(i));  // ����

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

	// �~�P
	void Shuffle() {

		srand(time(NULL));
		int num;

		for (int i = 0; i < 100; i++) {

			num = (rand() % 51) + 1;  // �üƿ�� 1~51
			Card* temp;

			if (num == 1) {   // �Y�Y
				temp = TopCard;
				TopCard = TopCard->NextCard;
			}

			else {  // ����
				Card* temp2;
				temp2 = TopCard;
				for (int i = 0; i < num - 2; i++) {
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

	bool CheckDeckHaveCard() {   // �T�{�O�_�٦��P
		if (TopCard == EndCard->NextCard)
			return false;
		return true;
	}
};

/*
	 _____   _____
	|  5  | | 1 0 |
	|  D  | |  S  |
	|     | |     |
	 �¡¡¡¡�   �¡¡¡¡�
*/

