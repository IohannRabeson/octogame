#ifndef COLLISIONMANAGER_HPP
# define COLLISIONMANAGER_HPP

# include <SFML/Graphics.hpp>
# include "Player.hpp"

class MapManager;

class CollisionManager
{
public:
	CollisionManager(void);
	virtual ~CollisionManager(void);

	Player & getPlayer(void) const;
	void init(MapManager * m_mapManager);
	void update(float pf_deltatime);

private:
	struct Projection
	{
		float	min;
		float	max;

		void project(sf::Vector2f const & axis, Polygon * polygon);
		bool overlap(Projection const & projection);
		float getOverlap(Projection const & projection);
	};

	//TODO: std::pair
	struct Pair
	{
		DynamicPolygon *	m_polygonA;
		Polygon *		m_polygonB;
	};

	// Owner
	std::vector<DynamicPolygon *>	m_dynamicPolygons;
	Player *			m_player;

	std::vector<Pair>		m_pairs;
	std::size_t			m_pairCount;
	MapManager *			m_mapManager;
	Projection			m_projectionA;
	Projection			m_projectionB;
	sf::Vector2f			m_axis;
	sf::Vector2f			m_edge;
	sf::Vector2f			m_mtv;

	// Determine which pairs of objects might be colliding
	void broadPhase(void);
	// Dtermine if pairs are colliding
	void narrowPhase(void);
	bool computeCollision(DynamicPolygon * polygonA, Polygon * polygonB);

};

#endif
