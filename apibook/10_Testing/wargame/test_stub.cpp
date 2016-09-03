/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   test_stub.cpp
/// \author Martin Reddy
/// \brief  Demonstrating testing with a stub object.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "wargame.h"
#include <iostream>
#include <stdlib.h>

#define Assert(x, y) if ((x) != (y)) abort()

///
/// A stub deck of cards that always returns the same cards
///
class StubDeck : public Deck
{
public:
	void Shuffle()
	{
		std::cout << "StubDeck::Shuffle" << std::endl;
	}

	void AddCard(const Card &card)
	{
		//  convert protected method to public
		Deck::AddCard(card);
	}
};

void TestWarGame()
{
	// fill the stub deck with specific cards
	StubDeck deck;
	deck.AddCard(Card("9C"));
	deck.AddCard(Card("2H"));
	deck.AddCard(Card("JS"));

	// play the War game using the stub deck of cards
	WarGame game(deck);
	game.Play();
	Assert(game.GetWinner(), WarGame::PLAYER_ONE);
}


int main(int, char **)
{
	TestWarGame();
	return 0;
}
