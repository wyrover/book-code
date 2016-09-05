/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   wargame.h
/// \author Martin Reddy
/// \brief  A simple card game to be stub/mock tested.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include <string>
#include <vector>

///
/// A single playing card
///
class Card
{
public:
	Card();
	explicit Card(const std::string &name);

	const std::string &Get() const;

private:
	std::string mCard;
};

///
/// A deck of cards
///
class Deck
{
public:
	Deck();
	virtual ~Deck();
	virtual void Shuffle();
	virtual int  RemainingCards();
	virtual Card DealCard();

protected:
	void AddCard(const Card &card);

private:
	std::vector<Card> mCards;
};

///
/// The game logic for the card game "War"
///
class WarGame
{
public:
	enum Player { PLAYER_ONE, PLAYER_TWO };

	/// Initialize the War game with a deck of cards
	explicit WarGame(Deck &deck);
	~WarGame();

	/// Play out the entire game to completion
	void Play();
	/// Return which player won the game
	Player GetWinner();

private:
	Deck &mDeck;
};
