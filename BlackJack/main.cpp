#include <iostream>
#include "Game.h"

using namespace std;

int main() {

	deck miko;  

	Game gura(miko);  

	gura.GameStart(); 

	return 0;
}
