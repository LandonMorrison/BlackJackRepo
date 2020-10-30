/*************************************************
* Author:			Landon Morrison
* File Name:		VingtEtUn.h
* Date Created:		3/6/2020
* Modifications:
*
*************************************************/
#ifndef VINGTETUN_H
#define VINGTETUN_H

#include "VingtEtUnDealer.h"

/***********************************
* Class: VingtEtUn
*
* Purpose: This is the game class for Vingt Et Un otherwise known as 21 or Blackjack
*
* Manager Functions:
*		VingtEtUn()
*			Default CTor.
*		~VingtEtUn()
*			DTor
*
* Game Functions:
*		
*
***********************************/
class VingtEtUn
{
public:
	// Manager Functions
	VingtEtUn();
	~VingtEtUn();

	// Game Functions
	void PlayGame();
	void PlayRound();
	void DealRound();
	void InsuranceChecks();
	void CheckForNaturals();
	void CheckForSplits();
	void CheckForDoubleDowns();
	void PlayerTurns();
	void DealerTurn();

	// Utilities
	void InsurancePayouts();
	void PayOut(int playerOffset, int handOffset, float multiplier);

private:
	// Data Members
	Player* m_players = nullptr;
	Dealer m_dealer;
	int m_numPlayers = 0;
	Deck m_deck;
	int PlayAgains = 0;

};
#endif