#include "PhysicsEngine.hpp"
#include "AShape.hpp"
#include "IContactListener.hpp"
#include "Circle.hpp"
#include "Polygon.hpp"
#include <Math.hpp>
#include <assert.h>
#include <limits>

PhysicsEngine::PhysicsEngine(void) :
	m_contactListener(nullptr),
	m_gravity(0.f, 800.f)
{
}

void PhysicsEngine::init(void)
{
	m_shapes.reserve(MaxShapes);
	m_pairs.resize(1000u);
}

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

void PhysicsEngine::registerShape(AShape * shape)
{
	assert(m_shapes.size() < MaxShapes);
	m_shapes.push_back(shape);
}

#include <iostream>
void PhysicsEngine::broadPhase(void)
{
	m_pairCount = 0u;
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
						m_pairs[m_pairCount].m_polygonA = m_dynamicPolygons[k];
						m_pairs[m_pairCount].m_polygonB = &tile;
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
						m_pairs[m_pairCount].m_polygonA = m_dynamicPolygons[k];
						m_pairs[m_pairCount].m_polygonB = &tile;
						m_pairCount++;
					}
				}
			}
		}
	}*/
	for (std::size_t i = 0u; i < m_shapes.size(); i++)
	{
		for (std::size_t k = i + 1u; k < m_shapes.size(); k++)
		{
			if (!m_shapes[i]->getSleep() && !m_shapes[k]->getSleep() && m_shapes[i] != m_shapes[k])
			{
				if (m_shapes[i]->getGlobalBounds().intersects(m_shapes[k]->getGlobalBounds()))
				{
					//TODO: remove casr
					m_pairs[m_pairCount].m_polygonA = dynamic_cast<Polygon *>(m_shapes[i]);
					m_pairs[m_pairCount].m_polygonB = dynamic_cast<Polygon *>(m_shapes[k]);
					m_pairCount++;
				}
			}
		}
		std::cout << m_pairCount << std::endl;
	}
}

void PhysicsEngine::narrowPhase(void)
{
	for(std::size_t i = 0u; i < m_pairCount; i++)
	{
		m_pairs[i].m_isColliding = false;
		if (m_pairs[i].m_polygonB->isColliding(m_pairs[i].m_polygonA->getCollisionType())
			|| m_pairs[i].m_polygonA->isColliding(m_pairs[i].m_polygonB->getCollisionType()))
		{
			// Collision with tiles to replace DynamicsPolygon
			if (computeCollision(m_pairs[i].m_polygonA, m_pairs[i].m_polygonB))
			{
				// Move the polygon on X axis
				/*if (m_pairs[i].m_polygonA->getVelocity().x > 0.f && m_pairs[i].m_polygonB->getVertex(0).y != m_pairs[i].m_polygonB->getVertex(3).y
					&& m_pairs[i].m_polygonB->getVertex(1).x >= m_pairs[i].m_polygonA->getVertex(1).x
					&& m_pairs[i].m_polygonB->getVertex(0).y <= m_pairs[i].m_polygonA->getVertex(3).y)
				{
					m_pairs[i].m_polygonA->addVelocity(m_mtv.x, 0.f);
				}
				else if (m_pairs[i].m_polygonA->getVelocity().x < 0.f && m_pairs[i].m_polygonB->getVertex(1).y != m_pairs[i].m_polygonB->getVertex(2).y
					&& m_pairs[i].m_polygonB->getVertex(0).x <= m_pairs[i].m_polygonA->getVertex(0).x
					&& m_pairs[i].m_polygonB->getVertex(1).y <= m_pairs[i].m_polygonA->getVertex(3).y)
				{
					m_pairs[i].m_polygonA->addVelocity(m_mtv.x, 0.f);
				}
				// Move the polygon on Y axis
				if ((m_mtv.y <= -0.00000001f || m_mtv.y >= 0.00000001f) && (m_mtv.x <= -0.00000001f || m_mtv.x >= 0.00000001f))
				{
					//m_pairs[i].m_polygonA->addVelocity(0.f, m_mtv.y + ((m_mtv.x * m_mtv.x) / m_mtv.y));
					m_mtv.y = m_mtv.y + ((m_mtv.x * m_mtv.x) / m_mtv.y);
				}
				else
				{
					//m_pairs[i].m_polygonA->addVelocity(0.f, m_mtv.y);
				}*/
				if (m_pairs[i].m_polygonA->getType() == AShape::Type::e_dynamic
					|| m_pairs[i].m_polygonA->getType() == AShape::Type::e_kinematic)
				m_pairs[i].m_polygonA->addVelocity(m_mtv.x, m_mtv.y);
				std::cout << m_pairs[i].m_polygonA->getVelocity().y << " " << m_mtv.y << std::endl;
			}
			//m_pairs[i].m_polygonA->onCollision(m_pairs[i].m_polygonB);
			//IContactListener
			m_pairs[i].m_isColliding = true;
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

bool PhysicsEngine::computeCollision(Circle * , Polygon *)
{
	std::cout << "circle" << std::endl;
	return true;
}

bool PhysicsEngine::computeCollision(Circle * , Circle * )
{
	std::cout << "circle" << std::endl;
	return true;
}

bool PhysicsEngine::test1(Polygon * polygonA, Polygon * polygonB)
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
			std::cout << "CONTAINEMENT " << min << " " << max <<  std::endl;
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

bool PhysicsEngine::computeCollision(Polygon * polygonA, Polygon * polygonB)
{
	m_magnitude = std::numeric_limits<float>::max();
	if (!test1(polygonA, polygonB) || !test1(polygonB, polygonA))
		return false;
	// On modifie dirige le vecteur dans la bonne direction
	if (octo::dotProduct(m_mtv, polygonA->getCenter() - polygonB->getCenter()) < 0)
		m_mtv = -m_mtv;
	m_mtv *= m_magnitude / 2.f;
	// Si on arrive içi c'est que l'on a testé tous les axes, et qu'il n'existe pas d'axe qui sépare les deux polygons, donc il y a une collision
	return true;
}

void PhysicsEngine::debugDraw(sf::RenderTarget & render) const
{
	for (auto i = m_shapes.begin(); i != m_shapes.end(); i++)
		(*i)->debugDraw(render);
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

void PhysicsEngine::Projection::project(sf::Vector2f const & axis, Polygon * polygon)
{
	sf::Vector2f v = polygon->getVertex(0u);
	float d = octo::dotProduct(axis, v);
	min = d;
	max = d;
	for (std::size_t i = 1u; i < polygon->getVertexCount(); i++)
	{
		sf::Vector2f v = polygon->getVertex(i);
		d = octo::dotProduct(axis, v);
		if (d < min)
			min = d;
		if (d > max)
			max = d;
	}
}
