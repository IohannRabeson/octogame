#ifndef PHYSICSENGINE_HPP
# define PHYSICSENGINE_HPP

# include <SFML/Graphics.hpp>
# include <vector>
# include <cassert>
# include "Array2D.hpp"

class AShape;
class PolygonShape;
class CircleShape;
class IContactListener;

/*!
 * \ingroup Physic
 * \class PhysicsEngine
 * Physics Engine, compute collision between registered objects
 */
class PhysicsEngine
{
public:
	PhysicsEngine(void);
	virtual ~PhysicsEngine(void) = default;

	void init(void);

	void update(float deltatime);
	void registerShape(PolygonShape * shape);
	void registerShape(CircleShape * shape);
	void registerTile(PolygonShape * shape, int x, int y);

	inline void setContactListener(IContactListener * contactListener) { m_contactListener = contactListener; }

	void debugDraw(sf::RenderTarget & render) const;

private:
	struct Projection
	{
		float	min;
		float	max;

		void project(sf::Vector2f const & axis, PolygonShape * polygon);
		void project(sf::Vector2f const & axis, CircleShape * circle);
		bool overlap(Projection const & projection);
		float getOverlap(Projection const & projection);
		bool contains(Projection const & projection);
	};

	template<class T, class U>
	class Pair
	{
	public:
		void setPair(T shapeA, U shapeB)
		{
			//TODO: ask io
			//static_assert(std::is_base_of<AShape, T>::value, "T must be derived from AShape");
			//static_assert(std::is_base_of<AShape, U>::value, "U must be derived from AShape");
			m_shapeA = shapeA;
			m_shapeB = shapeB;
		}

		bool isColliding(void) const { return m_isColliding; }
		void setColliding(bool isColliding) { m_isColliding = isColliding; }
		T getShapeA(void) const { return m_shapeA; }
		U getShapeB(void) const { return m_shapeB; }

	private:
		T		m_shapeA = nullptr;
		U		m_shapeB = nullptr;
		bool		m_isColliding = false;
	};

	static constexpr std::size_t	MaxShapes = 1000u;
	std::vector<AShape *>		m_shapes;
	std::vector<CircleShape *>	m_circleShapes;
	std::vector<PolygonShape *>	m_polygonShapes;
	octo::Array2D<PolygonShape *>	m_tileShapes;

	// Pairs of object which might be colliding
	std::vector<Pair<PolygonShape *, PolygonShape *>>	m_polyPolyPairs;
	std::vector<Pair<PolygonShape *, CircleShape *>>	m_polyCirclePairs;
	std::vector<Pair<CircleShape *, CircleShape *>>		m_circleCirclePairs;
	std::size_t			m_pairCount;
	std::size_t			m_cpairCount;

	// Variable used to compute collisions
	Projection			m_projectionA;
	Projection			m_projectionB;
	sf::Vector2f			m_axis;
	sf::Vector2f			m_edge;
	sf::Vector2f			m_mtv;
	sf::Vector2f			m_support;
	sf::Vector2f			m_gravity;
	float				m_magnitude;

	// Determine which pairs of objects might be colliding
	void broadPhase(void);

	/*! Compare all the AShape in the vectorA with all the AShape in the vectorB,
	 * and check for AABB collisions
	 * If they are colliding store both shape in the pairs given as parameter
	 * Set cullingDuplicate at true if you use this method with the same vector as vectorA and vectorB
	 *
	 * \return The number of AABB collisions
	 */
	template<class T, class U>
	std::size_t broadPhase(std::vector<T> const & vectorA, std::vector<U> const & vectorB, std::vector<Pair<T, U>> & pairs, bool cullingDuplicate = false);

	// Determine if pairs are colliding
	void narrowPhase(void);
	template<class T, class U>
	void narrowPhase(std::vector<Pair<T, U>> & pairs, std::size_t pairCount);

	bool computeCollision(PolygonShape * polygonA, PolygonShape * polygonB);
	bool computeCollision(PolygonShape * polygonA, CircleShape * polygonB);
	bool resolveCollision(PolygonShape * polygonA, PolygonShape * polygonB);
	sf::Vector2f const & getSupportVertex(PolygonShape * polygon, sf::Vector2f const & direction);

	// TODO: use smart ptr
	IContactListener *		m_contactListener;

};

#endif
