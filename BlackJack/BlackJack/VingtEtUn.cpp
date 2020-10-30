#include <iostream>
#include <Windows.h>

#include "VingtEtUn.h"

using std::cout;
using std::cin;
using std::endl;

// Default CTor
VingtEtUn::VingtEtUn() : m_players(nullptr), m_dealer(), m_deck(), m_numPlayers(0)
{ }

// DTor
VingtEtUn::~VingtEtUn()
{
	delete[] m_players;
	m_players = nullptr;
}

/******************* Game Functions **********************/
void VingtEtUn::PlayGame()
{
	// Set Colour
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, FOREGROUND_INTENSITY);

	// Welcome message and number of players check
	cout << "Welcome to VingetEtUn!" << endl << endl;

	cout << "You can play with anywhere between 1 and 5 players. " << endl;
	string choice = "1";
	bool done = false;
	while (!done)
	{
		cout << "How many players would you like? ";
		cin >> choice;
		cout << endl;
		if (choice == "1")
		{
			m_numPlayers = 1;
			done = true;
		}
		else if (choice == "2")
		{
			m_numPlayers = 2;
			done = true;
		}
		else if (choice == "3")
		{
			m_numPlayers = 3;
			done = true;
		}
		else if (choice == "4")
		{
			m_numPlayers = 4;
			done = true;
		}
		else if (choice == "5")
		{
			m_numPlayers = 5;
			done = true;
		}
		else
		{
			cout << "I'm sorry Dave, you can only play with 1-5 players. " << endl << endl;
		}
	}
	done = false;

	// Setup Players
	m_players = new Player[m_numPlayers];
	for (int i = 0; i < m_numPlayers; i++)
	{
		string tempName = "Player ";
		tempName += std::to_string(i + 1);
		m_players[i].SetName(tempName);
	}

	m_deck.ShuffleDeck();

	/********************** Play Rounds *************************/
	while (!done)
	{
		PlayRound();

		bool innerDone = false;
		string playAgain = "n";
		while (!innerDone)
		{
			cout << " would you like to play again? y/n ";
			cin >> playAgain;
			cout << endl;
			if (playAgain == "n" || playAgain == "no")
			{
				Sleep(500);
				innerDone = true;
				done = true;
			}
			else if (playAgain == "y" || playAgain == "yes")
			{
				Sleep(500);
				innerDone = true;
			}
			else
			{
				cout << "Oy, nimrod, I only know \'y\', \'n\', \"yes\", and \"no\"." << endl;
				Sleep(1000);
			}
		}
	}

	cout << "Thanks for playing! ^^ Goodbye..." << endl;
	Sleep(2000);

}

void VingtEtUn::PlayRound()
{
	cout << "Beginning Round..." << endl;
	Sleep(1000);

	// Have players bet
	for (int i = 0; i < m_numPlayers; i++)
	{
		m_players[i].Bet();
	}
	cout << "All bets locked in!" << endl;
	cout << "Dealing..." << endl << endl;
	Sleep(1500); // Artifical loading to make it feel more natural

	/******** Deal ********/
	DealRound();

	/******* Checks *******/
	InsuranceChecks();
	CheckForNaturals();
	CheckForSplits();
	CheckForDoubleDowns();

	// Turns
	PlayerTurns();
	DealerTurn();

}

void VingtEtUn::DealRound()
{
	// Deal a card to each player and display as you deal for game feel
	for (int i = 0; i < m_numPlayers; i++)
	{
		// Create the player hands
		m_players[i].AddHand();
		for (int j = 0; j < m_players[i].m_numHands; j++)
		{
			if (m_players[i].hasCurrency)
			{
				// Add first card to each hand
				m_players[i].m_hands[j].AddCard(m_deck.DealCard());
				m_players[i].Display();
				Sleep(1000);
			}
		}
	}

	// Create Dealer hand
	m_dealer.AddHand();

	// Deal a card face down to it
	m_dealer.m_hands[0].AddCard(m_deck.DealCard(false));
	cout << "Dealer's Hand: " << endl;
	m_dealer.m_hands[0].Display();
	Sleep(2000);

	// Deal the second card to each player
	for (int i = 0; i < m_numPlayers; i++)
	{
		for (int j = 0; j < m_players[i].m_numHands; j++)
		{
			if (m_players[i].hasCurrency)
			{
				// Add second card to each player hand
				m_players[i].m_hands[j].AddCard(m_deck.DealCard());
				m_players[i].Display();
				Sleep(1000);
			}
		}
	}

	// Deal a card face up for dealer
	m_dealer.m_hands[0].AddCard(m_deck.DealCard());
	cout << "Dealer's Hand: " << endl;
	m_dealer.m_hands[0].Display();
	Sleep(2000);
}

void VingtEtUn::InsuranceChecks()
{
	if (m_dealer.m_hands[0].GetCard(1).GetRank() == RANK::ACE)
	{
		cout << endl << "Dealer's Hand: " << endl;
		m_dealer.m_hands[0].Display();

		for (int i = 0; i < m_numPlayers; i++)
		{
			if (m_players[i].hasCurrency)
			{
				m_players[i].CheckForInsurance();
			}
		}

		Sleep(1000);
		cout << endl << "Dealer's Hand: " << endl;
		m_dealer.m_hands[0].Display();
		m_dealer.m_hands[0].FlipCard(0, true);
		cout << "All player Insurances locked in. Dealer checking facedown card..." << endl << endl;
		Sleep(1500);

		if (m_dealer.m_hands[0].CalculateHandScore() == 21)
		{
			cout << endl << "Dealer's Hand: " << endl;
			m_dealer.m_hands[0].Display();
			cout << "Dealer has 21! Calculating insurances... " << endl << endl;
			Sleep(1500);
			InsurancePayouts();
		}
		else
		{
			cout << "The dealer does not have 21, collecting insurance and continuing game..." << endl << endl;
			Sleep(1500);
			m_dealer.m_hands[0].FlipCard(0, false);
			for (int i = 0; i < m_numPlayers; i++)
			{
				m_players[i].ResetInsuranceBet();
			}
		}
	}
}

void VingtEtUn::CheckForNaturals()
{
	for (int i = 0; i < m_numPlayers; i++)
	{
		if (m_players[i].m_hands[0].CalculateHandScore() == 21)
		{
			m_players[i].Display();
			cout << m_players[i].GetName() << " has a natural 21! Paying out 3:2..." << endl << endl;

			// Payout with a 1.5 multiplier
			PayOut(i, 0, 1.5);
			Sleep(1000);
		}
	}
}

void VingtEtUn::CheckForSplits()
{
	for (int i = 0; i < m_numPlayers; i++)
	{
		if (m_players[i].m_hands[0].GetCard(0).GetRank() == m_players[i].m_hands[0].GetCard(1).GetRank())
		{
			string choice = "n";
			bool done = false;
			while (!done)
			{
				cout << m_players[i].GetName() << ", you have a pair. Would you like to split? (You will wager the same bet on the second hand) y/n ";
				cin >> choice;
				cout << endl;

				if (choice == "y" || choice == "yes")
				{
					if (m_players[i].m_purse - m_players[i].AmountBet < 0)
					{
						cout << "Just kidding. You don't have enough currency for this. " << endl << endl;
						done = true;
					}
					else
					{
						// Bet on split
						m_players[i].SplitBet = m_players[i].AmountBet;
						m_players[i].m_purse -= m_players[i].SplitBet;

						// Splitting
						m_players[i].AddHand();
						m_players[i].Split(m_deck.DealCard(), m_deck.DealCard());

						cout << m_players[i].GetName() << " now has a split hand. " << endl << endl;
						m_players[i].Display();
						done = true;
					}
				}
				else if (choice == "n" || choice == "no")
				{
					cout << m_players[i].GetName() << " Has chosen not to split. Proceeding..." << endl << endl;
					Sleep(1000);
					done = true;
				}
				else
				{
					cout << "Please indicate your choice using y or n." << endl << endl;
				}
			}
		}
	}
}

void VingtEtUn::CheckForDoubleDowns()
{
	for (int i = 0; i < m_numPlayers; i++)
	{
		for (int j = 0; j < m_players[i].m_numHands; j++)
		{
			string temp = "";
			string choice = "n";

			// If the player has a split hand, differentiate them
			if (m_players[i].m_numHands > 1 && j == 0)
			{
				temp = " on your Main Hand";
				cout << m_players[i].GetName() << ", here are your hands: " << endl;
			}
			else if (j == 1)
			{
				cout << m_players[i].GetName() << ", here are your hands: " << endl;
				temp = " on your Split Hand";
			}

			// Otherwise, player has one hand
			else
			{
				cout << m_players[i].GetName() << ", here is your " << temp << "Hand: " << endl;
			}

			m_players[i].Display();

			bool done = false;
			while (!done)
			{
				cout << "Would you like to double down" << temp << "? y/n ";
				cin >> choice;
				cout << endl;
				if (choice == "y" || choice == "yes")
				{
					cout << "ALRIGHT! DOUBLE DOWN!" << endl;

					// Doubling split bet
					if (j == 1)
					{
						if (m_players[i].m_purse - m_players[i].SplitBet < 0)
						{
							cout << "Just kidding, you're broke! Proceeding..." << endl << endl;
							Sleep(1000);
							done = true;
						}
						else
						{
							m_players[i].m_purse -= m_players[i].SplitBet;
							m_players[i].SplitBet += m_players[i].SplitBet;
							cout << "You split bet is now: " << m_players[i].SplitBet << endl << endl;
						}
					}

					// Doubling main bet
					else
					{
						if (m_players[i].m_purse - m_players[i].AmountBet < 0)
						{
							cout << "Just kidding, you're broke! Proceeding..." << endl << endl;
							Sleep(1000);
							done = true;
						}
						else
						{
							m_players[i].m_purse -= m_players[i].AmountBet;
							m_players[i].AmountBet += m_players[i].AmountBet;
							cout << "You bet is now: " << m_players[i].AmountBet << endl << endl;
						}
					}
					Sleep(1000);
					done = true;
				}
				else if (choice == "n" || choice == "no")
				{
					cout << m_players[i].GetName() << " has chosen not to double down. Proceeding..." << endl << endl;
					Sleep(1000);
					done = true;
				}
				else
				{
					cout << "Pleas indicate a choice with either y or n." << endl << endl;
				}
			}
		}
	}
}

void VingtEtUn::PlayerTurns()
{
	for (int i = 0; i < m_numPlayers; i++)
	{
		cout << m_players[i].GetName() << ", it is your turn. " << endl << endl;

		// Displayer the dealer for more easily informed player decisions
		cout << endl << "Dealer's Hand: " << endl;
		m_dealer.m_hands[0].Display();

		// Set needed variables
		string choice = "";
		bool done = false;

		// Looping through all hands, currently only works for 2 hands, a main and a split
		for (int j = 0; j < m_players[i].m_numHands; j++)
		{
			// Loop until the player makes a decision
			while (!done)
			{
				if (j == 0)
				{
					cout << "Hand:              bet: " << m_players[i].GetAmountBet() << endl;
					m_players[i].m_hands[j].Display();
				}
				else if (j == 1)
				{
					cout << "Split hand:              bet: " << m_players[i].GetSplitBet() << endl;
					m_players[i].m_hands[j].Display();
				}
				cout << "Will you hit or stand? ";
				cin >> choice;
				cout << endl;
				if (choice == "hit")
				{
					// Add card then check for bust
					m_players[i].m_hands[j].AddCard(m_deck.DealCard());
					if (m_players[i].m_hands[j].CalculateHandScore() > 21)
					{
						m_players[i].m_hands[j].Display();
						cout << "Bust! Taking bet..." << endl << endl;
						m_players[i].m_hands[j].SetIsInPlay(false);
						Sleep(1000);
						if (j == 0)
						{
							m_players[i].ResetAmountBet();
						}
						if (j == 1)
						{
							m_players[i].ResetSplitBet();
						}
						done = true;
					}

					// Check for a 21
					else if (m_players[i].m_hands[j].CalculateHandScore() == 21)
					{
						m_players[i].m_hands[j].Display();
						cout << "Blackjack! uhh... I mean... VingtEtUn!" << endl << endl;
						PayOut(i, j, 1); // Payout at 1:1
						m_players[i].m_hands[j].SetIsInPlay(false);
						Sleep(2000);
						done = true;
					}

					// Otherwise loop back so player can choose again
				}
				else if (choice == "stand")
				{
					cout << m_players[i].GetName() << " has chosen to stand. Proceeding..." << endl << endl;
					Sleep(1000);
					done = true;
				}
				else
				{
					cout << "Please indicate your choice by entering either \"hit\" or \"stand\"." << endl << endl;
				}
			}
		}

	}
}

void VingtEtUn::DealerTurn()
{
	int playersIn = 0;
	for (int i = 0; i < m_numPlayers; i++)
	{
		if (m_players[i].m_hands[0].GetIsInPlay())
		{
			playersIn++;
		}
		if (m_players[i].m_numHands > 1)
		{
			if (m_players[i].m_hands[0].GetIsInPlay())
			{
				playersIn++;
			}
		}
	}
	if (playersIn > 0)
	{
		cout << "Player turns complete. Dealer playing..." << endl;
		Sleep(1000);
		m_dealer.m_hands[0].FlipCard(0, true);
		cout << endl << "Dealer's Hand: " << endl;
		m_dealer.m_hands[0].Display();
		Sleep(2000);

		bool done = false;
		while (!done)
		{
			if (m_dealer.m_hands[0].CalculateHandScore() < 17)
			{
				m_dealer.m_hands[0].AddCard(m_deck.DealCard());
				cout << endl << "Dealer's Hand: " << endl;
				m_dealer.m_hands[0].Display();
				Sleep(2000);
			}
			else if (m_dealer.m_hands[0].CalculateHandScore() > 21)
			{
				cout << "Dealer busts! Paying out all standing bets..." << endl << endl;;
				Sleep(2000);

				// Loop all players
				for (int i = 0; i < m_numPlayers; i++)
				{
					// Loop all hands
					for (int j = 0; j < m_players[i].m_numHands; j++)
					{
						// Only payout to hands still in play
						if (m_players[i].m_hands[j].GetIsInPlay())
						{
							PayOut(i, j, 1); // Payout 1:1
						}
					}
				}
				done = true;
			}
			else if (m_dealer.m_hands[0].CalculateHandScore() == 21)
			{
				cout << "Dealer 21! Suckers! Calculating how much I won..." << endl;
				Sleep(2000);
			}
			else
			{
				cout << "Dealer stands" << endl;
				cout << endl << "Dealer's Hand: " << endl;
				m_dealer.m_hands[0].Display();
				Sleep(2000);
				done = true;
			}
		}

		// Loop all players
		for (int i = 0; i < m_numPlayers; i++)
		{
			// Loop all hands
			for (int j = 0; j < m_players[i].m_numHands; j++)
			{
				// Only payout to hands still in play
				if (m_players[i].m_hands[j].GetIsInPlay())
				{
					if (j == 0)
					{
						cout << "Comparing " << m_players[i].GetName() << "'s Hand to the Dealer's..." << endl;
					}
					else if(j==1)
					{ 
						cout << "Comparing " << m_players[i].GetName() << "'s Split Hand to the Dealer's..." << endl;
					}
					Sleep(1000);
					m_players[i].m_hands[j].Display();
					Sleep(500);
					m_dealer.m_hands[0].Display();
					Sleep(2000);

					// Payout winning hands
					if (m_players[i].m_hands[j].CalculateHandScore() > m_dealer.m_hands[0].CalculateHandScore())
					{
						PayOut(i, j, 1); // Payout 1:1
					}
					else if (m_players[i].m_hands[j].CalculateHandScore() == m_dealer.m_hands[0].CalculateHandScore())
					{
						PayOut(i, j, 0); // If it's a tie, player gets their bet back but no more
					}
					else
					{
						cout << "Dealer wins! Collecting bet..." << endl;
					}
					Sleep(1000);
				}
			}
		}

	}
	else
	{
		cout << "No players left standing. Round over..." << endl;
		Sleep(1000);
	}
	for (int i = 0; i < m_numPlayers; i++)
	{
		for (int j = 0; j < m_players[i].m_numHands; j++)
		{
			for (int k = 0; k < m_players[i].m_hands[j].GetHandSize(); k++)
			{
				m_deck.Discard(m_players[i].m_hands[j].Discard(k));
			}
		}
	}
	for (int i = 0; i < m_numPlayers; i++)
	{
		delete[] m_players[i].m_hands;
		m_players[i].m_hands = nullptr;
		m_players[i].m_numHands = 0;
	}
	delete[] m_dealer.m_hands;
	m_dealer.m_hands = nullptr;
	m_dealer.m_numHands = 0;
}

/********************** Utilities ************************/
void VingtEtUn::InsurancePayouts()
{
	int amount = 0;
	for (int i = 0; i < m_numPlayers; i++)
	{
		// Payout Insurance
		amount = m_players[i].InsuranceBet * 2;
		m_players[i].AddToPurse(amount);

		for (int i = 0; i < m_numPlayers; i++)
		{
			// Clear insurance wagers
			m_players[i].ResetInsuranceBet();
		}
	}
}

void VingtEtUn::PayOut(int playerOffset, int handOffset, float multiplier)
{

	if (handOffset == 0)
	{
		if (multiplier == 0)
		{
			cout << m_players[playerOffset].GetName() << " Ties! Returning bet..." << endl << endl;
		}
		else
		{
			cout << m_players[playerOffset].GetName() << " Wins! Paying out 1:1..." << endl << endl;
		}

		// Setup amount to pay out
		m_players[playerOffset].AmountBet += m_players[playerOffset].GetAmountBet() * multiplier; // drops remainder

		// Payout
		m_players[playerOffset].AddToPurse(m_players[playerOffset].GetAmountBet());

		// Clear Wager
		m_players[playerOffset].ResetAmountBet();

		Sleep(1000);
	}
	else
	{
		if (multiplier == 0)
		{
			cout << m_players[playerOffset].GetName() << " Ties! Returning bet..." << endl << endl;
		}
		else
		{
			cout << m_players[playerOffset].GetName() << "'s Split Hand Wins! Paying out 1:1..." << endl << endl;
		}

		// Setup amount to pay out
		m_players[playerOffset].AmountBet += m_players[playerOffset].GetSplitBet() * multiplier; // drops remainder

		// Payout
		m_players[playerOffset].AddToPurse(m_players[playerOffset].GetSplitBet());

		// Clear Wager
		m_players[playerOffset].ResetSplitBet();

		Sleep(1000);
	}

	m_players[playerOffset].m_hands[handOffset].SetIsInPlay(false);
}
