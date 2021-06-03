#pragma once

#include <iostream>
using namespace std;
class Card
{
private:
	char suit;
	int number;
	bool cover = false;  // �O�_�\�P

public:
	Card* NextCard;   // ���V�U�@�i Card
	Card(char SetSuit, int SetNumber) :suit(SetSuit), number(SetNumber) { }  // �غc�l
	
	// �h��
	bool operator==(const Card& card) {
		if ((this->number == card.number) && (this->suit == card.suit) )
			return true;
		return false;
	}

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


