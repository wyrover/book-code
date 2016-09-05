/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   scenegraph.h
/// \author Martin Reddy
/// \brief  Simple scenegraph API with different node types.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <string>

class INodeVisitor;

///
/// An abstract base class used by all nodes in the scenegraph.
///
class BaseNode
{
public:
	explicit BaseNode(const std::string &name);
	virtual ~BaseNode() {}

	virtual void Accept(INodeVisitor &visitor) = 0;

private:
	std::string mName;
};

///
/// A concrete node to represent a shape
///
class ShapeNode : public BaseNode
{
public:
	explicit ShapeNode(const std::string &name);

	void Accept(INodeVisitor &visitor);
	int GetPolygonCount() const;
};

///
/// A concrete node to represent a transform matrix
///
class TransformNode : public BaseNode 
{
public:
	explicit TransformNode(const std::string &name);

	void Accept(INodeVisitor &visitor);
};

///
/// A concrete node to represent a light
///
class LightNode : public BaseNode
{
public:
	explicit LightNode(const std::string &name);

	void Accept(INodeVisitor &visitor);
};

///
/// The scenegraph object.
///
class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();

	/// Call the relevant Accept() method of the node visitor class
	/// for every node in the scenegraph
	void Traverse(INodeVisitor &visitor);

private:
	// Make this object be noncopyable because it holds a pointer
	SceneGraph(const SceneGraph &);
	const SceneGraph &operator =(const SceneGraph &);

	class Impl;
	Impl *mImpl;
};

#endif
