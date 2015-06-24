#include "PhysicsEngine.hpp"
#include "IContactListener.hpp"
#include "ConvexShape.hpp"
#include "CircleShape.hpp"
#include "RectangleShape.hpp"
#include "Tile.hpp"
#include <Math.hpp>
#include <cassert>
#include <limits>

std::unique_ptr<PhysicsEngine> PhysicsEngine::m_instance = nullptr;

PhysicsEngine::PhysicsEngine(void) :
	m_polyPolyPairCount(0u),
	m_polyCirclePairCount(0u),
	m_circleCirclePairCount(0u),
	m_projectionA(),
	m_projectionB(),
	m_axis(),
	m_mtv(),
	m_gravity(0.f, 800.f),
	m_magnitude(0.f),
	m_contactListener(nullptr)
{
}

PhysicsEngine::~PhysicsEngine(void)
{
	m_shapes.clear();
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
	m_tileShapes.set(x, y, shape);
}

void PhysicsEngine::update(float deltatime)
{
	// Add gravity
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
	m_polyPolyPairCount = 0u;
	m_circleCirclePairCount = 0u;
	m_polyCirclePairCount = 0u;
	m_polyPolyPairCount = broadPhase(m_polygonShapes, m_polygonShapes, m_polyPolyPairs, true);
	m_circleCirclePairCount = broadPhase(m_circleShapes, m_circleShapes, m_circleCirclePairs, true);
	m_polyCirclePairCount = broadPhase(m_polygonShapes, m_circleShapes, m_polyCirclePairs);
}

template<class T>
std::size_t PhysicsEngine::broadPhase(std::vector<T> const & vector, std::vector<Pair<ConvexShape *, T>> & pairs)
{
	/*
	for (std::size_t k = 0u; k < vector.size(); k++)
	{
		sf::Rect<float> const & rect = vector[k]->getGlobalBounds();
		int offsetX = static_cast<int>(rect.left / Tile::TileSize) + 2 - static_cast<int>(m_mapManager->getCameraManager().getUpLeft()->x / Tile::TileSize);
		if (rect.left < 0)
			offsetX--;
		int offsetY = static_cast<int>(rect.top / Tile::TileSize) + 2 - static_cast<int>(m_mapManager->getCameraManager().getUpLeft()->y / Tile::TileSize);
		int width = static_cast<int>(rect.width / Tile::TileSize) + 1 + offsetX;
		int height = static_cast<int>(rect.height / Tile::TileSize) + 1 + offsetY;
		if (vector[k]->getVelocity().x < 0)
		{
			for (int i = width; i >= offsetX; i--)
			{
				for (int j = height; j >= offsetY; j--)
				{
					if (i < 0 || i >= static_cast<int>(m_mapManager->getTransitionManager().getMapWidth()) || j < 0 || j >= static_cast<int>(m_mapManager->getTransitionManager().getMapHeight()))
						continue;
					Tile & tile = m_mapManager->getTransitionManager().getTile(i, j);
					if (tile.getCollisionType() & vector[k]->getCollideMask() && tile.me_transition != Tile::e_transition_none)
					{
						m_polyPolyPairs[m_polyPolyPairCount].getShapeA() = vector[k];
						m_polyPolyPairs[m_polyPolyPairCount].getShapeB() = &tile;
						m_polyPolyPairCount++;
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
					if (tile.getCollisionType() & vector[k]->getCollideMask() && tile.me_transition != Tile::e_transition_none)
					{
						m_polyPolyPairs[m_polyPolyPairCount].getShapeA() = vector[k];
						m_polyPolyPairs[m_polyPolyPairCount].getShapeB() = &tile;
						m_polyPolyPairCount++;
					}
				}
			}
		}
	}
	*/(void)vector; (void)pairs;
	return 0u;
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
}

template<class T, class U>
void PhysicsEngine::narrowPhase(std::vector<Pair<T, U>> & pairs, std::size_t pairCount)
{
	for(std::size_t i = 0u; i < pairCount; i++)
	{
		pairs[i].m_isColliding = false;
		if (computeCollision(pairs[i].m_shapeA, pairs[i].m_shapeB))
		{
			if (pairs[i].m_shapeA->getType() == AShape::Type::e_dynamic || pairs[i].m_shapeA->getType() == AShape::Type::e_kinematic)
			{
				pairs[i].m_shapeA->addVelocity(m_mtv.x / 2.f, m_mtv.y / 2.f);
				pairs[i].m_shapeB->addVelocity(-m_mtv.x / 2.f, -m_mtv.y / 2.f);
			}
			pairs[i].m_isColliding = true;
		}
	}
}

//TODO: voir avec les tiles quelle technique fonctionne le mieux
bool PhysicsEngine::resolveCollision(PolygonShape * polygonA, PolygonShape * polygonB)
{
	for (std::size_t i = 0u; i < polygonA->getEfficientVertexCount(); i++)
	{
		// Calcul du premier axe (normal du coté du polygon)
		m_axis = polygonA->getNormal(i);
		octo::normalize(m_axis);
		// TODO: Tile, remove properly same point
		/*if (polygonB->getNormal(i).x == 0 && polygonB->getNormal(i).y == 0)
		{
			continue;
		}*/
		// On fait une projection du polygonA et du polygonB sur l'axe
		m_projectionA.project(m_axis, polygonA);
		m_projectionB.project(m_axis, polygonB);
		float overlap = m_projectionA.getOverlap(m_projectionB);
		// TODO: manage containement
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
		if (overlap > 0.f)
			return false;
		if (overlap > m_magnitude)
		{
			m_magnitude = overlap;
			m_mtv = m_axis;
		}
	}
	return true;
}

bool PhysicsEngine::findAxisLeastPenetration(PolygonShape *polygonA, PolygonShape *polygonB)
{
	for(std::size_t i = 0u; i < polygonA->getEfficientVertexCount(); ++i)
	{
		//Retrieve a face normal from A
		sf::Vector2f const & n = polygonA->getNormal(i);
		// Retrieve support point from B along -n
		sf::Vector2f const & s = polygonB->getSupportVertex(-n);
		// Retrieve vertex on face from A, transform into
		// B's model space
		sf::Vector2f const & v = polygonA->getVertex(i);
		// Compute penetration distance (in B's model space)
		float d = octo::dotProduct(n, s - v);
		if (d > 0.f)
			return false;
		// Store greatest distance
		if (d > m_magnitude)
		{
			m_magnitude = d;
			m_mtv = n;
			m_debug1 = s - v;
			m_debug2 = v;
			m_debug3 = s;
		}
	}
	return true;
}

bool PhysicsEngine::computeCollision(PolygonShape * polygonA, PolygonShape * polygonB)
{
	m_magnitude = -std::numeric_limits<float>::max();
	if (!resolveCollision(polygonA, polygonB) || !resolveCollision(polygonB, polygonA))
		return false;
	// On dirige le vecteur dans la bonne direction
	if (octo::dotProduct(m_mtv, polygonB->getBaryCenter() - polygonA->getBaryCenter()) < 0.f)
		m_mtv = -m_mtv;
	m_mtv *= m_magnitude;
	// Si on arrive içi c'est que l'on a testé tous les axes, et qu'il n'existe pas d'axe qui sépare les deux polygons, donc il y a une collision
	return true;
/*
	m_magnitude = -std::numeric_limits<float>::max();
	if (!FindAxisLeastPenetration(polygonA, polygonB) || !FindAxisLeastPenetration(polygonB, polygonA))
		return false;
	octo::normalize(m_mtv);
	float d = octo::dotProduct(m_mtv, m_debug3);
	float dd = octo::dotProduct(m_mtv, m_debug2);
	m_magnitude = dd - d;
	m_mtv *= m_magnitude;
	if (octo::dotProduct(m_mtv, polygonA->getBaryCenter() - polygonB->getBaryCenter()) < 0.f)
		m_mtv = -m_mtv;
	return true;*/
}

bool PhysicsEngine::computeCollision(CircleShape * circleA, CircleShape * circleB)
{
	m_magnitude = -std::numeric_limits<float>::max();
	m_axis = circleA->getBaryCenter() - circleB->getBaryCenter();
	octo::normalize(m_axis);
	m_projectionA.project(m_axis, circleA);
	m_projectionB.project(m_axis, circleB);
	float overlap = m_projectionA.getOverlap(m_projectionB);
	if (overlap > 0.f)
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

bool PhysicsEngine::computeCollision(PolygonShape * polygon, CircleShape * circle)
{
	m_magnitude = -std::numeric_limits<float>::max();
	for (std::size_t i = 0u; i < polygon->getEfficientVertexCount(); i++)
	{
		m_axis = polygon->getNormal(i);
		octo::normalize(m_axis);
		m_projectionA.project(m_axis, polygon);
		m_projectionB.project(m_axis, circle);
		float overlap = m_projectionA.getOverlap(m_projectionB);
		if (overlap > 0.f)
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
	float overlap = m_projectionA.getOverlap(m_projectionB);
	if (overlap > 0.f)
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
	if (vel < 0.f)
		min += vel;
	else
		max += vel;
}
