#ifndef TRANSITIONMANAGER_HPP
# define TRANSITIONMANAGER_HPP

# include <Array2D.hpp>
# include "Map.hpp"

class MapManager;

class TransitionManager : public sf::Drawable
{
public:
	TransitionManager(void);
	virtual ~TransitionManager(void);

	inline sf::Vertex * getHeight(int x) { m_tiles->getHeight(x); return m_tilesPrev->getHeight(x); }
	inline Tile & getTile(int x, int y) const { return m_tiles->get(x, y); }
	inline std::size_t getMapWidth(void) const { return m_tiles->getColumns(); }
	inline std::size_t getMapHeight(void) const { return m_tiles->getRows(); }
	// Only used by StaticObjectManager to compute initial position
	inline void computeDecor(void) { m_tiles->computeDecor(); }

	void init(MapManager * p_mapManager, Biome * p_biome);
	void update(float pf_deltatime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	// Containes base value to avoid redundant calcul
	octo::Array2D<sf::Vector2f>		m_baseValue;

	MapManager *				m_mapManager;
	Map *					m_tiles;
	Map *					m_tilesPrev;
	float					mf_transitionTimer;
	float					mf_transitionTimerMax;
	float					m_offsetX;
	float					m_offsetY;
	sf::Vector2f				m_offset;
	std::unique_ptr<sf::Vertex[]>		m_vertices;
	std::size_t				mn_verticesCount;

	// Transition
	void defineTransition(void);
	void defineTransitionRange(int p_startX, int p_endX, int p_startY, int p_endY);
	void defineTransition(int x, int y);
	void defineTransitionBorderTileRange(int p_startX, int p_endX, int p_startY, int p_endY);
	void setTransitionAppear(int x, int y);
	void setTransitionDisappear(int x, int y);
	void setTransitionModify(int x, int y);
	void setTransitionFull(Map * tiles, int x, int y);

	void swapMap(void);
	void updateOffset(float pf_deltatime);
	void updateTransition(float pf_deltatime);
	void lerp(sf::Vector2f & p_result, sf::Vector2f & p_start, sf::Vector2f & p_end, float p_transition);

};

#endif
