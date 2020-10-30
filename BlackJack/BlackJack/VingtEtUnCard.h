/*************************************************
* Author:			Landon Morrison
* File Name:		VingtEtUnCard.h
* Date Created:		3/6/2020
* Modifications:	3/8/2020 - changed from Card to VingtEtUnCard
*
*************************************************/
#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <windows.h>
#include <time.h>

enum class RANK { ACE = 1, DEUCE, TREY, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };
enum class SUIT { SPADES, HEARTS, CLUBS, DIAMONDS };

/***********************************
* Class: Card
*
* Purpose: This class creates a card with a rank and suit that is face up
*
* Manager Functions:
*		Card(RANK RankIn = ACE, SUIT SuitIn = SPADES)
*			Overloaded CTor that will default to ACE of SPADES if given no arguments
*		Card(const Card& copy)
*			Copy CTor called when passed by value, returned by value, or instantiated with an object of the same type.
*		Card(Card&& temp) noexcept
*			Move CTor called to create an object using values from a temporary object.
*		Card& operator = (const Card& rhs)
*			Copy Assignment Operator used to define how the "=" operator should function when both the lhs and rhs string objects have
*			been initialized prior to assignment.
*		Card& operator = (Card&& temp) noexcept
*			Move Assignement Operator used when the rhs is a temporary object such as a function return that will go out of scope
*			when the line finishes.
*		~Card()
*			DTor for releasing any memory allocated by Card.
*
***********************************/
class Card
{
public:
	// Manager Functions
	Card(RANK RankIn = RANK::ACE, SUIT SuitIn = SUIT::SPADES);
	Card(const Card& copy);
	Card(Card&& tmep) noexcept;
	Card& operator=(const Card& rhs);
	Card& operator=(Card&& temprhs) noexcept;
	~Card();

	// Card Functions
	int CalculateCardScore();

	// Setters and Getters
	void SetRank(RANK InRank);
	void SetSuit(SUIT InSuit);
	void SetFaceUp(bool FaceUpIn);
	void SetScore(int ScoreIn);
	const RANK GetRank() const;
	const SUIT GetSuit() const;
	const bool GetFaceUp() const;
	const int GetScore() const;

	// Utilities
	void Display() const;

private:
	// Data Members
	RANK m_rank = RANK::ACE;
	SUIT m_suit = SUIT::SPADES;
	bool m_faceUp = true;
	int m_score = 11;

};
#endif