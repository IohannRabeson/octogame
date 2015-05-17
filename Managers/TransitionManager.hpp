#ifndef TRANSITIONMANAGER_HPP
# define TRANSITIONMANAGER_HPP

# include "FactoryMap.hpp"

class MapManager;

class TransitionManager : public sf::Drawable
{
public:
	TransitionManager(void);
	virtual ~TransitionManager(void);

	void init(MapManager * p_mapManager, Biome * p_biome, Map::EMapType p_mapType);
	sf::Vertex * getHeight(int x);
	Tile & getTile(int x, int y) const;
	float getOffsetX(void) const;
	float getOffsetY(void) const;
	std::size_t getMapWidth(void) const;
	std::size_t getMapHeight(void) const;
	int getMapOffsetX(void) const;
	// Only used by StaticObjectManager to compute initial position
	void computeDecor(void);
	void update(float pf_deltatime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	// Containes base value to avoid redundant calcul
	static sf::Vector2f **				ms_baseValue;

	MapManager *					m_mapManager;
	Map *						m_tiles;
	Map *						m_tilesPrev;
	FactoryMap					m_factoryMap;
	float						mf_transitionTimer;
	float						mf_transitionTimerMax;
	float						mf_offsetX;
	float						mf_offsetY;
	bool						mb_isInit;
	std::unique_ptr<sf::Vertex[]>			m_vertices;
	unsigned int					mn_verticesCount;

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
