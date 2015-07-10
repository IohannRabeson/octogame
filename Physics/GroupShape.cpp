#include "GroupShape.hpp"
#include "PhysicsEngine.hpp"
#include "CircleShape.hpp"
#include <cassert>
#include <limits>

GroupShape::GroupShape(void)
{
	m_shapes.reserve(100u);
	m_polyShapes.reserve(100u);
	m_circleShapes.reserve(100u);
}

GroupShape::~GroupShape(void)
{
	for (auto & shape : m_shapes)
		delete shape;
}

RectangleShape * GroupShape::addRectangleShape(void)
{
	ShapeBuilder & builder = PhysicsEngine::getInstance();
	RectangleShape * rect = builder.createRectangle(true);
	rect->setType(Type::e_trigger);
	m_shapes.push_back(rect);
	m_polyShapes.push_back(rect);
	return rect;
}

CircleShape * GroupShape::addCircleShape(void)
{
	ShapeBuilder & builder = PhysicsEngine::getInstance();
	CircleShape * rect = builder.createCircle(true);
	rect->setType(Type::e_trigger);
	m_shapes.push_back(rect);
	m_circleShapes.push_back(rect);
	return rect;
}

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
	m_groupGlobalBounds.left = minX;
	m_groupGlobalBounds.top = minY;
	m_groupGlobalBounds.width = maxX - minX;
	m_groupGlobalBounds.height = maxY - minY;
}

void GroupShape::debugDraw(sf::RenderTarget & render)
{
	sf::RectangleShape rect;
	rect.setOutlineColor(sf::Color::Red);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(1);
	rect.setPosition(sf::Vector2f(m_groupGlobalBounds.left, m_groupGlobalBounds.top));
	rect.setSize(sf::Vector2f(m_groupGlobalBounds.width, m_groupGlobalBounds.height));
	render.draw(rect);
	for (auto & pair : m_shapes)
		pair->debugDraw(render);
	RectangleShape::debugDraw(render);
}
