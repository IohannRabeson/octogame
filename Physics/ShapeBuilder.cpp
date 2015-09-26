#include "ShapeBuilder.hpp"
#include "PhysicsEngine.hpp"
#include "ConvexShape.hpp"
#include "RectangleShape.hpp"
#include "GroupShape.hpp"
#include "CircleShape.hpp"
#include "TileShape.hpp"

ConvexShape * ShapeBuilder::createConvex(bool inGroup)
{
	ConvexShape * shape = new ConvexShape();
	if (!inGroup)
		PhysicsEngine::getInstance().registerShape(shape);
	return shape;
}

CircleShape * ShapeBuilder::createCircle(bool inGroup)
{
	CircleShape * shape = new CircleShape();
	if (!inGroup)
		PhysicsEngine::getInstance().registerShape(shape);
	return shape;
}

RectangleShape * ShapeBuilder::createRectangle(bool inGroup)
{
	RectangleShape * shape = new RectangleShape();
	if (!inGroup)
		PhysicsEngine::getInstance().registerShape(shape);
	return shape;
}

GroupShape * ShapeBuilder::createGroupShape(void)
{
	GroupShape * shape = new GroupShape();
	PhysicsEngine::getInstance().registerShape(shape);
	return shape;
}

TileShape * ShapeBuilder::createTile(std::size_t x, std::size_t y)
{
	TileShape * shape = new TileShape();
	PhysicsEngine::getInstance().registerTile(shape, x, y);
	return shape;
}
