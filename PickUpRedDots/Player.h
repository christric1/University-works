#include <string>
#include <vector>
#include "Card.h"

using namespace std;

class Player
{
public:
	string name;  // ���a�W
	vector<Card> HandCard;  // ��P
	vector <Card> EatCard;  // �Y�쪺��P
	int ResultPoint;  // �̲פ���

	// �h��
	bool operator!=(const Player& player) {
		if (this->name == player.name)
			return false;
		return true;
	}
};

