/*************************************************
* Author:			Landon Morrison
* File Name:		VingtEtUnDeck.h
* Date Created:		3/6/2020
* Modifications:	3/8/2020 - changed from Deck to VingtEtUnDeck
*
*************************************************/
#ifndef DECK_H
#define DECK_H

#include "VingtEtUnCard.h"

/***********************************
* Class: Deck
*
* Purpose: This class creates a standard deck of 52 playing cards specifically for VingtEtUn.
*
* Manager Functions:
*		Deck()
*			Default CTor that makes a standard card deck.
*		Deck(const Deck& copy)
*			Copy CTor called when passed by value, returned by value, or instantiated with an object of the same type.
*		Deck(Deck&& temp) noexcept
*			Move CTor called to create an object using values from a temporary object.
*		Deck& operator = (const Deck& rhs)
*			Copy Assignment Operator used to define how the "=" operator should function when both the lhs and rhs string objects have
*			been initialized prior to assignment.
*		Deck& operator = (Deck&& temp) noexcept
*			Move Assignement Operator used when the rhs is a temporary object such as a function return that will go out of scope
*			when the line finishes.
*		~Deck()
*			DTor for releasing any memory allocated by Deck.
*
* Deck Functions:
*		void RecombineDeck()
*			Puts the discards back into the deck.
*		void ShuffleDeck()
*			Recombines and then Shuffles the card deck.
*		Card DealCard(bool FaceUp)
*			Deals the top card, if no arg provided, will deal card face up.
*		void Discard(Card CardToDiscard)
*			Puts the passed in card into the discard pile.		
*
* Utility Functions:
*		void Display() const
*			Displays the whole deck from top to bottom.
*
***********************************/
class Deck
{
public:
	// Manager Functions
	Deck();
	Deck(const Deck& copy);
	Deck(Deck&& temp) noexcept;
	Deck& operator= (const Deck& rhs);
	Deck& operator= (Deck&& temprhs) noexcept;
	~Deck();
	
	// Deck Functions
	void RecombineDeck();
	void ShuffleDeck();
	Card DealCard(bool FaceUp = true);
	void Discard(Card CardToDiscard);

	// Getters and Setters
	const int GetTopCardIndex() const;
	const int GetDeckSize() const;
	const int GetDiscardsSize() const;
	
	// Utilities
	void Display() const;

private:
	// Data Members
	Card* m_cards = nullptr;
	Card* m_discards = nullptr;
	int m_topCardIndex = 0;
	int m_deckSize = 0;
	int m_discardsSize = 0;

	// Internal Utilities
	void Swap(Card* a, Card* b);

};
#endif