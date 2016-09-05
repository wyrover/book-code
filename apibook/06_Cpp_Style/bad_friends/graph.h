/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   graph.h
/// \author Martin Reddy
/// \brief  A graph that contains multiple nodes.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include <vector>

namespace apibook {

///
/// A graph of nodes.
///
class Graph
{
public:
	/// Add a node to the graph
	void AddNode(const Node &node);

	/// Refresh the contents of the graph
	void Refresh();

private:
	std::vector<Node> mNodes;
};

}

#endif
