/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   graph.cpp
/// \author Martin Reddy
/// \brief  A graph that contains multiple nodes.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "graph.h"

namespace apibook {

void Graph::AddNode(const Node &node)
{
	mNodes.push_back(node);
}

void Graph::Refresh()
{
	std::vector<Node>::iterator it;
	for ( it = mNodes.begin(); it != mNodes.end(); ++it)
	{
		Node &node = *it;
		node.SetVisited();
	}
}

}
