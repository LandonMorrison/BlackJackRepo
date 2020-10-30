/*************************************************
* Author:			Landon Morrison
* File Name:		VingtEtUnDeck.cpp
* Date Created:		3/6/2020
* Modifications:	3/8/2020 - changed from Deck to VingtEtUnDeck
*
*************************************************/

#include "VingtEtUnDeck.h"

using std::cout;
using std::endl;

// Default CTor
Deck::Deck() : m_deckSize(52), m_cards(new Card[52]), m_topCardIndex(0), m_discardsSize(0), m_discards(nullptr) // BMI
{
	// We have 4 suits so we will loop once for all cards in each suit
	for (int i = 0; i < 4; i++)
	{
		SUIT SuitIn = static_cast <SUIT>(i);

		// We have 13 ranks so we will loop through 13 times starting at 1 because 0 is an invalid rank
		for (int j = 1; j <= 13; j++)
		{
			RANK RankIn = static_cast <RANK>(j);

			*(m_cards + m_topCardIndex) = Card(RankIn, SuitIn);

			// Cards default to face up so we have to flip them when building the deck
			(m_cards + m_topCardIndex)->SetFaceUp(false);
			m_topCardIndex++;
		}
	}
	m_topCardIndex--;
}

// Copy CTor
Deck::Deck(const Deck& copy)
{
	// Make sure copy exists
	if (&copy != nullptr)
	{
		m_deckSize = copy.m_deckSize;
		m_discardsSize = copy.m_discardsSize;
		m_topCardIndex = copy.m_topCardIndex;

		// Check if copy has cards before copying them
		if (copy.m_cards != nullptr)
		{
			m_cards = new Card[copy.m_deckSize];
			for (int i = 0; i < copy.m_deckSize; i++)
			{
				*(m_cards + i) = *(copy.m_cards + i);
			}
		}

		// Check if copy has discards before copying them
		if (copy.m_discards != nullptr)
		{
			for (int i = 0; i < copy.m_discardsSize; i++)
			{
				*(m_discards + i) = *(copy.m_discards + i);
			}
		}
	}
}

// Move CTor
Deck::Deck(Deck&& temp) noexcept
{
	// Make sure temp exists
	if (&temp != nullptr)
	{
		// Steal!!!
		m_deckSize = temp.m_deckSize;
		m_topCardIndex = temp.m_topCardIndex;
		m_cards = temp.m_cards; // We steal this even if it is nullptr
		m_discardsSize = temp.m_discardsSize;
		m_discards = temp.m_discards; // We steal this even if it is nullptr

		// Wipe away traces
		temp.m_deckSize = 0;
		temp.m_cards = nullptr;
		temp.m_topCardIndex = 0;
		temp.m_discardsSize = 0;
		temp.m_discards = nullptr;
	}
}

// Copy Assignment Operator
Deck& Deck::operator=(const Deck& rhs)
{
	if (this != &rhs)
	{
		// Clear previously allocated memory and reset values
		delete[] m_cards;
		m_cards = nullptr;
		m_deckSize = 0;
		m_topCardIndex = 0;
		delete[] m_discards;
		m_discards = nullptr;
		m_discardsSize = 0;

		// Only proceed if there is actually an rhs
		if (&rhs != nullptr)
		{
			m_deckSize = rhs.m_deckSize;
			m_topCardIndex = rhs.m_topCardIndex;
			m_discardsSize = rhs.m_discardsSize;

			// Only create a new deck if the rhs has cards
			if (rhs.m_cards != nullptr)
			{
				// new memory for the new deck
				m_cards = new Card[rhs.m_deckSize];
				for (int i = 0; i < rhs.m_deckSize; i++)
				{
					*(m_cards + i) = *(rhs.m_cards + i);
				}				
			}

			// Only create new memory for the new discards if the rhs has discards
			if (rhs.m_discards != nullptr)
			{
				m_discards = new Card[rhs.m_discardsSize];
				for (int i = 0; i < rhs.m_discardsSize; i++)
				{
					*(m_discards + i) = *(rhs.m_discards + i);
				}
			}
		}
	}

	return *this;
}

// Move Assignment Operator
Deck& Deck::operator=(Deck&& temprhs) noexcept
{
	if (this != &temprhs)
	{
		// Clear the old stuff
		delete[] m_cards;
		m_cards = nullptr;
		m_deckSize = 0;
		m_topCardIndex = 0;
		delete[] m_discards;
		m_discards = nullptr;
		m_discardsSize = 0;

		// Steal!!!
		m_deckSize = temprhs.m_deckSize;
		m_cards = temprhs.m_cards; // We steal this even if it is nullptr
		m_topCardIndex = temprhs.m_topCardIndex;
		m_discards = temprhs.m_discards; // We steal this even if it is nullptr
		m_discardsSize = temprhs.m_discardsSize;

		// Wpe away traces
		temprhs.m_deckSize = 0;
		temprhs.m_cards = nullptr;
		temprhs.m_topCardIndex = 0;
		temprhs.m_discardsSize = 0;
		temprhs.m_discards = nullptr;
	}

	return *this;
}

// DTor
Deck::~Deck()
{
	delete[] m_cards;
	m_cards = nullptr;
	m_deckSize = 0;
	m_topCardIndex = 0;
	delete[] m_discards;
	m_discards = nullptr;
	m_discardsSize = 0;
}

/************** Deck Functions ***************/
void Deck::RecombineDeck()
{
	// Create a temp deck to store all of the cards from the deck and the discard pile
	Card* temp = new Card[m_deckSize + m_discardsSize];
	int tempSize = m_deckSize + m_discardsSize;
	for (int i = 0; i < m_deckSize; i++)
	{
		*(temp + i) = *(m_cards + i);
	}
	if (m_discards != nullptr)
	{
		for (int i = 0; i < m_discardsSize; i++)
		{
			*(temp + i + m_deckSize) = *(m_discards + i);
		}
	}

	// Clean the old deck
	delete[] m_cards;
	m_cards = nullptr;
	m_deckSize = 0;
	m_topCardIndex = -1;

	// Clean the discards
	delete[] m_discards;
	m_discards = nullptr;
	m_discardsSize = 0;

	// Recreate the deck
	m_cards = new Card[tempSize];
	for (int i = 0; i < tempSize; i++)
	{
		*(m_cards + i) = *(temp + i);
		m_deckSize++;
		m_topCardIndex++;
	}

	// Clean up
	delete[] temp;
}

void Deck::ShuffleDeck()
{
	RecombineDeck();

	// Set the seed value for rand based on the time at which the program is run
	srand(static_cast<unsigned int>(time(NULL)));

	// Start from the last element and run one by one in descending order
	for (int i = m_topCardIndex; i > 0; i--)
	{
		// Pick a random index from 0 to i
		int j = rand() % (i + 1);

		// Swap the card at the current index with the card at the randomly chosen index j
		Swap(&m_cards[i], &m_cards[j]);
	}
}

Card Deck::DealCard(bool FaceUpIn)
{
	// Check if we are out of cards and shuffle if so
	if (m_topCardIndex == -1)
	{
		cout << "The Deck is out of cards. Shuffling..." << endl;
		ShuffleDeck();
		Sleep(1000); // Simulate a small shuffling delay
	}

	// Take out the card to be dealt
	Card CardToBeDealt = m_cards[m_topCardIndex];
	CardToBeDealt.SetFaceUp(FaceUpIn);
	m_topCardIndex--;
	m_deckSize--;

	// Make a temp deck that is one smaller than the original deck
	Card* temp = new Card[m_deckSize];
	for (int i = 0; i < m_deckSize; i++)
	{
		*(temp + i) = *(m_cards + i);
	}

	// Clear the original deck
	delete[] m_cards;
	m_cards = nullptr;

	// Remake the deck with the new size
	m_cards = new Card[m_deckSize];
	for (int i = 0; i < m_deckSize; i++)
	{
		*(m_cards + i) = *(temp + i);
	}

	// Clean up
	delete[] temp;
	temp = nullptr;

	return CardToBeDealt;
}

void Deck::Discard(Card CardToDiscard)
{
	// Create a temp set of cards to hold the old cards plus the new one
	Card* temp = new Card[m_discardsSize + 1];
	for (int i = 0; i < m_discardsSize; i++)
	{
		*(temp + i) = *(m_discards+i);
	}
	*(temp + m_discardsSize) = CardToDiscard;

	// Clear the original set of discards
	delete[] m_discards;
	m_discards = nullptr;

	// Recreate the original set of discards with the new one added
	m_discardsSize++;
	m_discards = new Card[m_discardsSize];
	for (int i = 0; i < m_discardsSize; i++)
	{
		*(m_discards + i) = *(temp + i);
	}

	// Clean up
	delete[] temp;
	temp = nullptr;
}

/*************** Getters *****************/
const int Deck::GetTopCardIndex() const
{
	return m_topCardIndex;
}

const int Deck::GetDeckSize() const
{
	return m_deckSize;
}

const int Deck::GetDiscardsSize() const
{
	return m_discardsSize;
}

/************** Utilities ***************/
void Deck::Display() const
{
	cout << "Displaying cards in deck: " << endl;
	for (int i = 0; i < m_deckSize; i++)
	{
		m_cards[i].SetFaceUp(true);
		m_cards[i].Display();
	}

	cout << "Discards: " << endl;
	for (int i = 0; i < m_discardsSize; i++)
	{
		m_discards[i].SetFaceUp(true);
		m_discards[i].Display();
	}
}

void Deck::Swap(Card* a, Card* b)
{
	Card tempCard = *a;
	*a = *b;
	*b = tempCard;
}