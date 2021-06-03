#include <iostream>
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */
#include "Card.h"

using namespace std;

class Deck
{
private:
	Card* TopCard;    // 排堆第一張牌
	Card* EndCard;    // 牌堆最後一張牌
	Card* peko[53];   // peko[0] 不放東西

public:
	Deck() {  // 建構子
		DeckReset();
	};

	// 抽牌
	Card PopCard () {
		Card* temp = TopCard;
		TopCard = TopCard->NextCard;
		return *temp;
	}

	// 重置牌組
	void DeckReset() {

		peko[1] = new Card('S', 1);    // 宣告 Deck 最上面的牌 黑桃A

		// 按照新牌順序
		for (int i = 2; i <= 52; i++) {     // 宣告 其他 51張牌
			if (i <= 13)
				peko[i] = new Card('S', hina(i));  // 黑桃
			else if (14 <= i && i <= 26)
				peko[i] = new Card('H', hina(i));  // 愛心
			else if (27 <= i && i <= 39)
				peko[i] = new Card('D', hina(i));  // 菱形
			else
				peko[i] = new Card('C', hina(i));  // 梅花

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

	// 洗牌
	void Shuffle() {

		srand(time(NULL));
		int num;

		for (int i = 0; i < 100; i++) {

			num = (rand() % 51) + 1;  // 亂數選擇 1~51
			Card* temp;

			if (num == 1) {   // 頭頭
				temp = TopCard;
				TopCard = TopCard->NextCard;
			}

			else {  // 中間
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

	bool CheckDeckHaveCard() {   // 確認是否還有牌
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
	 ¯¯¯¯¯   ¯¯¯¯¯
*/

