/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   nodevisitor.h
/// \author Martin Reddy
/// \brief  An interface for visitor classes to implement.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef NODE_VISITOR_H
#define NODE_VISITOR_H

class ShapeNode;
class TransformNode;
class LightNode;

///
/// An abstract base class for a scenegraph visitor
///
class INodeVisitor
{
public:
	virtual ~INodeVisitor() {}
	virtual void Visit(ShapeNode &node) = 0;
	virtual void Visit(TransformNode &node) = 0;
	virtual void Visit(LightNode &node) = 0;
};

#endif
