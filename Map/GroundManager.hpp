#ifndef TERRAINMANAGER_HPP
# define TERRAINMANAGER_HPP

# include <AudioManager.hpp>
# include <GenericFactory.hpp>
# include <Array2D.hpp>
# include "Map.hpp"
# include "DecorManager.hpp"
# include "Portal.hpp"
# include "ElevatorStream.hpp"
# include "NanoRobot.hpp"
# include "ANpc.hpp"
# include "InstanceDecor.hpp"
# include "IPlaceable.hpp"
# include "Water.hpp"

class ADecor;
class ABiome;
class TileShape;
class ABiome;
class SkyCycle;

class GroundManager
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
	void setupGroundRock(ABiome & biome);
	void update(float deltatime);
	void drawBack(sf::RenderTarget& render, sf::RenderStates states) const;
	void drawFront(sf::RenderTarget& render, sf::RenderStates states) const;
	void drawWater(sf::RenderTarget& render, sf::RenderStates states) const;
	void drawText(sf::RenderTarget& render, sf::RenderStates states) const;
	NanoRobot * getNanoRobot(NanoRobot * robot);
	void setNextGenerationState(GenerationState state, sf::Vector2f const & octoPos);

private:
	template<class T>
	struct GameObjectPosition
	{
		std::size_t							m_position;
		std::size_t							m_width;
		std::unique_ptr<T>					m_gameObject;

		GameObjectPosition(std::size_t position, std::size_t width, std::unique_ptr<T> & gameObject) :
			m_position(position),
			m_width(width),
			m_gameObject(nullptr)
		{
			m_gameObject = std::move(gameObject);
		}

		GameObjectPosition(std::size_t position, std::size_t width, T * gameObject) :
			m_position(position),
			m_width(width),
			m_gameObject(nullptr)
		{
			m_gameObject.reset(gameObject);
		}
	};

	typedef octo::GenericFactory<std::string, ANpc>				NpcFactory;

	NpcFactory								m_npcFactory;
	std::unique_ptr<Map>					m_tiles;
	std::unique_ptr<Map>					m_tilesPrev;
	float									m_transitionTimer;
	float									m_transitionTimerMax;
	sf::Vector2f							m_offset;
	std::unique_ptr<sf::Vertex[]>			m_vertices;
	std::size_t								m_verticesCount;
	sf::Vector2i							m_oldOffset;
	sf::Vector2u							m_mapSize;
	octo::Array2D<TileShape *>				m_tileShapes;
	std::vector<sf::Vector2f>				m_decorPositions;
	DecorManager							m_decorManagerBack;
	DecorManager							m_decorManagerFront;
	DecorManager							m_decorManagerGround;
	DecorManager							m_decorManagerInstanceBack;
	DecorManager							m_decorManagerInstanceFront;
	DecorManager							m_decorManagerInstanceGround;
	GenerationState							m_nextState;
	std::unique_ptr<Water>					m_water;
	sf::Vector2f							m_octoPosState;

	// Game objects
	std::vector<GameObjectPosition<ElevatorStream>>		m_elevators;
	std::vector<GameObjectPosition<Portal>>				m_portals;
	std::vector<GameObjectPosition<NanoRobot>>			m_nanoRobots;
	std::vector<GameObjectPosition<ANpc>>				m_npcsOnFloor;
	std::vector<GameObjectPosition<IPlaceable>>			m_otherObjectsHigh;
	std::vector<GameObjectPosition<IPlaceable>>			m_otherObjectsLow;
	std::vector<std::unique_ptr<InstanceDecor>>			m_instanceDecors;
	std::vector<std::unique_ptr<InstanceDecor>>			m_instanceDecorsFront;
	std::vector<std::unique_ptr<Portal>>				m_otherOnInstance;
	std::vector<std::unique_ptr<ANpc>>					m_npcs;
	std::vector<std::unique_ptr<NanoRobot>>				m_nanoRobotOnInstance;

	template<class T>
	void placeMax(std::vector<GameObjectPosition<T>> & objects, Map::WideDecors const & currentDecors, Map::WideDecors const & prevDecors, float transition);

	template<class T>
	void placeMin(std::vector<GameObjectPosition<T>> & objects, Map::WideDecors const & currentDecors, Map::WideDecors const & prevDecors, float transition);

	void defineTransition(void);
	void defineTransitionRange(int startX, int endX, int startY, int endY);
	void defineTransition(int x, int y);
	void defineTransitionBorderTileRange(int startX, int endX, int startY, int endY);
	void setTransitionAppear(int x, int y);
	void setTransitionDisappear(int x, int y);
	void setTransitionModify(int x, int y);

	template<class T>
	void setupGameObjectPosition(std::vector<GameObjectPosition<T>> const & gameObjectPosition);
	void setupDecors(ABiome & biome, SkyCycle & cycle);
	void setupGameObjects(ABiome & biome, SkyCycle & cycle);
	void updateOffset(float deltatime);
	void updateTransition(sf::FloatRect const & cameraRect);
	void updateDecors(sf::Time deltatime);
	void updateGameObjects(sf::Time deltatime);
	void computeDecor(void);
	void swapMap(void);

};

#endif
