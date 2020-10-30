/*************************************************
* Author:			Landon Morrison
* File Name:		VingtEtUnPlayer.cpp
* Date Created:		3/6/2020
* Modifications:	
*
*************************************************/
#include <iostream>

#include "VingtEtUnPlayer.h"

using std::cout;
using std::cin;
using std::endl;

// Default CTor
Player::Player() : m_name(""), m_purse(500), m_hands(nullptr),
	InsuranceBet(0), AmountBet(0), SplitBet(0), m_numHands(0), 
	PlayAgain(true), hasCurrency(true) // BMI
{ }

Player::Player(const Player & copy)
{
	if (&copy != nullptr)
	{
		m_name = copy.m_name;
		m_purse = copy.m_purse;
		InsuranceBet = copy.InsuranceBet;
		AmountBet = copy.AmountBet;
		SplitBet = copy.SplitBet;
		m_numHands = copy.m_numHands;
		PlayAgain = copy.PlayAgain;
		hasCurrency = copy.hasCurrency;

		m_hands = new Hand[copy.m_numHands];
		for (int i = 0; i < copy.m_numHands; i++)
		{
			*(m_hands + i) = *(copy.m_hands + i);
		}
	}
}

Player::Player(Player&& temp) noexcept
{
	if (&temp != nullptr)
	{
		// Steal!!!
		m_name = temp.m_name;
		m_purse = temp.m_purse;
		InsuranceBet = temp.InsuranceBet;
		AmountBet = temp.AmountBet;
		SplitBet = temp.SplitBet;
		m_numHands = temp.m_numHands;
		PlayAgain = temp.PlayAgain;
		hasCurrency = temp.hasCurrency;
		m_hands = temp.m_hands;

		// Wipe away traces
		temp.m_name = "";
		temp.m_purse = 500;
		temp.InsuranceBet = 0;
		temp.AmountBet = 0;
		temp.SplitBet = 0;
		temp.m_numHands = 0;
		temp.PlayAgain = true;
		temp.hasCurrency = true;
		temp.m_hands = nullptr;
	}
}

Player& Player::operator=(const Player& rhs)
{
	if (this != &rhs)
	{
		if (&rhs != nullptr)
		{
			m_name = rhs.m_name;
			m_purse = rhs.m_purse;
			InsuranceBet = rhs.InsuranceBet;
			AmountBet = rhs.AmountBet;
			SplitBet = rhs.SplitBet;
			m_numHands = rhs.m_numHands;
			PlayAgain = rhs.PlayAgain;
			hasCurrency = rhs.hasCurrency;

			m_hands = new Hand[rhs.m_numHands];
			for (int i = 0; i < rhs.m_numHands; i++)
			{
				*(m_hands + i) = *(rhs.m_hands + i);
			}
		}
	}

	return *this;
}

Player& Player::operator=(Player&& temprhs) noexcept
{
	if (this != &temprhs)
	{
		if (&temprhs != nullptr)
		{
			// Steal!!!
			m_name = temprhs.m_name;
			m_purse = temprhs.m_purse;
			InsuranceBet = temprhs.InsuranceBet;
			AmountBet = temprhs.AmountBet;
			SplitBet = temprhs.SplitBet;
			m_numHands = temprhs.m_numHands;
			PlayAgain = temprhs.PlayAgain;
			hasCurrency = temprhs.hasCurrency;
			m_hands = temprhs.m_hands;

			// Wipe away traces
			temprhs.m_name = "";
			temprhs.m_purse = 500;
			temprhs.InsuranceBet = 0;
			temprhs.AmountBet = 0;
			temprhs.SplitBet = 0;
			temprhs.m_numHands = 0;
			temprhs.PlayAgain = true;
			temprhs.hasCurrency = true;
			temprhs.m_hands = nullptr;
		}
	}
	return *this;
}

// DTor
Player::~Player()
{
	m_name = "";
	m_purse = 0;
	delete[] m_hands;
	m_hands = nullptr;
	m_numHands = 0;
	InsuranceBet = 0;
	AmountBet = 0;
	PlayAgain = true;
}

/***************************** Player Functions ********************************/
void Player::Bet()
{
	bool done = false;
	while (!done)
	{
		cout << m_name << endl;
		cout << "You have: " << m_purse << "currency to use." << endl;
		if (m_purse < 5)
		{
			cout << "Sorry, this isn't enough to continue play." << endl;
			if (BuyIn())
			{
				AddToPurse(500);
				hasCurrency = true;
			}
			else
			{
				hasCurrency = false;
			}
			done = true;
		}
		if(hasCurrency)
		{
			cout << "How much would you like to bet? ";
			cin >> AmountBet;
			cout << endl;
			if (AmountBet < 5 || AmountBet>500)
			{
				cout << "You can only bet an amount between 5 and 500." << endl << endl;
			}
			else if (AmountBet > m_purse)
			{
				cout << "This is more currency than you have. " << endl << endl;
			}
			else
			{
				cout << m_name << " bet: " << AmountBet;
				m_purse -= AmountBet;
				cout << " and now has: " << m_purse << "currency remaining" << endl << endl;

				Sleep(1000);
				done = true;
			}
		}
	}
}

bool Player::BuyIn()
{
	char choice = 'n';
	bool done = false;
	bool returnChoice = false;
	while (!done)
	{
		cout << "Would you like to buy into the game? (costs 500currency) y/n ";
		cin >> choice;
		cout << endl;
		if (choice == 'y')
		{
			cout << "You have payed 500currency and are now in the game." << endl;
			returnChoice = true;
			done = true;
		}
		else if (choice == 'n')
		{
			cout << "You have chosen not to buy into the game. Goodbye." << endl;
			returnChoice = false;
			done = true;
		}
		else
		{
			cout << "Please enter a choice of either y or n" << endl << endl;
		}
	}
	return returnChoice;
}

// This function currently has no safety checks and is only set up for 2 hands so be careful with it
void Player::Split(Card CardForHand, Card CardForSplitHand) 
{
	// Move a card from hand 0 to hand 1
	m_hands[1].AddCard(m_hands[0].Discard(1));
	// Add a card to the main hand
	m_hands[0].AddCard(CardForHand);
	// Add a card to the split hand
	m_hands[1].AddCard(CardForSplitHand);
}

void Player::AddToPurse(int AmountToAdd)
{
	m_purse += AmountToAdd;
	cout << m_name << " now has: " << m_purse << "currency" << endl;
}

void Player::AddHand()
{
	// Temp set of hands to store old hands + 1
	Hand* temp = new Hand[m_numHands+1];
	for (int i = 0; i < m_numHands; i++)
	{
		*(temp + i) = *(m_hands + i);
	}

	// Clear old hands
	delete[] m_hands;
	m_hands = nullptr;

	// Make new hands
	m_numHands++;
	m_hands = new Hand[m_numHands];
	for (int i = 0; i < m_numHands; i++)
	{
		*(m_hands + i) = *(temp + i);
	}

	// Clean up
	delete[] temp;
	temp = nullptr;
}

void Player::RemoveHand(int handOffset)
{
	if (m_numHands > 0)
	{
		if (m_numHands == 1)
		{
			delete[] m_hands;
			m_hands = nullptr;
		}
		if (m_numHands > 1)
		{
			// Temp set of hands to store old hands - 1
			Hand* temp = new Hand[m_numHands - 1];
			for (int i = 0; i < m_numHands; i++)
			{
				if (i != handOffset)
				{
					*(temp + i) = *(m_hands + i);
				}
			}

			// Clear old hands
			delete[] m_hands;
			m_hands = nullptr;

			// Make new hands
			m_numHands--;
			m_hands = new Hand[m_numHands];
			for (int i = 0; i < m_numHands; i++)
			{
				*(m_hands + i) = *(temp + i);
			}

			// Clean up
			delete[] temp;
			temp = nullptr;
		}
	}
}

/********************* Utilities *************************/
void Player::Display() const
{
	if (m_hands != nullptr)
	{
		for (int i = 0; i < m_numHands; i++)
		{
			string handNum = "";
			int amount = AmountBet;
			if (i == 1)
			{
				handNum = std::to_string(i+1) + " ";
				amount = SplitBet;
			}
			
			cout << m_name << "'s " << handNum << "Hand:          Bet: " << amount << endl;

			m_hands[i].Display();

			cout << endl;
		}
	}
}

void Player::CheckForInsurance()
{
	char choice = 'n';
	bool done = false;
	while (!done)
	{
		cout << "The Dealer's faceup card is an ACE, would " << m_name << " like to make an insurance bet? y/n ";
		cin >> choice;
		cout << endl;
		if (choice == 'y')
		{
			bool innerDone = false;
			while (!innerDone)
			{
				cout << m_name << " has chosen to make an insurance bet, you may bet up to half of your original bet which was: " << AmountBet << endl;
				cout << "How much would you like to bet? ";
				cin >> InsuranceBet;
				cout << endl;
				if (InsuranceBet > (AmountBet / 2))
				{
					cout << "Your insurance bet must be half or less of your original bet. " << endl;
				}
				else if (InsuranceBet < 1)
				{
					cout << "You must bet at least 1currency. " << endl;
				}
				else
				{
					cout << "You have made an insurance bet of " << InsuranceBet << "currency. " << endl;
					innerDone = true;
					done = true;
				}
			}
		}
		else if (choice == 'n')
		{
			cout << "You have chosen not to make an insurance bet. " << endl;
			done = true;
		}
		else
		{
			cout << "Please indicate a choice using either y or n." << endl << endl;
		}
	}
}

void Player::ResetAmountBet()
{
	AmountBet = 0;
}

void Player::ResetSplitBet()
{
	SplitBet = 0;
}

void Player::ResetInsuranceBet()
{
	InsuranceBet = 0;
}

/******************* Getters and Setters ********************/
const string Player::GetName() const
{
	return m_name;
}
const int Player::GetPurse() const
{
	return m_purse;
}
const bool Player::GetNumHands() const
{
	return m_numHands;
}
const int Player::GetAmountBet() const
{
	return AmountBet;
}
const int Player::GetSplitBet() const
{
	return SplitBet;
}
const int Player::GetInsuranceBet() const
{
	return InsuranceBet;
}
void Player::SetName(string NameIn)
{
	m_name = NameIn;
}