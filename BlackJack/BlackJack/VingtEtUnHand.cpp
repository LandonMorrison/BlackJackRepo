/*************************************************
* Author:			Landon Morrison
* File Name:		VingtEtUnHand.cpp
* Date Created:		3/6/2020
* Modifications:
*
*************************************************/
#include <iostream>

#include "VingtEtUnHand.h"

using std::cout;
using std::endl;

// Default CTor
Hand::Hand() : m_handSize(0), m_cards(nullptr), m_handScore(0), isInPlay(false) // BMI
{ }

// Copy CTor
Hand::Hand(const Hand& copy) 
{
	// Make sure copy exists
	if (&copy != nullptr)
	{
		m_handSize = copy.m_handSize;
		m_handScore = copy.m_handScore;
		isInPlay = copy.isInPlay;

		// Make sure copy has cards before copying them
		if (copy.m_cards != nullptr)
		{
			m_cards = new Card[copy.m_handSize];
			for (int i = 0; i < copy.m_handSize; i++)
			{
				*(m_cards + i) = *(copy.m_cards + i);
			}
		}
	}
}

// Move CTor
Hand::Hand(Hand&& temp) noexcept 
{
	// Make sure temp exists
	if (&temp != nullptr)
	{
		// Steal!!!
		m_handSize = temp.m_handSize;
		m_handScore = temp.m_handScore;
		m_cards = temp.m_cards; // We steal this even if it is nullptr
		isInPlay = temp.isInPlay;

		// Wipe away traces
		temp.m_cards = nullptr;
		temp.m_handSize = 0;
		temp.m_handScore = 0;
		temp.isInPlay = false;
	}
}

// Copy Assignment Operator
Hand& Hand::operator=(const Hand& rhs)
{
	// Guard against self assignment
	if (this != &rhs)
	{
		// Clear previously allocated memory and reset values
		delete[] m_cards;
		m_cards = nullptr;
		m_handSize = 0;
		m_handScore = 0;
		isInPlay = false;
		
		// Make sure rhs exists
		if (&rhs != nullptr)
		{
			m_handSize = rhs.m_handSize;
			m_handScore = rhs.m_handScore;
			isInPlay = rhs.isInPlay;

			// Check if rhs has cards before we copy
			if (rhs.m_cards != nullptr)
			{
				m_cards = new Card[rhs.m_handSize];
				for (int i = 0; i < rhs.m_handSize; i++)
				{
					*(m_cards + i) = *(rhs.m_cards + i);
				}
			}
		}
	}

	return *this;
}

// Move Assignment Operator
Hand& Hand::operator=(Hand&& temprhs) noexcept
{
	// Guard against self assignment
	if (this != &temprhs)
	{
		// Clear the old stuff
		delete[] m_cards;
		m_cards = nullptr;
		m_handSize = 0;
		m_handScore = 0;
		isInPlay = false;

		// Make sure temprhs exists
		if (&temprhs != nullptr)
		{
			// Steal!!!
			m_cards = temprhs.m_cards; // We steal this even if it is nullptr
			m_handSize = temprhs.m_handSize;
			m_handScore = temprhs.m_handScore;
			isInPlay = temprhs.isInPlay;

			// Wipe away traces
			temprhs.m_cards = nullptr;
			temprhs.m_handSize = 0;
			temprhs.m_handScore = 0;
			isInPlay = false;
		}
	}

	return *this;
}

// DTor
Hand::~Hand()
{
	delete[] m_cards;
	m_cards = nullptr;
	m_handSize = 0;
	m_handScore = 0;
	isInPlay = false;
}

/******************** Hand Functions ************************/
void Hand::AddCard(Card CardIn)
{
	// Create temp hand to copy old hand and add the new card
	Card* temp = new Card[m_handSize+1];
	for (int i = 0; i < m_handSize; i++)
	{
		*(temp + i) = *(m_cards + i);
	}
	*(temp + m_handSize) = CardIn;

	// clear old hand
	delete[] m_cards;
	m_cards = nullptr;

	// Make the new hand using temp
	m_handSize++;
	m_cards = new Card[m_handSize];
	for (int i = 0; i < m_handSize; i++)
	{
		*(m_cards + i) = *(temp + i);
	}

	// Clean Up
	delete[] temp;
	temp = nullptr;

	// Update score and put into play
	m_handScore = CalculateHandScore();
	isInPlay = true;
}

Card Hand::Discard(int offset)
{
	Card CardToReturn = m_cards[offset];

	m_handSize--;
	if (m_handSize == 0)
	{
		delete[] m_cards;
		m_cards = nullptr;
		return CardToReturn;
	}

	Card* temp = new Card[m_handSize];

	for (int i = 0; i < m_handSize; i++)
	{
		*(temp + i) = *(m_cards + i);
	}

	delete[] m_cards;
	m_cards = nullptr;

	m_cards = new Card[m_handSize];
	for (int i = 0; i < m_handSize; i++)
	{
		*(m_cards + i) = *(temp + i);
	}

	delete[] temp;
	temp = nullptr;
	return CardToReturn;
}

int Hand::CalculateHandScore()
{
	int Score = 0;
	bool hasAce = false;
	for (int i = 0; i < m_handSize; i++)
	{
		if (m_cards[i].GetRank() == RANK::ACE)
		{
			hasAce = true;
		}
		Score += m_cards[i].CalculateCardScore();
	}
	if (Score > 21 && hasAce)
	{
		cout << "Ace switch to 1 to prevent bust. " << endl;
		Score -= 10;
	}

	return Score;
}

/******************* Utilities *********************/
void Hand::Display()
{
	if (m_cards != nullptr)
	{
		cout << "Score: " << m_handScore << endl;
		PrintCards();
		cout << endl;
	}
}

void Hand::FlipCard(int offset, bool FaceUp)
{
	m_cards[offset].SetFaceUp(FaceUp);
	if (FaceUp)
	{
		m_handScore = CalculateHandScore();
	}
}

/******************* Getters *********************/
const int Hand::GetHandSize() const
{
	return m_handSize;
}

const Card Hand::GetCard(int offset) const
{
	return m_cards[offset];
}

const bool Hand::GetIsInPlay() const
{
	return isInPlay;
}

void Hand::SetIsInPlay(bool PlayIn)
{
	isInPlay = PlayIn;
}

/***************** Internal Utilities *******************/
void Hand::PrintAscci(int ascii) const
{
	cout << static_cast<char>(ascii);
}

void Hand::PrintCards() const
{
	PrintTops();

	PrintTopChars();

	PrintMids();

	PrintBotChars();

	PrintBots();
}

void Hand::PrintTops() const
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// Tops
	for (int i = 0; i < m_handSize; i++)
	{
		if (m_cards[i].GetFaceUp())
		{
			// Set Colour
			if (m_cards[i].GetSuit() == SUIT::HEARTS || m_cards[i].GetSuit() == SUIT::DIAMONDS)
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			}
			else
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
			}
		}
		else
		{
			SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
		}

		// Print top left then top then top right then space for seperating cards
		PrintAscci(201);
		for (int j = 0; j < 9; j++)
		{
			PrintAscci(205);
		}
		PrintAscci(187);
		for (int j = 0; j < 2; j++)
		{
			PrintAscci(32);
		}

		// Reset Colour
		SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);

	}
	cout << endl;
}

void Hand::PrintTopChars() const
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	bool tenCard = false;
	int RankIn = 32;

	// Top characters
	for (int i = 0; i < m_handSize; i++)
	{
		if (m_cards[i].GetFaceUp())
		{
			// Set Colour
			if (m_cards[i].GetSuit() == SUIT::HEARTS || m_cards[i].GetSuit() == SUIT::DIAMONDS)
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			}
			else
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
			}
			// Print left side of card
			PrintAscci(186);
			PrintAscci(32);

			// Get the card rank
			switch (m_cards[i].GetRank()) {
			case RANK::ACE:
				RankIn = 65;
				break;
			case RANK::DEUCE:
				RankIn = 50;
				break;
			case RANK::TREY:
				RankIn = 51;
				break;
			case RANK::FOUR:
				RankIn = 52;
				break;
			case RANK::FIVE:
				RankIn = 53;
				break;
			case RANK::SIX:
				RankIn = 54;
				break;
			case RANK::SEVEN:
				RankIn = 55;
				break;
			case RANK::EIGHT:
				RankIn = 56;
				break;
			case RANK::NINE:
				RankIn = 57;
				break;
			case RANK::TEN:
				tenCard = true;
				break;
			case RANK::JACK:
				RankIn = 74;
				break;
			case RANK::QUEEN:
				RankIn = 81;
				break;
			case RANK::KING:
				RankIn = 75;
				break;
			}

			// Print the rank followed by the rest of the row for the card
			if (tenCard)
			{
				PrintAscci(49);
				PrintAscci(48);
				for (int j = 0; j < 6; j++)
				{
					PrintAscci(32);
				}
			}
			else
			{
				PrintAscci(RankIn);
				for (int j = 0; j < 7; j++)
				{
					PrintAscci(32);
				}
			}

			// Print right wall
			PrintAscci(186);

			// Make space between cards
			for (int j = 0; j < 2; j++)
			{
				PrintAscci(32);
			}

			// Reset Colour
			SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
		}
		else
		{
			// Print left side of card
			PrintAscci(186);
			for (int j = 0; j < 9; j++)
			{
				PrintAscci(176);
			}

			// Print right wall
			PrintAscci(186);

			// Make space between cards
			for (int j = 0; j < 2; j++)
			{
				PrintAscci(32);
			}
		}
	}
	cout << endl;
}

void Hand::PrintMids() const
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	bool tenCard = false;
	int SuitIn = 32;

	// Middles
	for (int i = 0; i < 5; i++)
	{
		// if we are in the middle then we want to print the suit
		if (i == 2)
		{
			for (int k = 0; k < m_handSize; k++)
			{
				if (m_cards[k].GetFaceUp()) // Face up
				{

					// Set Colour
					if (m_cards[k].GetSuit() == SUIT::HEARTS || m_cards[k].GetSuit() == SUIT::DIAMONDS)
					{
						SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
					}
					else
					{
						SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
					}

					// Left wall
					PrintAscci(186);

					// Left space
					for (int j = 0; j < 4; j++)
					{
						PrintAscci(32);
					}

					// Get and print suit
					switch (m_cards[k].GetSuit()) {
					case SUIT::CLUBS:
						SuitIn = 5;
						break;
					case SUIT::DIAMONDS:
						SuitIn = 4;
						break;
					case SUIT::HEARTS:
						SuitIn = 3;
						break;
					case SUIT::SPADES:
						SuitIn = 6;
						break;
					}
					PrintAscci(SuitIn);

					// Right space
					for (int j = 0; j < 4; j++)
					{
						PrintAscci(32);
					}

					// Right wall
					PrintAscci(186);

					// space between cards
					for (int j = 0; j < 2; j++)
					{
						PrintAscci(32);
					}

					// Reset Colour
					SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
				}
				else // Face Down
				{

					SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);

					// Print left side of card
					PrintAscci(186);

					// Print Middle
					PrintAscci(32);
					for (int j = 0; j < 7; j++)
					{
						PrintAscci(176);
					}
					PrintAscci(32);


					// Print right wall
					PrintAscci(186);

					// Make space between cards
					for (int j = 0; j < 2; j++)
					{
						PrintAscci(32);
					}
				}
			}
			cout << endl;
		}
		// Otherwise just print the left side, empty space, and then the right side
		else
		{
			for (int k = 0; k < m_handSize; k++)
			{
				if (m_cards[k].GetFaceUp()) // Face up
				{

					// Set Colour
					if (m_cards[k].GetSuit() == SUIT::HEARTS || m_cards[k].GetSuit() == SUIT::DIAMONDS)
					{
						SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
					}
					else
					{
						SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
					}

					// Left
					PrintAscci(186);

					// Middle
					for (int j = 0; j < 4; j++)
					{
						PrintAscci(32);
					}
					PrintAscci(176);
					for (int j = 0; j < 4; j++)
					{
						PrintAscci(32);
					}

					// Right
					PrintAscci(186);

					// Space between cards
					for (int j = 0; j < 2; j++)
					{
						PrintAscci(32);
					}

					// Reset Colour
					SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
				}
				else // Face Down
				{

					SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);

					// Print left side of card
					PrintAscci(186);

					// Middle
					for (int j = 0; j < 4; j++)
					{
						PrintAscci(32);
					}
					PrintAscci(177);
					for (int j = 0; j < 4; j++)
					{
						PrintAscci(32);
					}

					// Print right wall
					PrintAscci(186);

					// Make space between cards
					for (int j = 0; j < 2; j++)
					{
						PrintAscci(32);
					}
				}
			}
			cout << endl;
		}
	}
}

void Hand::PrintBotChars() const
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	bool tenCard = false;
	int RankIn = 32;

	// Bot characters
	for (int i = 0; i < m_handSize; i++)
	{
		if (m_cards[i].GetFaceUp())
		{

			// Set Colour
			if (m_cards[i].GetSuit() == SUIT::HEARTS || m_cards[i].GetSuit() == SUIT::DIAMONDS)
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			}
			else
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
			}

			// Left wall
			PrintAscci(186);

			// Get rank in case it's a ten
			switch (m_cards[i].GetRank()) {
			case RANK::ACE:
				RankIn = 65;
				break;
			case RANK::DEUCE:
				RankIn = 50;
				break;
			case RANK::TREY:
				RankIn = 51;
				break;
			case RANK::FOUR:
				RankIn = 52;
				break;
			case RANK::FIVE:
				RankIn = 53;
				break;
			case RANK::SIX:
				RankIn = 54;
				break;
			case RANK::SEVEN:
				RankIn = 55;
				break;
			case RANK::EIGHT:
				RankIn = 56;
				break;
			case RANK::NINE:
				RankIn = 57;
				break;
			case RANK::TEN:
				tenCard = true;
				break;
			case RANK::JACK:
				RankIn = 74;
				break;
			case RANK::QUEEN:
				RankIn = 81;
				break;
			case RANK::KING:
				RankIn = 75;
				break;
			}
			if (tenCard)
			{
				// 6 space followed by 10
				for (int j = 0; j < 6; j++)
				{
					PrintAscci(32);
				}
				PrintAscci(49);
				PrintAscci(48);
			}
			else
			{
				// 7 spaces follwoed by rank
				for (int j = 0; j < 7; j++)
				{
					PrintAscci(32);
				}
				PrintAscci(RankIn);
			}

			// Right space and right wall
			PrintAscci(32);
			PrintAscci(186);

			// Space between cards
			for (int j = 0; j < 2; j++)
			{
				PrintAscci(32);
			}

			// Reset Colour
			SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
		}
		else
		{

			SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);

			// Print left side of card
			PrintAscci(186);
			for (int j = 0; j < 9; j++)
			{
				PrintAscci(176);
			}

			// Print right wall
			PrintAscci(186);

			// Make space between cards
			for (int j = 0; j < 2; j++)
			{
				PrintAscci(32);
			}
		}
	}
	cout << endl;
}

void Hand::PrintBots() const
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// Bots
	for (int i = 0; i < m_handSize; i++)
	{
		if (m_cards[i].GetFaceUp())
		{
			// Set Colour
			if (m_cards[i].GetSuit() == SUIT::HEARTS || m_cards[i].GetSuit() == SUIT::DIAMONDS)
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			}
			else
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
			}
		}
		else
		{
			SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
		}

		// Left wall
		PrintAscci(200);

		// Empty space
		for (int i = 0; i < 9; i++)
		{
			PrintAscci(205);
		}

		// Right wall
		PrintAscci(188);

		// Space between cards
		for (int j = 0; j < 2; j++)
		{
			PrintAscci(32);
		}

		// Reset Colour
		SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);
	}
	cout << endl;
}