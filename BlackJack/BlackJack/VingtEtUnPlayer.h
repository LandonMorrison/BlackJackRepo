/*************************************************
* Author:			Landon Morrison
* File Name:		VingtEtUnPlayer.h
* Date Created:		3/6/2020
* Modifications:	3/8/2020 Adjusted to match the new Card, Deck, and Hand
*
*************************************************/
#ifndef PLAYER_H
#define PLAYER_H
#include <string>

#include "VingtEtUnHand.h"

using std::string;

/***********************************
* Class: Player
*
* Purpose: This is the default player class for Vingt Et Un otherwise known as 21 or Blackjack
*
* Manager Functions:
*		Player()
*			Default CTor
*		~Player()
*			DTor for releasing any memory allocated by Player.
*
***********************************/
class Player
{
public:
	// Manager Functions
	Player();
	Player(const Player& copy);
	Player(Player&& temp) noexcept;
	Player& operator=(const Player& rhs);
	Player& operator=(Player&& temprhs) noexcept;
	~Player();

	// Player Functions
	void Bet();
	bool BuyIn();
	void Split(Card CardForHand, Card CardForSplitHand); // This function currently has no safety checks and is only set up for 2 hands so be careful with it
	void AddToPurse(int AmountToAdd);

	// Utilities
	void AddHand();
	void RemoveHand(int handOffset);
	void Display() const;
	void CheckForInsurance();
	void ResetAmountBet();
	void ResetSplitBet();
	void ResetInsuranceBet();

	// Getters and Setters
	const string GetName() const;
	const int GetPurse() const;
	const bool GetNumHands() const;
	const int GetAmountBet() const;
	const int GetSplitBet() const;
	const int GetInsuranceBet() const;
	const bool GetOutOfGame() const;
	void SetName(string NameIn);

private:
	friend class VingtEtUn; // Can't have the game without players and vice versa

	// Data Members
	string m_name = "";
	int m_purse = 500;
	int m_numHands = 0;
	bool PlayAgain = true;

protected:
	Hand* m_hands = nullptr;

private:
	// Currency
	int AmountBet = 0;
	int InsuranceBet = 0;
	int SplitBet = 0;
	bool hasCurrency = true;
	

};

#endif