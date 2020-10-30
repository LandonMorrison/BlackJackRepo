/*************************************************
* Author:			Landon Morrison
* File Name:		VingtEtUnHand.h
* Date Created:		3/6/2020
* Modifications:	3/8/2020 - changed from Hand to VingtEtUnHand
*
*************************************************/
#ifndef HAND_H
#define HAND_H

#include "VingtEtUnDeck.h"

/***********************************
* Class: VingtEtUnHand
*
* Purpose: This class creates a hand of cards for VingtEtUn.
*
* Manager Functions:
*		Hand()
*			Default CTor creates an empty hand.
*		Hand(const Hand& copy)
*			Copy CTor called when passed by value, returned by value, or instantiated with an object of the same type.
*		Hand(Hand&& temp) noexcept
*			Move CTor called to create an object using values from a temporary object.
*		Hand& operator = (const Hand& rhs)
*			Copy Assignment Operator used to define how the "=" operator should function when both the lhs and rhs string objects have
*			been initialized prior to assignment.
*		Hand& operator = (Hand&& temp) noexcept
*			Move Assignement Operator used when the rhs is a temporary object such as a function return that will go out of scope
*			when the line finishes.
*		~Hand()
*			DTor for releasing any memory allocated by Hand.
*
* Hand Functions:
*		Card Discard(int offset)
*			Specific to VingtEtUn, returns a temporary copy of the card at the given offset to be added to the deck's discard pile
*			and then deletes the card from the hand.
*
***********************************/
class Hand
{
public:
	// Manager Functions
	Hand();
	Hand(const Hand& copy);
	Hand(Hand&& temp) noexcept;
	Hand& operator=(const Hand& rhs);
	Hand& operator=(Hand&& temp) noexcept;
	~Hand();

	// Hand Functions
	void AddCard(Card CardIn);
	Card Discard(int offset);
	int CalculateHandScore();

	// Getters and Setters
	const int GetHandSize() const;
	const Card GetCard(int offset) const;
	const bool GetIsInPlay() const;
	void SetIsInPlay(bool PlayIn);

	// Utilities
	void Display();
	void FlipCard(int offset, bool FaceUp);

private:
	// Data Members
	Card* m_cards = nullptr;
	int m_handSize = 0;
	int m_handScore = 0;
	bool isInPlay = false;

	// Internal Utilities
	void PrintAscci(int ascii) const;
	void PrintCards() const;
	void PrintTops() const;
	void PrintTopChars() const;
	void PrintMids() const;
	void PrintBotChars() const;
	void PrintBots() const;

};
#endif