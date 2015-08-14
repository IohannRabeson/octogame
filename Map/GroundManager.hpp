#ifndef TERRAINMANAGER_HPP
# define TERRAINMANAGER_HPP

# include "Map.hpp"
# include "DecorManager.hpp"
# include "Portal.hpp"
# include "ElevatorStream.hpp"

class ADecor;
class ABiome;
class TileShape;
class ABiome;
class SkyCycle;

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

	void setup(ABiome & biome, SkyCycle & cycle);
	void update(float deltatime);
	void draw(sf::RenderTarget& render, sf::RenderStates states) const;
	DecorManager const & getDecorsBack(void) const;
	DecorManager const & getDecorsFront(void) const;
	DecorManager const & getDecorsGround(void) const;

	inline void setNextGenerationState(GenerationState state) { m_nextState = state; }

private:
	template<class T>
	struct GameObjectPosition
	{
		std::size_t				m_position;
		std::size_t				m_width;
		std::unique_ptr<T>		m_gameObject;

		GameObjectPosition(std::size_t position, std::size_t width, T * gameObject) :
			m_position(position),
			m_width(width),
			m_gameObject(nullptr)
		{
			m_gameObject.reset(gameObject);
		}
	};

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
	DecorManager						m_decorManagerGround;
	GenerationState						m_nextState;
	SkyCycle *							m_cycle;
	//TODO: delete
	Portal			m_test;

	// Game objects
	std::vector<GameObjectPosition<ElevatorStream>>		m_elevators;

	void defineTransition(void);
	void defineTransitionRange(int startX, int endX, int startY, int endY);
	void defineTransition(int x, int y);
	void defineTransitionBorderTileRange(int startX, int endX, int startY, int endY);
	void setTransitionAppear(int x, int y);
	void setTransitionDisappear(int x, int y);
	void setTransitionModify(int x, int y);

	void setupDecors(ABiome & biome);
	void setupGameObjects(ABiome & biome);
	void updateOffset(float deltatime);
	void updateTransition(void);
	void updateDecors(sf::Time deltatime);
	void updateGameObjects(float deltatime);
	void computeDecor(void);
	void swapMap(void);

};

#endif
