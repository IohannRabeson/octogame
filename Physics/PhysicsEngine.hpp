#ifndef PHYSICSENGINE_HPP
# define PHYSICSENGINE_HPP

# include <SFML/Graphics.hpp>
# include <vector>
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

	inline void setGravity(sf::Vector2f const & gravity) { m_gravity = gravity; }
	inline sf::Vector2f const & getGravity(void) const { return m_gravity; }

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
		void project(sf::Vector2f const & axis, sf::Vector2f const & center, sf::Vector2f const & support);
		float getOverlap(Projection const & projection);
		bool contains(Projection const & projection);
	};

	template<class T, class U>
	struct Pair
	{
		T		m_shapeA = nullptr;
		U		m_shapeB = nullptr;
		bool		m_isColliding = false;
	};

	static constexpr std::size_t	MaxShapes = 1000u;
	// Vectors containing all the shapes
	std::vector<AShape *>					m_shapes;

	// Vectors containing shapes by type to improve performance during collision detection
	std::vector<CircleShape *>				m_circleShapes;
	std::vector<PolygonShape *>				m_polygonShapes;
	octo::Array2D<PolygonShape *>				m_tileShapes;

	// Pairs of object which might be colliding
	std::vector<Pair<PolygonShape *, PolygonShape *>>	m_polyPolyPairs;
	std::vector<Pair<PolygonShape *, CircleShape *>>	m_polyCirclePairs;
	std::vector<Pair<CircleShape *, CircleShape *>>		m_circleCirclePairs;
	std::size_t						m_polyPolyPairCount;
	std::size_t						m_polyCirclePairCount;
	std::size_t						m_circleCirclePairCount;

	// Variable used to compute collisions (to avoid creation and copy)
	Projection						m_projectionA;
	Projection						m_projectionB;
	sf::Vector2f						m_axis;
	sf::Vector2f						m_edge;
	sf::Vector2f						m_mtv;
	sf::Vector2f						m_support;
	sf::Vector2f						m_gravity;
	float							m_magnitude;

	/*! Determine which pairs of objects might be colliding */
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

	/*! Determine if pairs are colliding */
	void narrowPhase(void);

	/*! Determine if pairs are colliding */
	template<class T, class U>
	void narrowPhase(std::vector<Pair<T, U>> & pairs, std::size_t pairCount);

	/*! Compute collision between different shape */
	bool resolveCollision(PolygonShape * polygonA, PolygonShape * polygonB);
	bool computeCollision(PolygonShape * polygonA, PolygonShape * polygonB);
	bool computeCollision(PolygonShape * polygon, CircleShape * circle);
	bool computeCollision(CircleShape * circleA, CircleShape * circleB);
	bool FindAxisLeastPenetration(PolygonShape *polygonA, PolygonShape *polygonB);

	// TODO: use smart ptr
	IContactListener *		m_contactListener;

	//TODO: delete
	sf::Vector2f m_debug;
	sf::Vector2f m_debug1;
	sf::Vector2f m_debug2;
	sf::Vector2f m_debug3;

};

#endif
