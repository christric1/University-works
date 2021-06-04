#include <iostream>
#include "Game.h"

using namespace std;

int main() {

	deck miko;  // 建立牌堆

	Game gura(miko);  // 建立遊戲

	gura.GameStart();  // 開始遊戲

	return 0;
}
