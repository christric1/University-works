#include <iostream>
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */

using namespace std;

class card
{
private:
	char suit;
	int number;
	bool cover = false;  // 是否蓋牌

public:
	card* NextCard;   // 指向下一張 card
	card(char SetSuit, int SetNumber) :suit(SetSuit), number(SetNumber) { }  // 建構子

	// 回傳 數字 (真)
	int GetRealNumber() {
		return number;
	}

	// 設定 Cover
	void SetCover(bool set) {
		cover = set;
	}

	// 回傳 Cover 
	bool GetCover() {
		return cover;
	}

	// 回傳 suit
	char GetSuit() {
		return suit;
	}

	// 回傳 數字
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
	card* TopCard;    // 排堆第一張牌
	card* EndCard;    // 牌堆最後一張牌
	card *peko[53];   // peko[0] 不放東西

public:
	deck(){  // 建構子
		DeckReset();
	};   

	// 抽牌
	card PopCard() {
		card* temp = TopCard;
		TopCard = TopCard->NextCard;
		return *temp;
	}

	// 重置牌組
	void DeckReset() {

		peko[1] = new card('S', 1);    // 宣告 deck 最上面的牌 黑桃A

		// 按照新牌順序
		for (int i = 2; i <= 52; i++) {     // 宣告 其他 51張牌
			if (i <= 13)
				peko[i] = new card('S', hina(i));  // 黑桃
			else if (14 <= i && i <= 26)
				peko[i] = new card('H', hina(i));  // 愛心
			else if (27 <= i && i <= 39)
				peko[i] = new card('D', hina(i));  // 菱形
			else
				peko[i] = new card('C', hina(i));  // 梅花

			peko[i - 1]->NextCard = peko[i];
		}

		TopCard = peko[1];
		EndCard = peko[52];
		EndCard->NextCard = NULL;
	}

	int hina(int i) {      // 回傳 1~13
		int k = i % 13;
		if (k == 0)
			return 13;
		else
			return k;
	}

	// 印出 deck 上 數量為 number 的牌
	void PoPCard(int number) {    

		for (int i = 0; i < number; i++) {
			cout << " _____  ";
		}
		cout << endl;

		card* temp = TopCard;       // temp 指向 TopCard 
		for (int i = 0; i < number; i++) {
			char y = temp->GetNumber();

			if (y == 'T')   // 確認是否為 10
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
			cout << " ¯¯¯¯¯  ";
		}
		cout << endl;
	}

	// 洗牌
	void Shuffle() {

		srand(time(NULL));
		int num;

		for (int i = 0; i < 100; i++) {

			num = (rand() % 51) + 1;  // 亂數選擇 1~51
			card* temp;

			if (num == 1) {   // 頭頭
				temp = TopCard;
				TopCard = TopCard->NextCard;
			}

			else {  // 中間
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
	 ¯¯¯¯¯   ¯¯¯¯¯
*/