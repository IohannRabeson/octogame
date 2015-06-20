#include "ShapeBuilder.hpp"
#include "PhysicsEngine.hpp"
#include "ConvexShape.hpp"
#include "CircleShape.hpp"
#include "RectangleShape.hpp"

ConvexShape * ShapeBuilder::createConvex(void)
{
	ConvexShape * shape = new ConvexShape();
	PhysicsEngine::getInstance().registerShape(shape);
	return shape;
}

CircleShape * ShapeBuilder::createCircle(void)
{
	CircleShape * shape = new CircleShape();
	PhysicsEngine::getInstance().registerShape(shape);
	return shape;
}

RectangleShape * ShapeBuilder::createRectangle(void)
{
	RectangleShape * shape = new RectangleShape();
	PhysicsEngine::getInstance().registerShape(shape);
	return shape;
}

ConvexShape * ShapeBuilder::createTile(std::size_t x, std::size_t y)
{
	ConvexShape * shape = new ConvexShape();
	shape->setVertexCount(4u);
	PhysicsEngine::getInstance().registerTile(shape, x, y);
	return shape;
}

