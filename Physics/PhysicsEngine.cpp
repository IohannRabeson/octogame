#include "PhysicsEngine.hpp"
#include "IContactListener.hpp"
#include "PolygonShape.hpp"
#include "CircleShape.hpp"
#include <Math.hpp>
#include <cassert>
#include <limits>

PhysicsEngine::PhysicsEngine(void) :
	m_gravity(0.f, 800.f),
	m_contactListener(nullptr)
{
}

void PhysicsEngine::init(void)
{
	m_shapes.reserve(MaxShapes);
	m_circleShapes.reserve(MaxShapes);
	m_polygonShapes.reserve(MaxShapes);
	m_polyPolyPairs.resize(1000u);
	m_polyCirclePairs.resize(1000u);
}

#include <iostream>
void PhysicsEngine::update(float deltatime)
{
	// Add gravity
	sf::Vector2f gravity = m_gravity * deltatime;
	for (auto i = m_shapes.begin(); i != m_shapes.end(); i++)
	{
		if (!(*i)->getSleep() && (*i)->getApplyGravity())
			(*i)->addVelocity(gravity);
	}
	// Determine which paris of objects might be colliding
	broadPhase();
	// Determine if pairs are colliding
	narrowPhase();
	// Apply the transformation computed by the collision manager
	for (auto i = m_shapes.begin(); i != m_shapes.end(); i++)
	{
		if (!(*i)->getSleep())
			(*i)->update();
	}
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

void PhysicsEngine::registerTile(PolygonShape * shape, int x,  int y)
{
	assert(m_shapes.size() < MaxShapes);
	assert(m_tileShapes.size() < MaxShapes);
	m_shapes.push_back(shape);
	m_tileShapes(x, y) = shape;
}

void PhysicsEngine::broadPhase(void)
{
	m_pairCount = 0u;
	m_cpairCount = 0u;
	/*for (std::size_t k = 0u; k < m_dynamicPolygons.size(); k++)
	{
		sf::Rect<float> const & rect = m_dynamicPolygons[k]->getGlobalBounds();
		int offsetX = static_cast<int>((m_dynamicPolygons[k]->getVelocity().x + rect.left) / Tile::TileSize) + 2 - static_cast<int>(m_mapManager->getCameraManager().getUpLeft()->x / Tile::TileSize);
		if (rect.left < 0)
			offsetX--;
		int offsetY = static_cast<int>((m_dynamicPolygons[k]->getVelocity().y + rect.top) / Tile::TileSize) + 2 - static_cast<int>(m_mapManager->getCameraManager().getUpLeft()->y / Tile::TileSize);
		int width = static_cast<int>(rect.width / Tile::TileSize) + 1 + offsetX;
		int height = static_cast<int>(rect.height / Tile::TileSize) + 1 + offsetY;
		if (m_dynamicPolygons[k]->getVelocity().x < 0)
		{
			for (int i = width; i >= offsetX; i--)
			{
				for (int j = height; j >= offsetY; j--)
				{
					if (i < 0 || i >= static_cast<int>(m_mapManager->getTransitionManager().getMapWidth()) || j < 0 || j >= static_cast<int>(m_mapManager->getTransitionManager().getMapHeight()))
						continue;
					Tile & tile = m_mapManager->getTransitionManager().getTile(i, j);
					if (tile.getCollideType() & m_dynamicPolygons[k]->getCollideMask() && tile.me_transition != Tile::e_transition_none)
					{
						m_polyPolyPairs[m_pairCount].getShapeA() = m_dynamicPolygons[k];
						m_polyPolyPairs[m_pairCount].getShapeB() = &tile;
						m_pairCount++;
					}
				}
			}
		}
		else
		{
			for (int i = offsetX; i <= width; i++)
			{
				for (int j = height; j >= offsetY; j--)
				{
					if (i < 0 || i >= static_cast<int>(m_mapManager->getTransitionManager().getMapWidth()) || j < 0 || j >= static_cast<int>(m_mapManager->getTransitionManager().getMapHeight()))
						continue;
					Tile & tile = m_mapManager->getTransitionManager().getTile(i, j);
					if (tile.getCollideType() & m_dynamicPolygons[k]->getCollideMask() && tile.me_transition != Tile::e_transition_none)
					{
						m_polyPolyPairs[m_pairCount].getShapeA() = m_dynamicPolygons[k];
						m_polyPolyPairs[m_pairCount].getShapeB() = &tile;
						m_pairCount++;
					}
				}
			}
		}
	}*/
	m_pairCount = broadPhase(m_polygonShapes, m_polygonShapes, m_polyPolyPairs, true);
	m_cpairCount = broadPhase(m_polygonShapes, m_circleShapes, m_polyCirclePairs);
}

template<class T, class U>
std::size_t PhysicsEngine::broadPhase(std::vector<T> const & vectorA, std::vector<U> const & vectorB, std::vector<Pair<T, U>> & pairs, bool cullingDuplicate)
{
	std::size_t	count = 0u;
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
				pairs[count++].setPair(vectorA[i], vectorB[k]);
		}
	}
	return count;
}

void PhysicsEngine::narrowPhase(void)
{
	narrowPhase(m_polyPolyPairs, m_pairCount);
	narrowPhase(m_polyCirclePairs, m_cpairCount);
}

template<class T, class U>
void PhysicsEngine::narrowPhase(std::vector<Pair<T, U>> & pairs, std::size_t pairCount)
{
	for(std::size_t i = 0u; i < pairCount; i++)
	{
		pairs[i].setColliding(false);
		if (computeCollision(pairs[i].getShapeA(), pairs[i].getShapeB()))
		{
			if (pairs[i].getShapeA()->getType() == AShape::Type::e_dynamic || pairs[i].getShapeA()->getType() == AShape::Type::e_kinematic)
			{
				pairs[i].getShapeA()->addVelocity(m_mtv.x / 2.f, m_mtv.y / 2.f);
				pairs[i].getShapeB()->addVelocity(-m_mtv.x / 2.f, -m_mtv.y / 2.f);
			}
			pairs[i].setColliding(true);
		}
	}
}

//TODO: move to math class
// Normalize vector (magnitude = 1)
void normalize(sf::Vector2f & norm, sf::Vector2f const & v)
{
	float magnitude = sqrt(v.x * v.x + v.y * v.y);
	norm.x = v.x / magnitude;
	norm.y = v.y / magnitude;
}

bool PhysicsEngine::resolveCollision(PolygonShape * polygonA, PolygonShape * polygonB)
{
	for (std::size_t i = 0u; i < polygonA->getVertexCount(); i++)
	{
		// Calcul du premier axe (normal du coté du polygon)
		normalize(m_axis, polygonA->getNormal(i));
		// TODO: Tile, remove properly same point
		if (polygonB->getNormal(i).x == 0 && polygonB->getNormal(i).y == 0)
		{
			continue;
		}
		// On fait une projection du polygonA et du polygonB sur l'axe
		m_projectionA.project(m_axis, polygonA);
		m_projectionB.project(m_axis, polygonB);
		float overlap = m_projectionA.getOverlap(m_projectionB);
		// On ajoute la velocité actuelle pour simuler le prochain déplacment
		float vel = octo::dotProduct(m_axis, polygonA->getVelocity());
		//std::cout << "1 -possible mtv " << m_axis.x << " " << m_axis.y << "  - vel  " << vel <<std::endl;
		if (vel < 0.f)
			m_projectionA.min += vel;
		else
			m_projectionA.max += vel;
		overlap = m_projectionA.getOverlap(m_projectionB);
		if (m_projectionA.contains(m_projectionB))
		{
			float min = std::abs(m_projectionA.min - m_projectionB.min);
			float max = std::abs(m_projectionA.max - m_projectionB.max);
			if (min < max)
				overlap += min;
			else
				overlap += max;
			//std::cout << "CONTAINEMENT " << min << " " << max <<  std::endl;
		}
		// Si ils sont l'un sur l'autre, on test la normal suivante
		// Si ils ne sont pas l'un sur l'autre, on est certain qu'il existe un axe entre les deux, donc il n'y a pas de collision
		if (overlap >= 0.f)
			return false;
		overlap = -overlap;
		if (overlap < m_magnitude)
		{
			m_magnitude = overlap;
			m_mtv = m_axis;
		}
	}
	return true;
}

bool PhysicsEngine::computeCollision(PolygonShape * polygonA, PolygonShape * polygonB)
{
	m_magnitude = std::numeric_limits<float>::max();
	if (!resolveCollision(polygonA, polygonB) || !resolveCollision(polygonB, polygonA))
		return false;
	// On modifie dirige le vecteur dans la bonne direction
	if (octo::dotProduct(m_mtv, polygonA->getCenter() - polygonB->getCenter()) < 0)
		m_mtv = -m_mtv;
	m_mtv *= m_magnitude / 2.f;
	// Si on arrive içi c'est que l'on a testé tous les axes, et qu'il n'existe pas d'axe qui sépare les deux polygons, donc il y a une collision
	return true;
}

bool PhysicsEngine::computeCollision(PolygonShape * polygon, CircleShape * circle)
{
	m_magnitude = std::numeric_limits<float>::max();
	sf::Vector2f const & support = polygon->getNearest(circle->getCenter());
	m_support = support;
	m_axis = support - circle->getCenter();
	// On fait une projection du polygon sur l'axe
	m_projectionA.project(m_axis, polygon);
	m_projectionB.project(m_axis, circle);
	float overlap = m_projectionA.getOverlap(m_projectionB);
	// On ajoute la velocité actuelle pour simuler le prochain déplacment
	float vel = octo::dotProduct(m_axis, polygon->getVelocity());
	if (vel < 0.f)
		m_projectionA.min += vel;
	else
		m_projectionA.max += vel;
	vel = octo::dotProduct(m_axis, circle->getVelocity());
	if (vel < 0.f)
		m_projectionB.min += vel;
	else
		m_projectionB.max += vel;
	overlap = m_projectionA.getOverlap(m_projectionB);
	// Si ils sont l'un sur l'autre, on test la normal suivante
	// Si ils ne sont pas l'un sur l'autre, on est certain qu'il existe un axe entre les deux, donc il n'y a pas de collision
	if (overlap >= 0.f)
		return false;
	overlap = -overlap;
	if (overlap < m_magnitude)
	{
		m_magnitude = overlap;
		m_mtv = m_axis;
	}
	for (std::size_t i = 0u; i < polygon->getVertexCount(); i++)
	{
		// Calcul du premier axe (normal du coté du polygon)
		normalize(m_axis, polygon->getNormal(i));
		// On fait une projection du polygon sur l'axe
		m_projectionA.project(m_axis, polygon);
		m_projectionB.project(m_axis, circle);
		float overlap = m_projectionA.getOverlap(m_projectionB);
		// On ajoute la velocité actuelle pour simuler le prochain déplacment
		float vel = octo::dotProduct(m_axis, polygon->getVelocity());
		if (vel < 0.f)
			m_projectionA.min += vel;
		else
			m_projectionA.max += vel;
		vel = octo::dotProduct(m_axis, circle->getVelocity());
		if (vel < 0.f)
			m_projectionB.min += vel;
		else
			m_projectionB.max += vel;
		overlap = m_projectionA.getOverlap(m_projectionB);
		/*if (m_projectionA.contains(m_projectionB))
		{
			float min = std::abs(m_projectionA.min - m_projectionB.min);
			float max = std::abs(m_projectionA.max - m_projectionB.max);
			if (min < max)
				overlap += min;
			else
				overlap += max;
			//std::cout << "CONTAINEMENT " << min << " " << max <<  std::endl;
		}*/
		// Si ils sont l'un sur l'autre, on test la normal suivante
		// Si ils ne sont pas l'un sur l'autre, on est certain qu'il existe un axe entre les deux, donc il n'y a pas de collision
		if (overlap >= 0.f)
			return false;
		overlap = -overlap;
		if (overlap < m_magnitude)
		{
			m_magnitude = overlap;
			m_mtv = m_axis;
		}
	}
	if (octo::dotProduct(m_mtv, polygon->getCenter() - circle->getCenter()) < 0)
		m_mtv = -m_mtv;
	m_mtv *= m_magnitude / 2.f;
	// Si on arrive içi c'est que l'on a testé tous les axes, et qu'il n'existe pas d'axe qui sépare les deux polygons, donc il y a une collision
	return true;
}

sf::Vector2f const & PhysicsEngine::getSupportVertex(PolygonShape * polygon, sf::Vector2f const & direction)
{
	float bestProjection = -std::numeric_limits<float>::max();
	sf::Vector2f const * bestVertex;
	for (std::size_t i = 0; i < polygon->getVertexCount(); i++)
	{
		sf::Vector2f const & v = polygon->getVertex(i);
		float projection = octo::dotProduct(v, direction);
		if (projection > bestProjection)
		{
			bestVertex = &v;
			bestProjection = projection;
		}
	}
	return *bestVertex;
}

void PhysicsEngine::debugDraw(sf::RenderTarget & render) const
{
	for (auto i = m_shapes.begin(); i != m_shapes.end(); i++)
		(*i)->debugDraw(render);

	sf::Vector2f sizeX(10.f, 0.f);
	sf::Vector2f sizeY(0.f, 10.f);

	sf::Vertex line[] =
	{
		sf::Vertex(m_support - sizeX, sf::Color::Magenta),
		sf::Vertex(m_support + sizeX, sf::Color::Magenta),
		sf::Vertex(m_support - sizeY, sf::Color::Magenta),
		sf::Vertex(m_support + sizeY, sf::Color::Magenta),
	};

	render.draw(line, 4, sf::Lines);
}

// Nested Class Projection
bool PhysicsEngine::Projection::overlap(Projection const & projection)
{
	if (min > projection.max || projection.min > max)
		return false;
	return true;
}

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
	sf::Vector2f v = circle->getCenter();
	float d = octo::dotProduct(axis, v);
	min = d - circle->getRadius();
	max = d + circle->getRadius();
}
