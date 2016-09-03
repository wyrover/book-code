/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   test_mock.cpp
/// \author Martin Reddy
/// \brief  Demonstrating testing with a mock object.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "wargame.h"
#include <iostream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

///
/// A mock deck of cards instrumented with Google Mock
///
class MockDeck : public Deck
{
public:
	MOCK_METHOD0(Shuffle, void());
	MOCK_METHOD0(RemainingCards, int());
	MOCK_METHOD0(DealCard, Card());
};

TEST(WarGame, Test1)
{
	// Define the expected behavior of the mock deck
	MockDeck deck;
	EXPECT_CALL(deck, Shuffle())
		.Times(AtLeast(1));

	EXPECT_CALL(deck, DealCard())
		.Times(3)
		.WillOnce(Return(Card("JS")))
		.WillOnce(Return(Card("2H")))
		.WillOnce(Return(Card("9C")))
		;

	// play out the War game using the mock deck
	WarGame game(deck);
	game.Play();
	ASSERT_EQ(game.GetWinner(), WarGame::PLAYER_ONE);

	// on destruction, the mock deck will validate its usage
}

int main(int argc, char** argv)
{
	InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}

