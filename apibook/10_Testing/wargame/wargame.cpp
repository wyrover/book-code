/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   wargame.cpp
/// \author Martin Reddy
/// \brief  A simple card game to be stub/mock tested.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "wargame.h"
#include <iostream>

using std::cout;
using std::endl;

Card::Card() : mCard("")
{
}

Card::Card(const std::string &name) :
	mCard(name)
{
}

const std::string &Card::Get() const
{
	return mCard;
}

Deck::Deck()
{
	mCards.clear();
}

Deck::~Deck()
{
}

void Deck::Shuffle()
{
	cout << "Deck::Shuffle" << endl;
}

int  Deck::RemainingCards()
{
	return mCards.size();
}

Card Deck::DealCard()
{
	if (mCards.size() == 0)
		return Card();
	Card card = mCards.back();
	mCards.pop_back();
	return card;
}

void Deck::AddCard(const Card &card)
{
	mCards.push_back(card);
}

WarGame::WarGame(Deck &deck) :
	mDeck(deck)
{
}

WarGame::~WarGame()
{
}

void WarGame::Play()
{
	Card card;
	mDeck.Shuffle();
	card = mDeck.DealCard();
	cout << "Card = " << card.Get() << endl;
	card = mDeck.DealCard();
	cout << "Card = " << card.Get() << endl;
	card = mDeck.DealCard();
	cout << "Card = " << card.Get() << endl;
}

WarGame::Player WarGame::GetWinner()
{
	return PLAYER_ONE;
}


