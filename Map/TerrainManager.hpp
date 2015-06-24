#ifndef TERRAINMANAGER_HPP
# define TERRAINMANAGER_HPP

# include "Map.hpp"

class MapManager;
class ConvexShape;
class TileShape;

class TerrainManager : public sf::Drawable
{
public:
	TerrainManager(void);
	virtual ~TerrainManager(void);

	inline sf::Vertex * getHeight(int x) { m_tiles->getHeight(x); return m_tilesPrev->getHeight(x); }
	inline Tile & getTile(int x, int y) const { return m_tiles->get(x, y); }
	inline std::size_t getMapWidth(void) const { return m_tiles->getColumns(); }
	inline std::size_t getMapHeight(void) const { return m_tiles->getRows(); }
	inline sf::Vector2f const & getOffset(void) const { return m_offset; }
	// TODO: delete ?
	inline int getMapOffsetX(void) const { return static_cast<int>(m_offset.x); }
	// Only used by StaticObjectManager to compute initial position
	inline void computeDecor(void) { m_tiles->computeDecor(); }

	void init(MapManager * p_mapManager, Biome * p_biome);
	void update(float pf_deltatime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

	//TODO: rename variables
private:
	// TODO: smart ptr
	Map *							m_tiles;
	Map *							m_tilesPrev;
	float							mf_transitionTimer;
	float							mf_transitionTimerMax;
	sf::Vector2f					m_offset;
	std::unique_ptr<sf::Vertex[]>	m_vertices;
	std::size_t						mn_verticesCount;
	sf::Vector2i					m_oldOffset;
	octo::Array2D<TileShape *>		m_tileShapes;

	// Transition
	inline void defineTransition(void) { defineTransitionRange(0, m_tiles->getColumns(), 0, m_tiles->getRows()); }
	void defineTransitionRange(int p_startX, int p_endX, int p_startY, int p_endY);
	void defineTransition(int x, int y);
	void defineTransitionBorderTileRange(int p_startX, int p_endX, int p_startY, int p_endY);
	void setTransitionAppear(int x, int y);
	void setTransitionDisappear(int x, int y);
	void setTransitionModify(int x, int y);

	void swapMap(void);
	void updateOffset(float pf_deltatime);
	void updateTransition(void);

};

#endif
