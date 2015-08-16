#include "PhysicsEngine.hpp"
#include "IContactListener.hpp"
#include "ConvexShape.hpp"
#include "GroupShape.hpp"
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
	m_gravity(0.f, 400.f),
	m_magnitude(0.f),
	m_iterationCount(0u),
	m_tileCollision(false),
	m_contactListener(nullptr)
{}

PhysicsEngine::~PhysicsEngine(void)
{
	unregisterAllShapes();
	unregisterAllTiles();
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
	m_groupShapes.reserve(MaxShapes);

	m_polyPolyPairs.reserve(MaxPairs);
	m_circleCirclePairs.reserve(MaxPairs);
	m_polyCirclePairs.reserve(MaxPairs);

	m_tilePolyPairs.resize(MaxPairs);
	for (auto & vector : m_tilePolyPairs)
		vector.reserve(MaxPairs);

	m_tileCirclePairs.resize(MaxPairs);
	for (auto & vector : m_tileCirclePairs)
		vector.reserve(MaxPairs);
}

void PhysicsEngine::setTileMapSize(sf::Vector2i const & tileMapSize)
{
	m_tileShapes.resize(tileMapSize.x, tileMapSize.y, nullptr);
	m_tileCollision = true;
}

void PhysicsEngine::registerShape(PolygonShape * shape)
{
	assert(m_shapes.size() < MaxShapes);
	m_shapes.push_back(shape);
	assert(m_polygonShapes.size() < MaxShapes);
	m_polygonShapes.push_back(shape);
}

void PhysicsEngine::registerShape(CircleShape * shape)
{
	assert(m_shapes.size() < MaxShapes);
	m_shapes.push_back(shape);
	assert(m_circleShapes.size() < MaxShapes);
	m_circleShapes.push_back(shape);
}

void PhysicsEngine::registerShape(GroupShape * shape)
{
	assert(m_shapes.size() < MaxShapes);
	m_shapes.push_back(shape);
	assert(m_groupShapes.size() < MaxShapes);
	m_groupShapes.push_back(shape);
	assert(m_polygonShapes.size() < MaxShapes);
	m_polygonShapes.push_back(shape);
}

void PhysicsEngine::registerTile(TileShape * shape, std::size_t x,  std::size_t y)
{
	assert(x < m_tileShapes.columns() && y < m_tileShapes.rows());
	m_tileShapes.set(x, y, shape);
}

void PhysicsEngine::unregisterShape(PolygonShape * shape)
{
	if (!shape)
		return;
	m_polygonShapes.erase(std::remove(m_polygonShapes.begin(), m_polygonShapes.end(), shape), m_polygonShapes.end());
	m_shapes.erase(std::remove(m_shapes.begin(), m_shapes.end(), shape), m_shapes.end());
	delete shape;
}

void PhysicsEngine::unregisterShape(CircleShape * shape)
{
	if (!shape)
		return;
	m_circleShapes.erase(std::remove(m_circleShapes.begin(), m_circleShapes.end(), shape), m_circleShapes.end());
	m_shapes.erase(std::remove(m_shapes.begin(), m_shapes.end(), shape), m_shapes.end());
	delete shape;
}

void PhysicsEngine::unregisterAllShapes(void)
{
	for (auto i = m_shapes.begin(); i != m_shapes.end(); i++)
		delete (*i);
	m_shapes.clear();
	m_polygonShapes.clear();
	m_circleShapes.clear();
	m_groupShapes.clear();
}

void PhysicsEngine::unregisterAllTiles(void)
{
	for (std::size_t x = 0u; x < m_tileShapes.columns(); x++)
	{
		for (std::size_t y = 0u; y < m_tileShapes.rows(); y++)
		{
			if (m_tileShapes.get(x, y))
				delete (m_tileShapes.get(x, y));
			m_tileShapes.get(x, y) = nullptr;
		}
	}
	m_tileShapes.resize(0, 0);
	m_tileCollision = false;
}

void PhysicsEngine::update(float deltatime)
{
	// Check if shape is out of screen
	sf::FloatRect const & camRect = octo::Application::getCamera().getRectangle();
	sf::Vector2f gravity = m_gravity * deltatime;
	for (auto & shape : m_shapes)
	{
		shape->setOutOfScreen(!camRect.intersects(shape->getGlobalBounds()));
		// Add gravity
		if (!shape->getSleep() && shape->getApplyGravity())
			shape->addVelocity(gravity);
	}
	// Tile don't move so we update them now
	for (std::size_t i = 0u; i < m_tileShapes.columns(); i++)
	{
		if (!m_tileShapes.get(i, 0u)->getSleep())
			m_tileShapes.get(i, 0u)->update();
	}
	// Determine which pairs of objects might be colliding
	broadPhase();
	// Determine if pairs are colliding
	narrowPhase();
	// Apply the transformation computed by the collision manager
	for (auto & shape : m_shapes)
	{
		if (!shape->getSleep())
			shape->update();
	}
}

void PhysicsEngine::broadPhase(void)
{
	m_polyPolyPairs.clear();
	m_circleCirclePairs.clear();
	m_polyCirclePairs.clear();

	// Poly vs Poly
	broadPhase(m_polygonShapes, m_polygonShapes, m_polyPolyPairs, true);
	// Circle vs Circle
	broadPhase(m_circleShapes, m_circleShapes, m_circleCirclePairs, true);
	// Poly vs Circle
	broadPhase(m_polygonShapes, m_circleShapes, m_polyCirclePairs);
	// Group vs Poly/Circle
	broadPhase(m_groupShapes);

	if (m_tileCollision)
	{
		broadPhase(m_polygonShapes, m_tilePolyPairs);
		broadPhase(m_circleShapes, m_tileCirclePairs);
	}
}

template<class T>
void PhysicsEngine::broadPhase(std::vector<T> const & vector, std::vector<std::vector<Pair<TileShape *, T>>> & pairs)
{
	sf::FloatRect const & camRect = octo::Application::getCamera().getRectangle();
	sf::FloatRect shapeAABB;
	std::size_t count = 0u;
	for (std::size_t i = 0u; i < pairs.size(); i++)
		pairs[i].clear();
	for (auto const & shape : vector)
	{
		sf::FloatRect const & rect = shape->getGlobalBounds();
		float offX = rect.left - camRect.left;
		float w = rect.width + offX;
		int offsetX = static_cast<int>(offX / Tile::TileSize) + 2 - 1;
		int width = static_cast<int>(w / Tile::TileSize) + 2 + 1;
		for (int x = width; x >= offsetX; x--)
		{
			if (x < 0 || x >= static_cast<int>(m_tileShapes.columns()))
				continue;
			if (shape->getSleep() || m_tileShapes(x, 0)->getSleep())
				continue;
			shapeAABB = shape->getGlobalBounds();
			shapeAABB.left += shape->getVelocity().x;
			shapeAABB.top += shape->getVelocity().y;
			if (shapeAABB.intersects(m_tileShapes(x, 0)->getGlobalBounds(), shapeAABB))
			{
				std::size_t i;
				for (i = 0u; i < pairs.size(); i++)
				{
					if (pairs[i].size() == 0u || m_tileShapes(x, 0)->getGlobalBounds().top == pairs[i][0u].m_shapeA->getGlobalBounds().top)
					{
						pairs[i].emplace_back(m_tileShapes(x, 0), shape, shapeAABB.width * shapeAABB.height);
						break;
					}
				}
				if (i > count)
					count = i;
			}
		}
	}
	count++;
	for (std::size_t i = 0u; i < count; i++)
	{
		std::sort(pairs[i].begin(), pairs[i].end(), [](Pair<TileShape *, T> const & pairA, Pair<TileShape *, T> const & pairB)
					{ return pairA.m_area > pairB.m_area; });
	}
	std::sort(pairs.begin(), pairs.begin() + count, [](std::vector<Pair<TileShape *, T>> const & vectorA, std::vector<Pair<TileShape *, T>> const & vectorB)
				{ return vectorA[0u].m_shapeA->getGlobalBounds().top > vectorB[0u].m_shapeA->getGlobalBounds().top; });
}

template<class T, class U>
void PhysicsEngine::broadPhase(std::vector<T> const & vectorA, std::vector<U> const & vectorB, std::vector<Pair<T, U>> & pairs, bool cullingDuplicate)
{
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
				pairs.emplace_back(vectorA[i], vectorB[k]);
			}
		}
	}
}

template<class T, class U>
void PhysicsEngine::broadPhase(std::vector<T> const & vector, CircleShape * shape, std::vector<Pair<U, CircleShape *>> & pairs)
{
	for (std::size_t i = 0u; i < vector.size(); i++)
	{
		if (vector[i]->getSleep() || shape->getSleep())
			continue;
		else if (static_cast<AShape *>(vector[i]) == static_cast<AShape *>(shape))
			continue;
		if (vector[i]->getGlobalBounds().intersects(shape->getGlobalBounds()))
		{
			pairs.emplace_back(vector[i], shape);
		}
	}
}

template<class T, class U>
void PhysicsEngine::broadPhase(std::vector<T> const & vector, PolygonShape * shape, std::vector<Pair<PolygonShape *, U>> & pairs)
{
	for (std::size_t i = 0u; i < vector.size(); i++)
	{
		if (vector[i]->getSleep() || shape->getSleep())
			continue;
		else if (static_cast<AShape *>(vector[i]) == static_cast<AShape *>(shape))
			continue;
		if (vector[i]->getGlobalBounds().intersects(shape->getGlobalBounds()))
		{
			pairs.emplace_back(shape, vector[i]);
		}
	}
}

void PhysicsEngine::broadPhase(std::vector<GroupShape *> const & groups)
{
	for (auto const & group : groups)
	{
		if (group->getSleep())
			continue;
		for (auto const & shape : m_polygonShapes)
		{
			if (shape->getSleep())
				continue;
			if (group->getGroupGlobalBounds().intersects(shape->getGlobalBounds()))
			{
				broadPhase(group->getPolygons(), shape, m_polyPolyPairs);
				broadPhase(group->getCircles(), shape, m_polyCirclePairs);
			}
		}
		for (auto const & shape : m_circleShapes)
		{
			if (shape->getSleep())
				continue;
			if (group->getGroupGlobalBounds().intersects(shape->getGlobalBounds()))
			{
				broadPhase(group->getPolygons(), shape, m_polyCirclePairs);
				broadPhase(group->getCircles(), shape, m_circleCirclePairs);
			}
		}
	}
}


void PhysicsEngine::narrowPhase(void)
{
	narrowPhase(m_polyPolyPairs);
	narrowPhase(m_circleCirclePairs);
	narrowPhase(m_polyCirclePairs);

	if (m_tileCollision)
	{
		narrowPhaseTile(m_tilePolyPairs);
		narrowPhaseTile(m_tileCirclePairs);
	}
}

template<class T, class U>
void PhysicsEngine::narrowPhase(std::vector<Pair<T, U>> & pairs)
{
	for (std::size_t i = 0u; i < pairs.size(); i++)
	{
		if (computeCollision(pairs[i].m_shapeA, pairs[i].m_shapeB))
		{
			if ((pairs[i].m_shapeA->getType() == AShape::Type::e_dynamic || pairs[i].m_shapeA->getType() == AShape::Type::e_static)
				&& (pairs[i].m_shapeB->getType() == AShape::Type::e_dynamic || pairs[i].m_shapeB->getType() == AShape::Type::e_static))
			{
				if (!pairs[i].m_shapeA->isType(AShape::Type::e_trigger) && !pairs[i].m_shapeB->isType(AShape::Type::e_trigger))
				{
					m_mtv /= 2.f;
					if (pairs[i].m_shapeA->getType() == AShape::Type::e_dynamic)
						pairs[i].m_shapeA->addVelocity(m_mtv.x, m_mtv.y);
					if (pairs[i].m_shapeA->getType() == AShape::Type::e_static)
						pairs[i].m_shapeB->addVelocity(-m_mtv.x, -m_mtv.y);
				}
				if (m_contactListener)
					m_contactListener->onShapeCollision(pairs[i].m_shapeA, pairs[i].m_shapeB);
			}
		}
	}
}

template<class T>
void PhysicsEngine::narrowPhaseTile(std::vector<std::vector<Pair<TileShape *, T>>> & pairs)
{
	bool collide;
	for (std::size_t i = 0u; i < pairs.size(); i++)
	{
		for (auto & pair : pairs[i])
		{
			// The shape A is the tile, the shapeB is the other one
			sf::Vector2f vel = pair.m_shapeB->getVelocity() / static_cast<float>(m_iterationCount);
			pair.m_shapeB->setVelocity(0.f, 0.f);
			collide = false;
			for (std::size_t j = 0u; j < m_iterationCount; j++)
			{
				pair.m_shapeB->addVelocity(vel);
				if (computeCollision(pair.m_shapeA, pair.m_shapeB))
				{
					if (!pair.m_shapeB->isType(AShape::Type::e_trigger))
					{
						if (std::fabs(m_mtv.y) < std::numeric_limits<float>::epsilon())
						{
							pair.m_shapeB->addVelocity(-m_mtv.x, 0.f);
						}
						else if (std::fabs(m_mtv.x) > std::numeric_limits<float>::epsilon())
						{
							pair.m_shapeB->addVelocity(0.f, -(m_mtv.y + ((m_mtv.x * m_mtv.x) / m_mtv.y)));
						}
						else
						{
							pair.m_shapeB->addVelocity(0.f, -m_mtv.y);
						}
						pair.m_shapeB->update();
					}
					collide = true;
				}
			}
			if (collide && m_contactListener)
				m_contactListener->onTileShapeCollision(pair.m_shapeA, pair.m_shapeB);
		}
	}
}

bool PhysicsEngine::findAxisLeastPenetration(TileShape * tile, PolygonShape * polygon)
{
	for(std::size_t i = 0u; i < polygon->getEfficientVertexCount(); ++i)
	{
		m_axis = polygon->getNormal(i);
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
		sf::Vector2f const & s = polygonB->getSupportVertex(-m_axis);
		// On récupère chaque vertex
		sf::Vector2f const & v = polygonA->getVertex(i);
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
	m_magnitude = -std::numeric_limits<float>::max();
	m_axis = tile->getNormal(0u);
	octo::normalize(m_axis);
	m_projectionA.project(m_axis, tile);
	m_projectionB.project(m_axis, circle);
	m_projectionB.swept(m_axis, circle);
	float overlap = m_projectionA.getOverlap(m_projectionB);
	if (overlap >= 0.f)
		return false;
	if (overlap > m_magnitude)
	{
		m_magnitude = overlap;
		m_mtv = m_axis;
	}

	m_axis = circle->getBaryCenter() - tile->getBaryCenter();
	octo::normalize(m_axis);
	m_projectionA.project(m_axis, tile);
	m_projectionB.project(m_axis, circle);
	m_projectionB.swept(m_axis, circle);
	overlap = m_projectionA.getOverlap(m_projectionB);
	if (overlap >= 0.f)
		return false;
	if (overlap > m_magnitude)
	{
		m_magnitude = overlap;
		m_mtv = m_axis;
	}
	if (octo::dotProduct(m_mtv, circle->getBaryCenter() - tile->getBaryCenter()) < 0.f)
		m_mtv = -m_mtv;
	m_mtv *= m_magnitude;
	return true;
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
	for (std::size_t i = 0u; i < m_tileCirclePairs.size(); i++)
	{
		for (auto const & pair : m_tilePolyPairs[i])
			pair.m_shapeA->debugDraw(render);
	}
	for (std::size_t i = 0u; i < m_tileCirclePairs.size(); i++)
	{
		for (auto const & pair : m_tileCirclePairs[i])
			pair.m_shapeA->debugDraw(render);
	}
	for (auto const & shape : m_shapes)
		shape->debugDraw(render);
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

template<class T>
void PhysicsEngine::Projection::project(sf::Vector2f const & axis, T * polygon)
{
	float d = octo::dotProduct(axis, polygon->getVertex(0u));
	min = d;
	max = d;
	for (std::size_t i = 1u; i < polygon->getEfficientVertexCount(); i++)
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
