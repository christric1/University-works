#include <string>
#include <vector>
#include "Card.h"

using namespace std;

class Player
{
public:
	string name;  // 玩家名
	vector<Card> HandCard;  // 手牌
	vector <Card> EatCard;  // 吃到的手牌
	int ResultPoint;  // 最終分數

	// 多載
	bool operator!=(const Player& player) {
		if (this->name == player.name)
			return false;
		return true;
	}
};

