#ifndef PHYSICSENGINE_HPP
# define PHYSICSENGINE_HPP

# include <SFML/Graphics.hpp>
# include <vector>

class AShape;
class PolygonShape;
class Circle;
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
	void registerShape(AShape * shape);

	inline void setContactListener(IContactListener * contactListener) { m_contactListener = contactListener; }

	void debugDraw(sf::RenderTarget & render) const;

private:
	struct Projection
	{
		float	min;
		float	max;

		void project(sf::Vector2f const & axis, PolygonShape * polygon);
		bool overlap(Projection const & projection);
		float getOverlap(Projection const & projection);
		bool contains(Projection const & projection);
	};

	struct Pair
	{
		PolygonShape *		m_polygonA = nullptr;
		PolygonShape *		m_polygonB = nullptr;
		bool			m_isColliding = false;
	};

	// Pairs of object which might be colliding
	std::vector<Pair>		m_pairs;
	std::size_t			m_pairCount;

	// Variable used to compute collisions
	Projection			m_projectionA;
	Projection			m_projectionB;
	sf::Vector2f			m_axis;
	sf::Vector2f			m_edge;
	sf::Vector2f			m_mtv;
	float				m_magnitude;

	// Determine which pairs of objects might be colliding
	void broadPhase(void);
	// Determine if pairs are colliding
	void narrowPhase(void);

	bool computeCollision(Circle * circle, PolygonShape * polygon);
	bool computeCollision(Circle * circleA, Circle * circleB);
	bool computeCollision(PolygonShape * polygonA, PolygonShape * polygonB);
	bool test1(PolygonShape * polygonA, PolygonShape * polygonB);
	bool test2(PolygonShape * polygonA, PolygonShape * polygonB);

	// TODO: use smart ptr
	IContactListener *		m_contactListener;

	static constexpr std::size_t	MaxShapes = 1000u;
	std::vector<AShape *>		m_shapes;

	sf::Vector2f			m_gravity;

};

#endif
