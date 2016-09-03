/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Showing how friends can be subverted in C++.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "node.h"

// create our own Graph class, which is a friend of Node
namespace apibook {

class Graph
{
public:
	void ViolateAccess(Node &node)
	{
		// call a private method in Node
		node.SetVisited();
	}
};

}

int main(int, char **)
{
	apibook::Node node;
	apibook::Graph local_graph;

	// Use our local Graph class to break into the Node object
	local_graph.ViolateAccess(node);

	return 0;
}
