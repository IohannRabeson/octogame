#include "GroupShape.hpp"
#include "PhysicsEngine.hpp"
#include <cassert>
#include <limits>

GroupShape::GroupShape(void)
{
	m_shapes.reserve(100u);
}

GroupShape::~GroupShape(void)
{
	//TODO: delete shapes
}

RectangleShape * GroupShape::addRectangleShape(void)
{
	ShapeBuilder & builder = PhysicsEngine::getInstance();
	RectangleShape * rect = builder.createRectangle(true);
	rect->setType(Type::e_trigger);
	m_shapes.push_back(rect);
	return rect;
}

void GroupShape::resetTriggerShapes(void)
{
	PhysicsEngine & engine = PhysicsEngine::getInstance();
	//TODO: reset shapes
}

#include <iostream>
void GroupShape::computeShape(void)
{
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = -std::numeric_limits<float>::max();
	float maxY = -std::numeric_limits<float>::max();

	RectangleShape::computeShape();
	for (auto & shape : m_shapes)
	{
		// Set the position
		sf::Vector2f pos = shape->getPosition();
		shape->setPosition(getPosition() + pos);
		shape->update();
		shape->setPosition(pos);

		//sf::Vector2f o = getPosition() - shape->getPosition() + getOrigin();
		//shape->setOrigin(o);
		//shape->setRotation(getRotation());
		//shape->update();

		sf::FloatRect const & rect = shape->getGlobalBounds();
		if (rect.left < minX)
			minX = rect.left;
		if (rect.left + rect.width > maxX)
			maxX = rect.left + rect.width;
		if (rect.top < minY)
			minY = rect.top;
		if (rect.top + rect.height > maxY)
			maxY = rect.top + rect.height;
	}
	sf::FloatRect const & rect = RectangleShape::getGlobalBounds();
	if (rect.left < minX)
		minX = rect.left;
	if (rect.left + rect.width > maxX)
		maxX = rect.left + rect.width;
	if (rect.top < minY)
		minY = rect.top;
	if (rect.top + rect.height > maxY)
		maxY = rect.top + rect.height;
	m_globalBounds.left = minX;
	m_globalBounds.top = minY;
	m_globalBounds.width = maxX - minX;
	m_globalBounds.height = maxY - minY;
}

void GroupShape::debugDraw(sf::RenderTarget & render)
{
	for (auto & pair : m_shapes)
		pair->debugDraw(render);
	RectangleShape::debugDraw(render);
}
