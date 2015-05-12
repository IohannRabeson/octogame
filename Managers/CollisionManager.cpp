#include "CollisionManager.hpp"
#include "MapManager.hpp"
#include <complex>

CollisionManager::CollisionManager(void) :
	m_player(nullptr),
	m_pairCount(0u),
	m_axis(0.f, 0.f),
	m_edge(0.f, 0.f),
	m_mtv(0.f, 0.f)
{
}

CollisionManager::~CollisionManager(void)
{
	if (m_player)
		delete m_player;
}

#include <iostream>
void CollisionManager::init(MapManager * p_mapManager)
{
	m_mapManager = p_mapManager;
	m_player = new Player();
	m_pairs.resize(1000u);
	m_dynamicPolygons.push_back(m_player);
}

void CollisionManager::update(float pf_deltatime)
{
	for (std::size_t i = 0u; i < m_dynamicPolygons.size(); i++)
		m_dynamicPolygons[i]->updateVelocity(pf_deltatime);
	broadPhase();
	narrowPhase();
	for (std::size_t i = 0u; i < m_dynamicPolygons.size(); i++)
		m_dynamicPolygons[i]->update(pf_deltatime);
}

void CollisionManager::broadPhase(void)
{
	for (std::size_t k = 0u; k < m_dynamicPolygons.size(); k++)
	{
		sf::Rect<float> const & rect = m_dynamicPolygons[k]->getGlobalBounds();
		int offsetX = static_cast<int>((m_dynamicPolygons[k]->m_velocity.x + rect.left - m_mapManager->getTransitionManager().getOffsetX()) / Tile::TileSize) + 2;
		int offsetY = static_cast<int>((m_dynamicPolygons[k]->m_velocity.y + rect.top - m_mapManager->getTransitionManager().getOffsetY()) / Tile::TileSize) + 2;
		int width = static_cast<int>(rect.width / Tile::TileSize) + 1 + offsetX;
		int height = static_cast<int>(rect.height / Tile::TileSize) + 1 + offsetY;
		//TODO: manage out of screen
		for (int i = offsetX; i <= width; i++)
		{
			for (int j = height; j >= offsetY; j--)
			{
				// TODO: get right size
				if (i < 0 || i >= static_cast<int>(m_mapManager->getTransitionManager().getMapWidth()) || j < 0 || j >= static_cast<int>(m_mapManager->getTransitionManager().getMapHeight()))
					continue;
				Tile & tile = m_mapManager->getTransitionManager().getTile(i, j);
				if (tile.me_transition != Tile::e_transition_none)
				{
					m_pairs[m_pairCount].m_polygonA = m_dynamicPolygons[k];
					m_pairs[m_pairCount].m_polygonB = &tile;
					m_pairCount++;
				}
			}
		}
	}
}

void CollisionManager::narrowPhase(void)
{
	int nb = 0;
	for(std::size_t i = 0u; i < m_pairCount; i++)
	{
		if (computeCollision(m_pairs[i].m_polygonA, m_pairs[i].m_polygonB))
		{
			nb++;
			//std::cout << " ---- " << m_pairs[i].m_polygonA->getVertex(3).x << " " << m_pairs[i].m_polygonA->getVertex(3).y << " "  << std::endl;
			//std::cout << m_pairs[i].m_polygonA->m_velocity.x << " " << m_pairs[i].m_polygonA->m_velocity.y << " "  << std::endl;
			//std::cout << m_mtv.x << " " << m_mtv.y << std::endl;
			m_pairs[i].m_polygonA->m_velocity += m_mtv;
		}
	}
	m_pairCount = 0u;
}

//TODO: move to math class
// Normalize vector (magnitude = 1)
void normalize(sf::Vector2f & norm, sf::Vector2f const & v)
{
	float magnitude = sqrt(v.x * v.x + v.y * v.y);
	norm.x = v.x / magnitude;
	norm.y = v.y / magnitude;
}

// Dot product
float dot(sf::Vector2f const & v1, sf::Vector2f const & v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

bool CollisionManager::Projection::overlap(Projection const & projection)
{
	if (min > projection.max || projection.min > max)
		return false;
	return true;
}

float CollisionManager::Projection::getOverlap(Projection const & projection)
{
	if (min < projection.min)
		return projection.min - max;
	return min - projection.max;
}

void CollisionManager::Projection::project(sf::Vector2f const & axis, Polygon * polygon)
{
	float d = dot(axis, polygon->getVertex(0u));
	min = d;
	max = d;
	for (std::size_t i = 1u; i < polygon->getVerticeCount(); i++)
	{
		d = dot(axis, polygon->getVertex(i));
		if (d < min)
			min = d;
		if (d > max)
			max = d;
	}
}

bool CollisionManager::computeCollision(DynamicPolygon * polygonA, Polygon * polygonB)
{
	float magnitude = 999999999.f;
	for (std::size_t i = 0u; i < polygonA->getVerticeCount(); i++)
	{
		// Calcul du premier axe (normal du coté du polygon)
		// TODO: normalize mtv only ?
		normalize(m_axis, polygonA->getNormal(i));
		// On fait une projection du polygonA et du polygonB sur l'axe
		m_projectionA.project(m_axis, polygonA);
		m_projectionB.project(m_axis, polygonB);
		// Si ils sont l'un sur l'autre, on test la normal suivante
		// Si ils ne sont pas l'un sur l'autre, on est certain qu'il existe un axe entre les deux, donc il n'y a pas de collision
		float mag = m_projectionA.getOverlap(m_projectionB);
		// On ajoute la velocité actuelle pour simuler le prochain déplacment
		float vel = dot(m_axis, polygonA->m_velocity);
		if (vel < 0.f)
			m_projectionA.min += vel;
		else
			m_projectionA.max += vel;
		mag = m_projectionA.getOverlap(m_projectionB);
		if (mag >= 0.f)
			return false;
		mag = std::abs(mag);
		if (mag < magnitude)
		{
			magnitude = mag;
			// On oriente le vecteur dans la bonne direction
			if (dot(m_axis, polygonA->getCenter() - polygonB->getCenter()) < 0)
				m_mtv = -m_axis;
			else
				m_mtv = m_axis;
		}
	}
	for (std::size_t i = 0u; i < polygonB->getVerticeCount(); i++)
	{
		normalize(m_axis, polygonB->getNormal(i));
		// TODO: Tile, remove properly same point
		if (polygonB->getNormal(i).x == 0 && polygonB->getNormal(i).y == 0)
		{
			continue;
		}
		m_projectionA.project(m_axis, polygonA);
		m_projectionB.project(m_axis, polygonB);
		float mag = m_projectionA.getOverlap(m_projectionB);
		float vel = dot(m_axis, polygonA->m_velocity);
		if (vel < 0.f)
			m_projectionA.min += vel;
		else
			m_projectionA.max += vel;
		mag = m_projectionA.getOverlap(m_projectionB);
		if (mag >= 0.f)
			return false;
		mag = std::abs(mag);
		if (mag < magnitude)
		{
			magnitude = mag;
			if (dot(m_axis, polygonA->getCenter() - polygonB->getCenter()) < 0)
				m_mtv = -m_axis;
			else
				m_mtv = m_axis;
		}
	}
	m_mtv *= magnitude;
	// Si on arrive içi c'est que l'on a testé tous les axes, et qu'il n'existe pas d'axe qui sépare les deux polygons, donc il y a une collision
	return true;
}

Player & CollisionManager::getPlayer(void) const
{
	return *m_player;
}
