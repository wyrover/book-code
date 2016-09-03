/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Demonstrate the Visitor design pattern.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "scenegraph.h"
#include "nodevisitor.h"

#include <iostream>

///
/// A visitor class that counts various elements
/// in a scenegraph object.
///
class MyVisitor : public INodeVisitor
{
public:
	int mNumShapes;
	int mNumPolygons;
	int mNumTransforms;
	int mNumLights;

	MyVisitor() : 
		mNumShapes(0),
		mNumPolygons(0),
		mNumTransforms(0),
		mNumLights(0)
	{
	}

	void Visit(ShapeNode &node)
	{
		mNumPolygons += node.GetPolygonCount();
		++mNumShapes;
	}

	void Visit(TransformNode &node)
	{
		++mNumTransforms;
	}

	void Visit(LightNode &node)
	{
		++mNumLights;
	}
};

int main(int, char **)
{
	SceneGraph sg;
	MyVisitor visitor;

	// traverse the scene graph using the given visitor
	sg.Traverse(visitor);

	std::cout << "Shapes:     " << visitor.mNumShapes << std::endl;
	std::cout << "Polygons:   " << visitor.mNumPolygons << std::endl;
	std::cout << "Transforms: " << visitor.mNumTransforms << std::endl;
	std::cout << "Lights:     " << visitor.mNumLights << std::endl;

	return 0;
}
