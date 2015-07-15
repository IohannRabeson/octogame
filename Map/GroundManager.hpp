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
	enum GenerationState
	{
		None,
		Next,
		Previous
	};

	GroundManager(void);
	virtual ~GroundManager(void) = default;

	void init(ABiome & biome);
	void update(float deltatime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;
	DecorManager const & getDecorsBack(void) const;
	DecorManager const & getDecorsFront(void) const;

	void computeDecor(void);
	inline void setNextGenerationState(GenerationState state) { m_nextState = state; }

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
	DecorManager						m_decorManagerBack;
	DecorManager						m_decorManagerFront;
	GenerationState						m_nextState;

	void defineTransition(void);
	void defineTransitionRange(int startX, int endX, int startY, int endY);
	void defineTransition(int x, int y);
	void defineTransitionBorderTileRange(int startX, int endX, int startY, int endY);
	void setTransitionAppear(int x, int y);
	void setTransitionDisappear(int x, int y);
	void setTransitionModify(int x, int y);

	void initDecors(ABiome & biome);
	void updateDecors(float deltatime);
	void updateOffset(float deltatime);
	void updateTransition(void);
	void swapMap(void);

};

#endif
