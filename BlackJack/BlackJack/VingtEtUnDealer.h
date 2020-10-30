/*************************************************
* Author:			Landon Morrison
* File Name:		VingtEtUnDealer.h
* Date Created:		3/6/2020
* Modifications:
*
*************************************************/
#ifndef DEALER_H
#define DEALER_H

#include "VingtEtUnPlayer.h"

/***********************************
* Class: Dealer
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
class Dealer : public Player
{
public:
	// Manager Functions
	Dealer();
	~Dealer();


private:
	friend class VingtEtUn;


};

#endif