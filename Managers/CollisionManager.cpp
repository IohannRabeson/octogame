#include "CollisionManager.hpp"
#include "MapManager.hpp"
#include <complex>

CollisionManager::CollisionManager(void) :
	m_player(nullptr),
	m_pairCount(0u),
	m_axis(0.f, 0.f),
	m_edge(0.f, 0.f),
	m_mtv(0.f, 0.f)
{}

CollisionManager::~CollisionManager(void)
{
	if (m_player)
		delete m_player;
}

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
		m_dynamicPolygons[i]->update(pf_deltatime);
	broadPhase();
	narrowPhase();
	for (std::size_t i = 0u; i < m_dynamicPolygons.size(); i++)
		m_dynamicPolygons[i]->applyTransform();
}

void CollisionManager::broadPhase(void)
{
	for (std::size_t k = 0u; k < m_dynamicPolygons.size(); k++)
	{
		sf::Rect<float> const & rect = m_dynamicPolygons[k]->getGlobalBounds();
		int offsetX = static_cast<int>((m_dynamicPolygons[k]->getVelocity().x + rect.left - m_mapManager->getTransitionManager().getOffsetX()) / Tile::TileSize) + 2;
		int offsetY = static_cast<int>((m_dynamicPolygons[k]->getVelocity().y + rect.top - m_mapManager->getTransitionManager().getOffsetY()) / Tile::TileSize) + 2;
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
					if (tile.me_transition != Tile::e_transition_none)
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
}

void CollisionManager::narrowPhase(void)
{
	for(std::size_t i = 0u; i < m_pairCount; i++)
	{
		if (computeCollision(m_pairs[i].m_polygonA, m_pairs[i].m_polygonB))
		{
			//TODO: Block vertical tile
			if (m_pairs[i].m_polygonA->getVelocity().x > 0 &&  m_pairs[i].m_polygonB->getVertex(0).y != m_pairs[i].m_polygonB->getVertex(3).y
					&& m_pairs[i].m_polygonA->getVertex(1).x < m_pairs[i].m_polygonB->getVertex(0).x)
			{
				//static_cast<Tile*>(m_pairs[i].m_polygonB)->m_startColor = (sf::Color::Green);
				m_pairs[i].m_polygonA->addVelocity(-m_mtv);
			}
			if ((m_mtv.y <= -0.00000001f || m_mtv.y >= 0.00000001f) && (m_mtv.x <= -0.00000001f || m_mtv.x >= 0.00000001f))
			{
				//static_cast<Tile*>(m_pairs[i].m_polygonB)->m_startColor = (sf::Color::Blue);
				m_pairs[i].m_polygonA->addVelocity(0.f, m_mtv.y + ((m_mtv.x * m_mtv.x) / m_mtv.y));
			}
			else
			{
				m_pairs[i].m_polygonA->addVelocity(0.f, m_mtv.y);
				//static_cast<Tile*>(m_pairs[i].m_polygonB)->m_startColor = (sf::Color::Red);
			}
		}
	}
	m_pairCount = 0u;
}

//TODO: move to math class
// Normalize vector (magnitude = 1)
void normalize(sf::Vector2<float> & norm, sf::Vector2f const & v)
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

bool CollisionManager::Projection::contains(Projection const & projection)
{
	if (projection.min > min && projection.max < max)
		return true;
	if (min > projection.min && max < projection.max)
		return true;
	return false;
}

void CollisionManager::Projection::project(sf::Vector2<float> const & axis, Polygon * polygon)
{
	sf::Vector2<float> v;
	v.x = static_cast<float>(polygon->getVertex(0u).x);
	v.y = static_cast<float>(polygon->getVertex(0u).y);
	float d = dot(axis, v);
	min = d;
	max = d;
	for (std::size_t i = 1u; i < polygon->getVerticeCount(); i++)
	{
		v.x = static_cast<float>(polygon->getVertex(i).x);
		v.y = static_cast<float>(polygon->getVertex(i).y);
		d = dot(axis, v);
		if (d < min)
			min = d;
		if (d > max)
			max = d;
	}
}

bool CollisionManager::computeCollision(DynamicPolygon * polygonA, Polygon * polygonB)
{
	float magnitude = 999999999.f;
	int	side = 0;
	sf::Vector2f vec;
	sf::Vector2<float> axis;
	for (std::size_t i = 0u; i < polygonA->getVerticeCount(); i++)
	{
		// Calcul du premier axe (normal du coté du polygon)
		normalize(m_axis, polygonA->getNormal(i));
		// On fait une projection du polygonA et du polygonB sur l'axe
		m_projectionA.project(m_axis, polygonA);
		m_projectionB.project(m_axis, polygonB);
		// Si ils sont l'un sur l'autre, on test la normal suivante
		// Si ils ne sont pas l'un sur l'autre, on est certain qu'il existe un axe entre les deux, donc il n'y a pas de collision
		float overlap = m_projectionA.getOverlap(m_projectionB);
		// On ajoute la velocité actuelle pour simuler le prochain déplacment
		float vel = dot(m_axis, polygonA->getVelocity());
		//std::cout << "1 -possible mtv " << m_axis.x << " " << m_axis.y << "  - vel  " << vel <<std::endl;
		if (vel < 0.f)
			m_projectionA.min += vel;
		else
			m_projectionA.max += vel;
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
		if (overlap >= 0.f)
			return false;
		overlap =  -overlap;
		if (overlap < magnitude)
		{
			magnitude = overlap;
			// On oriente le vecteur dans la bonne direction
			if (dot(m_axis, polygonA->getCenter() - polygonB->getCenter()) < 0)
				m_mtv = -m_axis;
			else
				m_mtv = m_axis;
			axis = m_axis;
			side = 1;
			vec = polygonB->getVertex(2);
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
		float overlap = m_projectionA.getOverlap(m_projectionB);
		float vel = dot(m_axis, polygonA->getVelocity());
		if (vel < 0.f)
			m_projectionA.min += vel;
		else
			m_projectionA.max += vel;
		overlap = m_projectionA.getOverlap(m_projectionB);
		if (overlap >= 0.f)
			return false;
		overlap =  -overlap;
		if (overlap < magnitude)
		{
			magnitude = overlap;
			if (dot(m_axis, polygonA->getCenter() - polygonB->getCenter()) < 0)
				m_mtv = -m_axis;
			else
				m_mtv = m_axis;
			axis = m_axis;
			side = 2;
			vec = polygonB->getVertex(2);
		}
	}
	m_mtv *= magnitude;
	// Si on arrive içi c'est que l'on a testé tous les axes, et qu'il n'existe pas d'axe qui sépare les deux polygons, donc il y a une collision
	return true;
}
