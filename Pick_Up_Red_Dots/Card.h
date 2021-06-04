#pragma once

#include <iostream>
using namespace std;
class Card
{
private:
	char suit;
	int number;
	bool cover = false;  // 是否蓋牌

public:
	Card* NextCard;   // 指向下一張 Card
	Card(char SetSuit, int SetNumber) :suit(SetSuit), number(SetNumber) { }  // 建構子
	
	// 多載
	bool operator==(const Card& card) {
		if ((this->number == card.number) && (this->suit == card.suit) )
			return true;
		return false;
	}

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
		else if (number > 1 && number < 10)
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


