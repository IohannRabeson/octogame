#include "PhysicsEngine.hpp"
#include "IContactListener.hpp"
#include "ConvexShape.hpp"
#include "CircleShape.hpp"
#include "Tile.hpp"
#include "TileShape.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <Math.hpp>
#include <cassert>
#include <limits>

std::unique_ptr<PhysicsEngine> PhysicsEngine::m_instance = nullptr;

PhysicsEngine::PhysicsEngine(void) :
	m_polyPolyPairCount(0u),
	m_polyCirclePairCount(0u),
	m_circleCirclePairCount(0u),
	m_tilePolyPairCount(0u),
	m_tileCirclePairCount(0u),
	m_gravity(0.f, 400.f),
	m_magnitude(0.f),
	m_iterationCount(0u),
	m_tileCollision(false),
	m_contactListener(nullptr)
{}

PhysicsEngine::~PhysicsEngine(void)
{
		// TODO: erase
	for (auto i = m_shapes.begin(); i != m_shapes.end(); i++)
		delete (*i);
	for (auto i = m_tileShapes.begin(); i != m_tileShapes.end(); i++)
		delete (*i);
}

PhysicsEngine & PhysicsEngine::getInstance(void)
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new PhysicsEngine());
		m_instance->init();
	}
	return *m_instance;
}

ShapeBuilder & PhysicsEngine::getShapeBuilder(void)
{
	return getInstance();
}

void PhysicsEngine::init(void)
{
	m_shapes.reserve(MaxShapes);
	m_circleShapes.reserve(MaxShapes);
	m_polygonShapes.reserve(MaxShapes);
	//TODO: get the right size
	m_tileShapes.resize(200u, 200u, nullptr);

	//TODO: use max pairs and assert in broadphae
	m_polyPolyPairs.resize(1000u);
	m_circleCirclePairs.resize(1000u);
	m_polyCirclePairs.resize(1000u);
	m_tilePolyPairs.resize(1000u);
	m_tileCirclePairs.resize(1000u);
}

void PhysicsEngine::registerShape(PolygonShape * shape)
{
	assert(m_shapes.size() < MaxShapes);
	assert(m_polygonShapes.size() < MaxShapes);
	m_shapes.push_back(shape);
	m_polygonShapes.push_back(shape);
}

void PhysicsEngine::registerShape(CircleShape * shape)
{
	assert(m_shapes.size() < MaxShapes);
	assert(m_circleShapes.size() < MaxShapes);
	m_shapes.push_back(shape);
	m_circleShapes.push_back(shape);
}

void PhysicsEngine::registerTileS(TileShape * shape, std::size_t x,  std::size_t y)
{
	assert(x < m_tileShapes.columns() && y < m_tileShapes.rows());
	m_tileShapes.set(x, y, shape);
}

void PhysicsEngine::update(float deltatime)
{
	// Add gravity
	for (std::size_t i = 0u; i < 123/*m_tileShapes.columns()*/; i++)
	{
		//for (std::size_t k = 0u; k < 72/*m_tileShapes.rows()*/; k++)
		{
				if (!m_tileShapes.get(i, 0)->getSleep())
					m_tileShapes.get(i, 0)->update();
		}
	}
	sf::Vector2f gravity = m_gravity * deltatime;
	for (auto shape : m_shapes)
	{
		if (!shape->getSleep() && shape->getApplyGravity())
			shape->addVelocity(gravity);
	}
	// Determine which pairs of objects might be colliding
	broadPhase();
	// Determine if pairs are colliding
	narrowPhase();
	// Apply the transformation computed by the collision manager
	for (auto shape : m_shapes)
	{
		if (!shape->getSleep())
			shape->update();
	}
}

void PhysicsEngine::broadPhase(void)
{
	m_polyPolyPairCount = broadPhase(m_polygonShapes, m_polygonShapes, m_polyPolyPairs, true);
	m_circleCirclePairCount = broadPhase(m_circleShapes, m_circleShapes, m_circleCirclePairs, true);
	m_polyCirclePairCount = broadPhase(m_polygonShapes, m_circleShapes, m_polyCirclePairs);

	if (m_tileCollision)
	{
		m_tilePolyPairCount = broadPhase(m_polygonShapes, m_tilePolyPairs);
		m_tileCirclePairCount = broadPhase(m_circleShapes, m_tileCirclePairs);
	}
}

#include <iostream>
template<class T>
std::size_t PhysicsEngine::broadPhase(std::vector<T> const & vector, std::vector<Pair<TileShape *, T>> & pairs)
{
	sf::FloatRect const & camRect = octo::Application::getCamera().getRectangle();
	sf::FloatRect shapeAABB;
	std::size_t count = 0u;
	std::size_t nb = 0u;
	for (auto shape : vector)
	{
		sf::Rect<float> const & rect = shape->getGlobalBounds();
		//TODO fix bug camera move
		float offX = rect.left - camRect.left;
		//if (rect.left < 0)
		//	offX--;
		//float offY = rect.top + Tile::DoubleTileSize - camRect.top;
		float w = rect.width + offX;
		//float h = rect.height + Tile::TileSize + offY + shape->getVelocity().y;
		int offsetX = static_cast<int>(offX / Tile::TileSize) + 2 - 1;
		//int offsetY = static_cast<int>(offY / Tile::TileSize);
		int width = static_cast<int>(w / Tile::TileSize) + 2 + 1;
		//int height = static_cast<int>(h / Tile::TileSize);
		if (shape->getVelocity().x < 0)
		{
			// TODO: make one more method to did this
			for (int x = width; x >= offsetX; x--)
			{
				//for (int y = height; y >= offsetY; y--)
				//for (int y = offsetY; y <= height; y++)
				{
					// TODO: check out of bounds
					//if (i < 0 || i >= static_cast<int>(m_mapManager->getTransitionManager().getMapWidth()) || j < 0 || j >= static_cast<int>(m_mapManager->getTransitionManager().getMapHeight()))
					//	continue;
					if (shape->getSleep() || m_tileShapes(x, 0)->getSleep())
						continue;
					// TODO: may be a better choice ?
					nb++;
					shapeAABB = shape->getGlobalBounds();
					shapeAABB.left += shape->getVelocity().x;
					shapeAABB.top += shape->getVelocity().y;
					if (shapeAABB.intersects(m_tileShapes(x, 0)->getGlobalBounds()))
					{
						pairs[count].m_shapeA = m_tileShapes(x, 0);
						pairs[count++].m_shapeB = shape;
					}
				}
			}
		}
		else
		{
			for (int x = offsetX; x <= width; x++)
			{
				//for (int y = height; y >= offsetY; y--)
				{
					if (shape->getSleep() || m_tileShapes(x, 0)->getSleep())
						continue;
					nb++;
					shapeAABB = shape->getGlobalBounds();
					shapeAABB.left += shape->getVelocity().x;
					shapeAABB.top += shape->getVelocity().y;
					if (shapeAABB.intersects(m_tileShapes(x, 0)->getGlobalBounds()))
					{
						pairs[count].m_shapeA = m_tileShapes(x, 0);
						pairs[count++].m_shapeB = shape;
					}
				}
			}
		}
	}
	return count;
}

template<class T, class U>
std::size_t PhysicsEngine::broadPhase(std::vector<T> const & vectorA, std::vector<U> const & vectorB, std::vector<Pair<T, U>> & pairs, bool cullingDuplicate)
{
	std::size_t count = 0u;
	for (std::size_t i = 0u; i < vectorA.size(); i++)
	{
		if (vectorA[i]->getSleep())
			continue;
		for (std::size_t k = 0u; k < vectorB.size(); k++)
		{
			if (vectorB[k]->getSleep() || (cullingDuplicate && k <= i))
				continue;
			else if (static_cast<AShape *>(vectorA[i]) == static_cast<AShape *>(vectorB[k]))
				continue;
			if (vectorA[i]->getGlobalBounds().intersects(vectorB[k]->getGlobalBounds()))
			{
				pairs[count].m_shapeA = vectorA[i];
				pairs[count++].m_shapeB = vectorB[k];
			}
		}
	}
	return count;
}

void PhysicsEngine::narrowPhase(void)
{
	narrowPhase(m_polyPolyPairs, m_polyPolyPairCount);
	narrowPhase(m_circleCirclePairs, m_circleCirclePairCount);
	narrowPhase(m_polyCirclePairs, m_polyCirclePairCount);

	if (m_tileCollision)
	{
		narrowPhaseTile(m_tilePolyPairs, m_tilePolyPairCount);
		narrowPhaseTile(m_tileCirclePairs, m_tileCirclePairCount);
	}
}

template<class T, class U>
void PhysicsEngine::narrowPhase(std::vector<Pair<T, U>> & pairs, std::size_t pairCount)
{
	for(std::size_t i = 0u; i < pairCount; i++)
	{
		if (computeCollision(pairs[i].m_shapeA, pairs[i].m_shapeB))
		{
			// TODO: manage type kinematic static, ...
			if (pairs[i].m_shapeA->getType() == AShape::Type::e_dynamic || pairs[i].m_shapeA->getType() == AShape::Type::e_kinematic)
			{
				m_mtv /= 2.f;
				pairs[i].m_shapeA->addVelocity(m_mtv.x, m_mtv.y);
				pairs[i].m_shapeB->addVelocity(-m_mtv.x, -m_mtv.y);
			}
		}
	}
}

template<class T, class U>
void PhysicsEngine::narrowPhaseTile(std::vector<Pair<T, U>> & pairs, std::size_t pairCount)
{
	for(std::size_t i = 0u; i < pairCount; i++)
	{
		// The shape A is the tile, the shapeB is the other one
		sf::Vector2f vel = pairs[i].m_shapeB->getVelocity() / static_cast<float>(m_iterationCount);
		pairs[i].m_shapeB->setVelocity(0.f, 0.f);

		for (std::size_t j = 0u; j < m_iterationCount; j++)
		{
			pairs[i].m_shapeB->addVelocity(vel);
			//TODO: if a shape will collide with a lot of tile, a AABB recheck could be efficient
			if (computeCollision(pairs[i].m_shapeA, pairs[i].m_shapeB))
			{
				if ((m_mtv.y <= -0.00000001f || m_mtv.y >= 0.00000001f) && (m_mtv.x <= -0.00000001f || m_mtv.x >= 0.00000001f))
				{
					pairs[i].m_shapeB->addVelocity(0.f, -(m_mtv.y + ((m_mtv.x * m_mtv.x) / m_mtv.y)));
				}
				else if (m_mtv.y >= -0.00000001f && m_mtv.y <= 0.00000001f)
				{
					pairs[i].m_shapeB->addVelocity(-m_mtv.x, 0.f);
				}
				else
				{
					pairs[i].m_shapeB->addVelocity(0.f, -m_mtv.y);
				}
			}
		}
	}
}

bool PhysicsEngine::findAxisLeastPenetration(TileShape * tile, PolygonShape * polygon)
{
	for(std::size_t i = 0u; i < polygon->getEfficientVertexCount(); ++i)
	{
		m_axis = polygon->getNormal(i);
		if (m_axis.x == 0.f && m_axis.y == 0.f)
			continue;
		octo::normalize(m_axis);
		// On récupère le supportPoint dans le sens inverse de l'axe
		sf::Vector2f const & s = tile->getSupportVertex(-m_axis);
		// On récupère chaque vertex
		sf::Vector2f const & v = polygon->getVertex(i) + polygon->getVelocity();
		// Projection de s - v sur l'axe, si d < 0.f, il y a une possible collision
		float d = octo::dotProduct(m_axis, s - v);
		// Il y a un axe separateur
		if (d >= 0.f)
			return false;
		if (d > m_magnitude)
		{
			m_magnitude = d;
			m_mtv = m_axis;
		}
	}

	// For the tile, we only test the axis of the top
	m_axis = tile->getNormal(0u);
	if (m_axis.x != 0.f && m_axis.y != 0.f)
	{
		octo::normalize(m_axis);
		// On récupère le supportPoint dans le sens inverse de l'axe
		sf::Vector2f const & s = polygon->getSupportVertex(-m_axis) + polygon->getVelocity();
		// On récupère chaque vertex
		sf::Vector2f v = tile->getVertex(0u);
		// Projection de s - v sur l'axe, si d < 0.f, il y a une possible collision
		float d = octo::dotProduct(m_axis, s - v);
		// Il y a un axe separateur
		if (d >= 0.f)
			return false;
		if (d > m_magnitude)
		{
			m_magnitude = d;
			m_mtv = m_axis;
		}
	}
	return true;
}

bool PhysicsEngine::findAxisLeastPenetration(PolygonShape * polygonA, PolygonShape * polygonB)
{
	for(std::size_t i = 0u; i < polygonA->getEfficientVertexCount(); ++i)
	{
		//Retrieve a face normal from A
		m_axis = polygonA->getNormal(i);
		if (m_axis.x == 0.f && m_axis.y == 0.f)
		{
			continue;
		}
		octo::normalize(m_axis);
		// On récupère le supportPoint dans le sens inverse de l'axe
		sf::Vector2f const & s = polygonB->getSupportVertex(-m_axis) + polygonB->getVelocity();
		// On récupère chaque vertex
		sf::Vector2f const & v = polygonA->getVertex(i) + polygonA->getVelocity();
		// Projection de s - v sur l'axe, si d < 0.f, il y a une possible collision
		float d = octo::dotProduct(m_axis, s - v);

		// Il y a un axe separateur
		if (d >= 0.f)
			return false;

		float vel = octo::dotProduct(m_axis, polygonA->getVelocity());
		float vell = octo::dotProduct(m_axis, polygonB->getVelocity());

		float dd = d;
		if (vel > 0.f)
			dd += vel;
		if (vell > 0.f)
			dd += vell;

		if (d >= 0.f)
			return false;

		// Store greatest distance
		if (d > m_magnitude)
		{
			m_magnitude = d;
			m_mtv = m_axis;
		}
	}
	return true;
}

bool PhysicsEngine::computeCollision(TileShape * tile, PolygonShape * polygon)
{
	m_magnitude = -std::numeric_limits<float>::max();
	if (!findAxisLeastPenetration(tile, polygon))
		return false;
	if (octo::dotProduct(m_mtv, tile->getBaryCenter() - polygon->getBaryCenter()) > 0.f)
		m_mtv = -m_mtv;
	m_mtv *= m_magnitude;
	return true;
}

bool PhysicsEngine::computeCollision(PolygonShape * polygonA, PolygonShape * polygonB)
{
	m_magnitude = -std::numeric_limits<float>::max();
	if (!findAxisLeastPenetration(polygonA, polygonB) || !findAxisLeastPenetration(polygonB, polygonA))
		return false;
	if (octo::dotProduct(m_mtv, polygonA->getBaryCenter() - polygonB->getBaryCenter()) > 0.f)
		m_mtv = -m_mtv;
	m_mtv *= m_magnitude;
	return true;
}

bool PhysicsEngine::computeCollision(CircleShape * circleA, CircleShape * circleB)
{
	m_magnitude = -std::numeric_limits<float>::max();
	m_axis = circleA->getBaryCenter() - circleB->getBaryCenter();
	octo::normalize(m_axis);
	m_projectionA.project(m_axis, circleA);
	m_projectionB.project(m_axis, circleB);
	m_projectionA.swept(m_axis, circleA);
	m_projectionB.swept(m_axis, circleB);
	float overlap = m_projectionA.getOverlap(m_projectionB);
	if (overlap >= 0.f)
		return false;
	if (overlap > m_magnitude)
	{
		m_magnitude = overlap;
		m_mtv = m_axis;
	}
	if (octo::dotProduct(m_mtv, m_axis) > 0.f)
		m_mtv = -m_mtv;
	m_mtv *= m_magnitude;
	return true;
}

bool PhysicsEngine::computeCollision(TileShape * tile, CircleShape * circle)
{
	return computeCollision(tile, circle);
}

bool PhysicsEngine::computeCollision(PolygonShape * polygon, CircleShape * circle)
{
	m_magnitude = -std::numeric_limits<float>::max();
	for (std::size_t i = 0u; i < polygon->getEfficientVertexCount(); i++)
	{
		m_axis = polygon->getNormal(i);
		octo::normalize(m_axis);
		m_projectionA.project(m_axis, polygon);
		m_projectionB.project(m_axis, circle);
		m_projectionA.swept(m_axis, polygon);
		m_projectionB.swept(m_axis, circle);
		float overlap = m_projectionA.getOverlap(m_projectionB);
		if (overlap >= 0.f)
			return false;
		if (overlap > m_magnitude)
		{
			m_magnitude = overlap;
			m_mtv = m_axis;
		}
	}
	m_axis = circle->getBaryCenter() - polygon->getBaryCenter();
	octo::normalize(m_axis);
	m_projectionA.project(m_axis, polygon);
	m_projectionB.project(m_axis, circle);
	m_projectionA.swept(m_axis, polygon);
	m_projectionB.swept(m_axis, circle);
	float overlap = m_projectionA.getOverlap(m_projectionB);
	if (overlap >= 0.f)
		return false;
	if (overlap > m_magnitude)
	{
		m_magnitude = overlap;
		m_mtv = m_axis;
	}
	if (octo::dotProduct(m_mtv, circle->getBaryCenter() - polygon->getBaryCenter()) < 0.f)
		m_mtv = -m_mtv;
	m_mtv *= m_magnitude;
	return true;
}

void PhysicsEngine::debugDraw(sf::RenderTarget & render) const
{
	for (auto shape : m_shapes)
		shape->debugDraw(render);
	for (std::size_t i = 0u; i < m_tilePolyPairCount; i++)
		m_tilePolyPairs[i].m_shapeA->debugDraw(render);
}

// Nested Class Projection
float PhysicsEngine::Projection::getOverlap(Projection const & projection)
{
	if (min < projection.min)
		return projection.min - max;
	return min - projection.max;
}

bool PhysicsEngine::Projection::contains(Projection const & projection)
{
	if (projection.min > min && projection.max < max)
		return true;
	if (min > projection.min && max < projection.max)
		return true;
	return false;
}

void PhysicsEngine::Projection::project(sf::Vector2f const & axis, PolygonShape * polygon)
{
	float d = octo::dotProduct(axis, polygon->getVertex(0u));
	min = d;
	max = d;
	for (std::size_t i = 1u; i < polygon->getVertexCount(); i++)
	{
		d = octo::dotProduct(axis, polygon->getVertex(i));
		if (d < min)
			min = d;
		if (d > max)
			max = d;
	}
}

void PhysicsEngine::Projection::project(sf::Vector2f const & axis, CircleShape * circle)
{
	float d = octo::dotProduct(axis, circle->getBaryCenter());
	min = d - circle->getRadius();
	max = d + circle->getRadius();
}

void PhysicsEngine::Projection::swept(sf::Vector2f const & axis, AShape const * shape)
{
	float vel = octo::dotProduct(axis, shape->getVelocity());
	min += vel;
	max += vel;
}
