#include "stdafx.h"
#include "cards.h"

CCard::CCard()
{
	m_dwCard = 0;		// Zero is an invalid card
}

CCard::CCard(DWORD dwCard)
{
	m_dwCard = dwCard;	// Set the card to what we passed
}

CCard::~CCard()
{
}

// Return the name of the card as text.
CString CCard::CardToText()
{
CString string;
char buff[64];

	// The cards 2-10 are simply their numerical value
	if(GetRank() <= 10)
	{
		wsprintf(buff, "%d", GetRank());
		string = buff;
	}
	// Face cards need special handling
	else
	{
		switch(GetRank())
		{
			case 11:	// Jack
				string="Jack";
				break;

			case 12:	// Queen
				string="Queen";
				break;

			case 13:	// King
				string="King";
				break;

			case 14:	// Ace
				string="Ace";
				break;

			default:
				ASSERT(FALSE);
				string="????";		// Should never happen
				break;
		}
	}

	// Add the seperator
	string+=" of ";

	// Tack on the suite of the cards
	switch(GetSuite())
	{
		case 0:	// Hearts
			string+="Hearts";
			break;

		case 1:	// Clubs
			string+="Clubs";
			break;

		case 2:	// Diamonds
			string+="Diamonds";
			break;

		case 3:	// Spades
			string+="Spades";
			break;

		default:
			ASSERT(FALSE);
			string="????";		// Should never happen
			break;
	}

return string;
}


// Our poker hand. The constructor simply clears
// the cards to make sure that they are all zero.
CHand::CHand()
{
	ClearCards();
}

// Nothing to do here.
CHand::~CHand()
{
}

void CHand::ClearCards()
{
	// Set all cards to zero (no card)
	for(int i=0; i<MAX_CARDS; i++)
		m_CardArray[i]=CCard(0);

	m_iCards = 0;					// No cards in the hand
}


BOOL CHand::AddCard(CCard& card)
{
	if(m_iCards >= MAX_CARDS)		// Should never happen.
	{
		TRACE("Can't add more cards!\n");
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT(card.IsCard());			// Make sure it's a valid card
	m_CardArray[m_iCards++]=card;

return TRUE;
}

CCard CHand::GetLastCard()
{
	if(m_iCards==0)
		return CCard(0);
	else
		return m_CardArray[m_iCards-1];
}

//--------------------------------------------------------------
// Get our high hand. The LOWORD is the high card and the
// HIWORD is the hand according to the following:
//
//	HIWORD = 	8 -> Straight flush
//				7 -> 4 of a kind
//				6 -> Full House
//				5 -> Flush
//				4 -> Straight
//				3 -> 3 of a kind
//				2 -> 2 Pair
//				1 -> 1 Pair
//				0 -> High card
//
// This means that we can compare hands by simply seeing which
// DWORD is highest. For example... the following hands would
// return the following values:
//
//	High Card (Ace High) - 0x000E
//		HIWORD (00) means High Card
//		LOWORD (0E) means ACE
//
//	9 High straight - 0x0409
//		HIWORD (04) means straight
//		LOWORD (09) means 9 was high
//
// This will accurately compare hands. Even if two players
// were to both receive a straight, the player with the high
// card would still have the higher value.
//--------------------------------------------------------------
DWORD CHand::GetHighHand()
{
	ASSERT(!NeedMoreCards());	// Better have all of our cards

	SortTheCards();				// Sort the hand from highest to lowest

	if(HaveStraightFlush())
	{
		TRACE("Straight flush\n");
		return MAKELONG(HaveStraightFlush(), 8);
	}

	else if(Have4Kind())
	{
		TRACE("4 of a kind\n");
		return MAKELONG(Have4Kind(), 7);
	}

	else if(HaveFullHouse())
	{
		TRACE("Full house\n");
		return MAKELONG(HaveFullHouse(), 6);
	}

	else if(HaveFlush())
	{
		TRACE("Flush\n");
		return MAKELONG(HaveFlush(), 5);
	}

	else if(HaveStraight())
	{
		TRACE("Straight\n");
		return MAKELONG(HaveStraight(), 4);
	}

	else if(Have3Kind())
	{
		TRACE("3 of a kind\n");
		return MAKELONG(Have3Kind(), 3);
	}

	else if(Have2Pair())
	{
		TRACE("2-Pair\n");
		return MAKELONG(Have2Pair(), 2);
	}

	else if(HavePair())
	{
		TRACE("1-Pair\n");
		return MAKELONG(HavePair(), 1);
	}

	// High card is simple. The cards are always sorted from 
	// highest to lowest so return the first (highest) card.
	else
	{
		TRACE("High card\n");
		return MAKELONG(m_CardArray[0].GetRank(), 0);
	}
}

// Sort the hand from highest to lowest
void CHand::SortTheCards()
{
BOOL bSwapped = TRUE;

	// Work until we don't have any more cards to swap.
	while(bSwapped)
	{
		bSwapped = FALSE;		// Assume we are done

		for(int i=0; i< MAX_CARDS-1; i++)
		{
			if(m_CardArray[i] < m_CardArray[i+1])
			{
				bSwapped = TRUE;		// We did a swap
				CCard temp = m_CardArray[i];
				m_CardArray[i] = m_CardArray[i+1];
				m_CardArray[i+1] = temp;
			}
		}
	}
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// This section of code checks for the various poker hands.
// The cards are sorted from highest to lowest prior to
// these routines being called. Nothing tricky is being
// done to determine what the hand it.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

WORD CHand::HaveStraightFlush()
{
	// Scan five (5) consecutive cards and see if both the
	// suite is the same and if the ranks are consecutive.
	for(int i=0; i<3; i++)
	{
		// If the suite is the same
		if(m_CardArray[i].GetSuite() == m_CardArray[i+1].GetSuite() &&
			(m_CardArray[i].GetSuite() == m_CardArray[i+2].GetSuite()) &&
			(m_CardArray[i].GetSuite() == m_CardArray[i+3].GetSuite()) &&
			(m_CardArray[i].GetSuite() == m_CardArray[i+4].GetSuite()))
		{
			if(m_CardArray[i].GetRank() == m_CardArray[i+1].GetRank()+1 &&
				m_CardArray[i].GetRank() == m_CardArray[i+2].GetRank()+2 &&
				m_CardArray[i].GetRank() == m_CardArray[i+3].GetRank()+3 &&
				m_CardArray[i].GetRank() == m_CardArray[i+4].GetRank()+4)

				return m_CardArray[i].GetRank();	// Return highest card
		}
	}

return 0;
}

WORD CHand::Have4Kind()
{
	// Loop thru all of the ranks and count the
	// number of matches for each rank. If we
	// have four matches then we have 4 of a kind.
	for(int rank=14; rank>=2; rank--)
	{
	int iMatches = 0;		// Zero matches

		for(int iCard=0; iCard<MAX_CARDS; iCard++)
			if(m_CardArray[iCard].GetRank() == rank)
				iMatches++;

		if(iMatches == 4)
			return rank;
	}
return 0;
}

WORD CHand::HaveFullHouse()
{
	// If we don't have three of a kind then return zero
	WORD wThreeKind = Have3Kind();
	if(!wThreeKind)
		return 0;

	// Look for a pair skipping the 3-of-a kind rank.
	// We must skip the 3-kind simply because it will
	// also show up as a pair.
	for(int rank=14; rank>=2; rank--)
	{
	int iMatches = 0;		// Zero matches
	if(rank == wThreeKind)	// If it's the card that we have
		continue;			// 3 of a kind in then keep going.

		for(int iCard=0; iCard<MAX_CARDS; iCard++)
			if(m_CardArray[iCard].GetRank() == rank)
				iMatches++;

		if(iMatches == 2)		// If we found a pair then
			return wThreeKind;	// Return the 3Kind rank
	}
return 0;
}

WORD CHand::HaveFlush()
{
	// Similar to 4-of a kind except that we
	// search for 5-of-a-kind and we check the
	// suite of the card and not the face value.
	for(int suite=0; suite < 4; suite++)
	{
	int iMatches = 0;		// Zero matches

		for(int iCard=0; iCard<MAX_CARDS; iCard++)
			if(m_CardArray[iCard].GetSuite() == suite)
				iMatches++;

			// If we have at least 5 of these then
			// return the highest card with this suite.
			if(iMatches >= 5)
			{
				for(iCard=0; iCard<MAX_CARDS; iCard++)
					if(m_CardArray[iCard].GetSuite() == suite)
						return m_CardArray[iCard].GetRank();
			}
	}

return 0;
}


WORD CHand::HaveStraight()
{
	// Scan five (5) consecutive cards and see if the face
	// values of the cards are consecutive
	for(int i=0; i<3; i++)
	{
		if(m_CardArray[i].GetRank() == m_CardArray[i+1].GetRank()+1 &&
			m_CardArray[i].GetRank() == m_CardArray[i+2].GetRank()+2 &&
			m_CardArray[i].GetRank() == m_CardArray[i+3].GetRank()+3 &&
			m_CardArray[i].GetRank() == m_CardArray[i+4].GetRank()+4)

			return m_CardArray[i].GetRank();	// Return highest card
	}

return 0;
}

WORD CHand::Have3Kind()
{
	// Scan thru all of the cards and look for
	// three (3) common face values.
	for(int rank=14; rank>=2; rank--)
	{
	int iMatches = 0;		// Zero matches

		for(int iCard=0; iCard<MAX_CARDS; iCard++)
			if(m_CardArray[iCard].GetRank() == rank)
				iMatches++;

		if(iMatches == 3)
			return rank;
	}
return 0;
}

WORD CHand::Have2Pair()
{
	// If we don't have three of a kind then return zero
	WORD wPair = HavePair();
	if(!wPair)
		return 0;

	// Look for a pair skipping the previously found rank
	for(int rank=14; rank>=2; rank--)
	{
	int iMatches = 0;		// Zero matches
	if(rank == wPair)		// If it's the card that we have
		continue;			// a pair in then keep going.

		for(int iCard=0; iCard<MAX_CARDS; iCard++)
			if(m_CardArray[iCard].GetRank() == rank)
				iMatches++;

		if(iMatches == 2)		// If we found another pair then return the
			return wPair;		// first pair since it's the greater of the two
	}

return 0;
}

WORD CHand::HavePair()
{
	// Just like 3 and 4 of a kind except that we
	// are only looking for a single pair.
	for(int rank=14; rank>=2; rank--)
	{
	int iMatches = 0;		// Zero matches

		for(int iCard=0; iCard<MAX_CARDS; iCard++)
			if(m_CardArray[iCard].GetRank() == rank)
				iMatches++;

		if(iMatches == 2)
			return rank;
	}
return 0;
}


