#ifndef COLLISIONMANAGER_HPP
# define COLLISIONMANAGER_HPP
/*
# include <SFML/Graphics.hpp>

class MapManager;
class Player;
class Polygon;
class DynamicPolygon;

class CollisionManager
{
public:
	CollisionManager(void);
	virtual ~CollisionManager(void);

	inline Player & getPlayer(void) const { return *m_player; }
	void init(MapManager * m_mapManager);
	void update(float pf_deltatime);
	void debugDraw(sf::RenderTarget & render) const;

private:
	struct Projection
	{
		float	min;
		float	max;

		void project(sf::Vector2f const & axis, Polygon * polygon);
		bool overlap(Projection const & projection);
		float getOverlap(Projection const & projection);
		bool contains(Projection const & projection);
	};

	struct Pair
	{
		DynamicPolygon *	m_polygonA = nullptr;
		Polygon *		m_polygonB = nullptr;
		bool			m_isColliding = false;
	};

	// Owner
	MapManager *			m_mapManager;

	// Contains the player
	std::vector<DynamicPolygon *>	m_dynamicPolygons;
	Player *			m_player;

	// Pairs of object which might be colliding
	std::vector<Pair>		m_pairs;
	std::size_t			m_pairCount;

	// Variable used to compute collisions
	Projection			m_projectionA;
	Projection			m_projectionB;
	sf::Vector2f			m_axis;
	sf::Vector2f			m_edge;
	sf::Vector2f			m_mtv;

	// World variable
	sf::Vector2f			m_gravity;

	// Determine which pairs of objects might be colliding
	void broadPhase(void);
	// Determine if pairs are colliding
	void narrowPhase(void);
	bool computeCollision(DynamicPolygon * polygonA, Polygon * polygonB);

};
*/
#endif
