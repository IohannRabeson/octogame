#ifndef PHYSICSENGINE_HPP
# define PHYSICSENGINE_HPP

# include <SFML/Graphics.hpp>
# include <vector>
# include <memory>

# include "ShapeBuilder.hpp"

# include <Array2D.hpp>

class AShape;
class PolygonShape;
class CircleShape;
class TileShape;
class IContactListener;

/*!
 * \ingroup Physic
 * \class PhysicsEngine
 * Physics Engine, compute collision between registered objects
 */
class PhysicsEngine : public ShapeBuilder
{
public:
	virtual ~PhysicsEngine(void);

	static PhysicsEngine & getInstance(void);
	static ShapeBuilder & getShapeBuilder(void);

	void init(void);

	/*! Set the gravity */
	inline void setGravity(sf::Vector2f const & gravity) { m_gravity = gravity; }

	/*! Get the gravity */
	inline sf::Vector2f const & getGravity(void) const { return m_gravity; }

	/*! Set the count of iteration
	 * The iteration count is only used to compute collision between shapes and tiles
	 * Such precision isn't needed between others polygon
	 *
	 * \param iterationCount The count of iteration
	 */
	inline void setIterationCount(std::size_t iterationCount) { m_iterationCount = iterationCount; }
	inline std::size_t getIterationCount(void) const { return m_iterationCount; }

	/*! Set whether the tile collision is activated or not */
	inline void setTileCollision(bool tileCollision) { m_tileCollision = tileCollision; }
	inline bool getTileCollision(void) const { return m_tileCollision; }

	/*! Used by the ShapeBuilder to register a PolygonShape */
	void registerShape(PolygonShape * shape);

	/*! Used by the ShapeBuilder to register a CircleShape */
	void registerShape(CircleShape * shape);

	/*! Used by the ShapeBuilder to register a TileShape */
	void registerTileS(TileShape * shape, std::size_t x, std::size_t y);

	/*! Update the physic */
	void update(float deltatime);

	/*! Set the ContactListener
	 * The contact listener is call each time there is a collision between two object
	 *
	 * \param contactListener The new ContactListener
	 * \see IContactListener
	 */
	inline void setContactListener(IContactListener * contactListener) { m_contactListener = contactListener; }

	/*! Draw debug information */
	void debugDraw(sf::RenderTarget & render) const;

private:
	PhysicsEngine(void);

	class Projection
	{
	public:
		void project(sf::Vector2f const & axis, PolygonShape * polygon);
		void project(sf::Vector2f const & axis, CircleShape * circle);
		float getOverlap(Projection const & projection);
		bool contains(Projection const & projection);
		void swept(sf::Vector2f const & axis, AShape const * shape);

	private:
		float	min;
		float	max;
	};

	//TODO: std::pair
	template<class T, class U>
	struct Pair
	{
		T				m_shapeA;
		U				m_shapeB;
	};

	static constexpr std::size_t						MaxShapes = 1000u;

	static std::unique_ptr<PhysicsEngine>				m_instance;

	// Vectors containing all the shapes
	std::vector<AShape *>								m_shapes;

	// Vectors containing shapes by type to improve performance during collision detection
	std::vector<CircleShape *>							m_circleShapes;
	std::vector<PolygonShape *>							m_polygonShapes;
	octo::Array2D<TileShape *>							m_tileShapes;

	// Pairs of object which might be colliding
	std::vector<Pair<PolygonShape *, PolygonShape *>>	m_polyPolyPairs;
	std::vector<Pair<PolygonShape *, CircleShape *>>	m_polyCirclePairs;
	std::vector<Pair<CircleShape *, CircleShape *>>		m_circleCirclePairs;
	std::size_t											m_polyPolyPairCount;
	std::size_t											m_polyCirclePairCount;
	std::size_t											m_circleCirclePairCount;

	// Pairs of object which might be colliding with the tile map
	std::vector<Pair<TileShape *, PolygonShape *>>		m_tilePolyPairs;
	std::vector<Pair<TileShape *, CircleShape *>>		m_tileCirclePairs;
	std::size_t											m_tilePolyPairCount;
	std::size_t											m_tileCirclePairCount;

	// Variable used to compute collisions (to avoid creation and copy)
	Projection											m_projectionA;
	Projection											m_projectionB;
	sf::Vector2f										m_axis;
	sf::Vector2f										m_mtv;		/// Minimum Translation Vector
	sf::Vector2f										m_gravity;
	float												m_magnitude;
	std::size_t											m_iterationCount;
	bool												m_tileCollision;

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

	/*! Broadphase for tiles */
	template<class T>
	std::size_t broadPhase(std::vector<T> const & vector, std::vector<Pair<TileShape *, T>> & pairs);

	/*! Determine if pairs are colliding */
	void narrowPhase(void);

	/*! Determine if pairs are colliding */
	template<class T, class U>
	void narrowPhase(std::vector<Pair<T, U>> & pairs, std::size_t pairCount);

	/*! Determine if pairs are colliding with tiles */
	template<class T, class U>
	void narrowPhaseTile(std::vector<Pair<T, U>> & pairs, std::size_t pairCount);

	/*! Compute collision between different shape */
	bool computeCollision(PolygonShape * polygonA, PolygonShape * polygonB);
	bool computeCollision(PolygonShape * polygon, CircleShape * circle);
	bool computeCollision(CircleShape * circleA, CircleShape * circleB);
	bool computeCollision(TileShape * tile, PolygonShape * polygon);
	bool computeCollision(TileShape * tile, CircleShape * polygon);
	bool findAxisLeastPenetration(PolygonShape * polygonA, PolygonShape * polygonB);
	bool findAxisLeastPenetration(TileShape * tile, PolygonShape * polygon);

	IContactListener *		m_contactListener;

};

#endif
