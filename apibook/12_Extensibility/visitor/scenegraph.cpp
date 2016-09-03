/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   scenegraph.cpp
/// \author Martin Reddy
/// \brief  Simple scenegraph API with different node types.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "scenegraph.h"
#include "nodevisitor.h"

#include <vector>

BaseNode::BaseNode(const std::string &name) :
	mName(name)
{
}

ShapeNode::ShapeNode(const std::string &name) :
	BaseNode(name)
{
}

void ShapeNode::Accept(INodeVisitor &visitor)
{
	visitor.Visit(*this);
}

TransformNode::TransformNode(const std::string &name) :
	BaseNode(name)
{
}

void TransformNode::Accept(INodeVisitor &visitor)
{
	visitor.Visit(*this);
}

LightNode::LightNode(const std::string &name) :
	BaseNode(name)
{
}

void LightNode::Accept(INodeVisitor &visitor)
{
	visitor.Visit(*this);
}

int ShapeNode::GetPolygonCount() const
{
	// return a different polygon count each time
	static int called = 1;
	return 100 * ++called;
}

class SceneGraph::Impl
{
public:
	Impl()
	{
		mNodes.push_back(new LightNode("Light0"));
		mNodes.push_back(new TransformNode("Translate"));
		mNodes.push_back(new ShapeNode("Shape0"));
		mNodes.push_back(new TransformNode("Scale"));
		mNodes.push_back(new ShapeNode("Shape1"));
	}

	std::vector<BaseNode *> mNodes;
};

SceneGraph::SceneGraph()
{
	mImpl = new Impl;
}

SceneGraph::~SceneGraph()
{
	delete mImpl;
}

void SceneGraph::Traverse(INodeVisitor &visitor)
{
	std::vector<BaseNode *>::iterator it;
	for (it = mImpl->mNodes.begin(); it != mImpl->mNodes.end(); ++it)
	{
		BaseNode *node = *it;
		node->Accept(visitor);
	}
}
