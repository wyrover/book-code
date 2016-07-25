
#ifndef __CARDS_H__
#define __CARDS_H__

// Card class. Encompasses a playing card
class	CCard
{
public:
	CCard();
	CCard(DWORD);
	~CCard();

	BOOL operator<(CCard& card)			// Less than
		{return (this->GetRank() < card.GetRank());}

	BOOL operator>(CCard& card)			// Greater than
		{return (this->GetRank() > card.GetRank());}

	BOOL IsCard(){return m_dwCard!=0;}	// Is this a valid card
	WORD GetSuite(){return (WORD)LOWORD(m_dwCard);}		// Suite is 0-3
	WORD GetRank(){return (WORD)HIWORD(m_dwCard);}		// Rank is 2-14
	CString CardToText();

protected:
	DWORD	m_dwCard;
};

// Represents a single poker hand.
class	CHand
{
public:
	CHand();
	~CHand();

	void ClearCards();							// Remove all cards in the hand
	BOOL AddCard(CCard&);						// Add a card to our hand
	CCard GetLastCard();						// Return last card in hand
	int		GetCardCount(){return m_iCards;}	// Return # of cards
	BOOL NeedMoreCards(){return m_iCards < MAX_CARDS;}
	DWORD GetHighHand();	// Get our high hand

	enum{
		MAX_CARDS = 7,
		};

protected:
	void	SortTheCards();				// Sorts the cards from highest to lowest

	// All of these functions return a zero if the hand doesn't exist
	// or the higest card in the hand if it does exist.
	WORD	HaveStraightFlush();
	WORD	Have4Kind();
	WORD	HaveFullHouse();
	WORD	HaveFlush();
	WORD	HaveStraight();
	WORD	Have3Kind();
	WORD	Have2Pair();
	WORD	HavePair();

protected:
	CCard	m_CardArray[MAX_CARDS];		// Our seven cards
	int		m_iCards;					// Number of cards in hand
};

#endif	// __CARDS_H__
