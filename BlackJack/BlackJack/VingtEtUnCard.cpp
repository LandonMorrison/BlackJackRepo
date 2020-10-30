/*************************************************
* Author:			Landon Morrison
* File Name:		VingtEtUnCard.cpp
* Date Created:		3/6/2020
* Modifications:	3/8/2020 - changed from Card to VingtEtUnCard
*
*************************************************/
#include <iostream>
#include <Windows.h>

#include "VingtEtUnCard.h"

using std::cout;
using std::endl;

// Default CTor replaced with 2 arg CTor that has default values for ACE of SPADES when no args are passed
Card::Card(RANK RankIn, SUIT SuitIn) : m_rank(RankIn), m_suit(SuitIn), m_faceUp(true), m_score(CalculateCardScore()) // BMI
{ }

// Copy CTor
Card::Card(const Card& copy)
{
	// Make sure copy exists
	if (&copy != nullptr)
	{
		m_rank = copy.m_rank;
		m_suit = copy.m_suit;
		m_faceUp = copy.m_faceUp;
		m_score = copy.m_score;
	}
}

// Move CTor
Card::Card(Card&& temp) noexcept
{ 
	// Make sure temp exists
	if (&temp != nullptr)
	{
		// Steal!!!
		m_rank = temp.m_rank;
		m_suit = temp.m_suit;
		m_faceUp = temp.m_faceUp;
		m_score = temp.m_score;

		// Wipe away traces
		temp.m_rank = RANK::ACE;
		temp.m_suit = SUIT::SPADES;
		temp.m_faceUp = true;
		temp.m_score = 11;
	}
}

// Copy Assignment Operator
Card& Card::operator=(const Card& rhs)
{
	// Guard agaisnt self assignment
	if (this != &rhs)
	{
		// no need to clear lhs because no pointer data members

		if (&rhs != nullptr)
		{
			m_rank = rhs.m_rank;
			m_suit = rhs.m_suit;
			m_faceUp = rhs.m_faceUp;
			m_score = rhs.m_score;
		}
	}

	return *this;
}

// Move Assignment Operator
Card& Card::operator=(Card&& temprhs) noexcept
{
	// Guard agaisnt self assignment
	if(this != &temprhs)
	{
		// no need to clear lhs because no pointer data members

		if (&temprhs != nullptr)
		{
			// Steal!!!
			m_rank = temprhs.m_rank;
			m_suit = temprhs.m_suit;
			m_faceUp = temprhs.m_faceUp;
			m_score = temprhs.m_score;

			// Wipe away traces
			temprhs.m_rank = RANK::ACE;
			temprhs.m_suit = SUIT::SPADES;
			temprhs.m_faceUp = true;
			temprhs.m_score = 11;
		}
	}

	return *this;
}

// DTor
Card::~Card()
{
	// Nothing to delete so we just reset values to default before destruction for posterity
	m_rank = RANK::ACE;
	m_suit = SUIT::SPADES;
	m_faceUp = true;
	m_score = 11;
}

/******* Card Functions *******/
int Card::CalculateCardScore()
{
	int Score = 0;
	if (m_faceUp)
	{
		switch (m_rank) {
		case RANK::ACE:
			Score = 11;
			break;
		case RANK::DEUCE:
			Score = 2;
			break;
		case RANK::TREY:
			Score = 3;
			break;
		case RANK::FOUR:
			Score = 4;
			break;
		case RANK::FIVE:
			Score = 5;
			break;
		case RANK::SIX:
			Score = 6;
			break;
		case RANK::SEVEN:
			Score = 7;
			break;
		case RANK::EIGHT:
			Score = 8;
			break;
		case RANK::NINE:
			Score = 9;
			break;
		case RANK::TEN:
			Score = 10;
			break;
		case RANK::JACK:
			Score = 10;
			break;
		case RANK::QUEEN:
			Score = 10;
			break;
		case RANK::KING:
			Score = 10;
			break;
		}
	}
	else
	{
		Score = 0;
	}
	return Score;
}


/************** Setters and Getters *******************/
void Card::SetRank(RANK RankIn)
{
	m_rank = RankIn;
	CalculateCardScore();
}
void Card::SetSuit(SUIT SuitIn)
{
	m_suit = SuitIn;
}
void Card::SetFaceUp(bool FaceUpIn)
{
	m_faceUp = FaceUpIn;
}
void Card::SetScore(int ScoreIn)
{
	m_score = ScoreIn;
}
const RANK Card::GetRank() const
{
	return m_rank;
}
const SUIT Card::GetSuit() const
{
	return m_suit;
}
const bool Card::GetFaceUp() const
{
	return m_faceUp;
}
const int Card::GetScore() const
{
	return m_score;
}

/******************** Utilities ********************/
void Card::Display() const
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (m_faceUp == false)
	{
		cout << "?" << endl;
	}
	else
	{
		// Convert from enums to display types
		const char* RankDisplay[] =
		{
			"Invalid Entry",
			"ACE",
			"DEUCE",
			"TREY",
			"FOUR",
			"FIVE",
			"SIX",
			"SEVEN",
			"EIGHT",
			"NINE",
			"TEN",
			"JACK",
			"QUEEN",
			"KING"
		};
		const char SuitDisplay[] =
		{
			6,
			3,
			5,
			4
		};

		if (m_suit == SUIT::HEARTS || m_suit == SUIT::DIAMONDS)
		{
			SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
		}
		
		cout << RankDisplay[static_cast<int>(GetRank())] << " " << SuitDisplay[static_cast<int>(GetSuit())] << " Score: " << m_score << endl;

		SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
	}
}