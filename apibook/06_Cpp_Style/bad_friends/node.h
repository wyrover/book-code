/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   node.h
/// \author Martin Reddy
/// \brief  A single node in a graph. Graph is a friend.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef NODE_H
#define NODE_H

namespace apibook {

///
/// A node in a graph.
///
class Node
{
public:
	friend class Graph;

private:
	void SetVisited();
	bool IsVisited() const;
};

}

#endif
