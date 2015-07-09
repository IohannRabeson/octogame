#ifndef TERRAINMANAGER_HPP
# define TERRAINMANAGER_HPP

# include "Map.hpp"
# include "DecorManager.hpp"

class ADecor;
class ABiome;
class TileShape;
class ABiome;

class GroundManager : public sf::Drawable
{
public:
	GroundManager(void);
	virtual ~GroundManager(void) = default;

	// Only used by StaticObjectManager to compute initial position
	inline void computeDecor(void) { m_tiles->computeDecor(); }

	void init(ABiome & biome);
	void update(float pf_deltatime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;
	DecorManager const & getDecors(void) const;

private:
	std::unique_ptr<Map>				m_tiles;
	std::unique_ptr<Map>				m_tilesPrev;
	float								m_transitionTimer;
	float								m_transitionTimerMax;
	sf::Vector2f						m_offset;
	std::unique_ptr<sf::Vertex[]>		m_vertices;
	std::size_t							m_verticesCount;
	sf::Vector2i						m_oldOffset;
	std::vector<TileShape *>			m_tileShapes;
	std::vector<sf::Vector2f>			m_decorPositions;
	DecorManager						m_decorManager;

	inline void defineTransition(void) { defineTransitionRange(0, m_tiles->getColumns(), 0, m_tiles->getRows()); }
	void defineTransitionRange(int startX, int endX, int startY, int endY);
	void defineTransition(int x, int y);
	void defineTransitionBorderTileRange(int startX, int endX, int startY, int endY);
	void setTransitionAppear(int x, int y);
	void setTransitionDisappear(int x, int y);
	void setTransitionModify(int x, int y);

	void initDecors(ABiome & biome);
	void updateDecors(float pf_deltatime);
	void swapMap(void);
	void updateOffset(float pf_deltatime);
	void updateTransition(void);

};

#endif
