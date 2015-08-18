#include "GroundManager.hpp"
#include "TileShape.hpp"
#include "RectangleShape.hpp"
#include "PhysicsEngine.hpp"
#include "ADecor.hpp"
#include "ABiome.hpp"
#include "Rainbow.hpp"
#include "SkyCycle.hpp"
#include "MapInstance.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <Camera.hpp>
#include <LevelMap.hpp>
#include <ResourceManager.hpp>
#include <Console.hpp>

GroundManager::GroundManager(void) :
	m_tiles(nullptr),
	m_tilesPrev(nullptr),
	m_transitionTimer(1.f),
	m_transitionTimerMax(0.4f),
	m_offset(),
	m_vertices(nullptr),
	m_verticesCount(0u),
	m_oldOffset(0, 0),
	//TODO:Estimate what we need
	m_decorManagerBack(200000),
	m_decorManagerFront(200000),
	m_decorManagerGround(200000),
	m_nextState(GenerationState::None),
	m_cycle(nullptr)
{}

void GroundManager::setup(ABiome & biome, SkyCycle & cycle)
{
	m_cycle = &cycle;

	// Init maps
	m_tiles.reset(new Map());
	m_tilesPrev.reset(new Map());
	m_tiles->init(biome);
	m_tilesPrev->init(biome);

	// Set pointer to the offset of the camera
	m_tiles->setCameraView(&m_offset);
	m_tilesPrev->setCameraView(&m_offset);

	// Init vertices
	m_vertices.reset(new sf::Vertex[m_tiles->getRows() * m_tiles->getColumns() * 4u]);

	// Init physics
	ShapeBuilder & builder = PhysicsEngine::getShapeBuilder();
	m_tileShapes.resize(m_tiles->getColumns(), nullptr);
	PhysicsEngine::getInstance().setTileMapSize(sf::Vector2i(m_tiles->getColumns(), m_tiles->getRows()));

	for (std::size_t x = 0u; x < m_tiles->getColumns(); x++)
	{
		m_tileShapes[x] = builder.createTile(x, 0u);
		m_tileShapes[x]->setVertex(&m_vertices[0u]);
		m_tileShapes[x]->setGameObject(nullptr);
	}

	m_transitionTimerMax = biome.getTransitionDuration();

	// Init decors
	setupDecors(biome);

	// Init game objects
	setupGameObjects(biome);

	swapMap();
}

void GroundManager::setupGameObjects(ABiome & biome)
{
	octo::Console&				console = octo::Application::getConsole();

	ShapeBuilder & builder = PhysicsEngine::getShapeBuilder();

	// Setup somes console commands
	console.addCommand(L"test.elevators.setRotationFactor", [this](float factor)
			{
				for(auto& elevator : m_elevators)
				{
					elevator.m_gameObject->setRotationFactor(factor);
				}
			});
	console.addCommand(L"test.elevators.setWidth", [this](float width)
			{
				for(auto& elevator : m_elevators)
				{
					elevator.m_gameObject->setWidth(width);
				}
			});
	// Get all the gameobjects from instances
	auto const & instances = biome.getInstances();
	for (auto & instance : instances)
	{
		octo::LevelMap const & levelMap = octo::Application::getResourceManager().getLevelMap(instance.second);
		for (std::size_t i = 0u; i < levelMap.getSpriteCount(); i++)
		{
			octo::LevelMap::SpriteTrigger const & spriteTrigger = levelMap.getSprite(i);
			//TODO: create NPC instead of just physics rect
			RectangleShape * rect = builder.createRectangle();
			rect->setPosition(sf::Vector2f(spriteTrigger.trigger.left + instance.first * Tile::TileSize - Map::OffsetX, (-levelMap.getMapSize().y + MapInstance::HeightOffset) * Tile::TileSize + spriteTrigger.trigger.top - Map::OffsetY));
			rect->setSize(sf::Vector2f(spriteTrigger.trigger.width, spriteTrigger.trigger.height));
			rect->setApplyGravity(false);
			rect->setType(AShape::Type::e_trigger);
		}

		// For each instance, create an elevator stream
		ElevatorStream * elevator = new ElevatorStream();
		//TODO: use correct size from elevator (static ocnstexpr in elevator ?
		elevator->setPosX((instance.first - 10.f) * Tile::TileSize);
		elevator->setPosY(-levelMap.getMapSize().y + MapInstance::HeightOffset);
		elevator->setHeight(400.f);
		elevator->setBiome(biome);
		m_elevators.emplace_back(instance.first - 10, 10, elevator);
	}

	// Register position for gameobjects on the ground
	// TODO: template function to manage this for each vector
	for (auto const & elevator : m_elevators)
	{
		for (std::size_t i = 0u; i < elevator.m_width; i++)
		{
			m_tiles->registerWideDecor(elevator.m_position + i);
			m_tilesPrev->registerWideDecor(elevator.m_position + i);
		}
	}
}

void GroundManager::setupDecors(ABiome & biome)
{
	m_decorManagerBack.setup(&biome);
	m_decorManagerFront.setup(&biome);
	m_decorManagerGround.setup(&biome);
	std::size_t mapSizeX = biome.getMapSize().x;

	std::size_t treeCount = biome.getTreeCount();
	std::size_t rainbowCount = biome.getRainbowCount();
	std::size_t rockCount = static_cast<std::size_t>(biome.getRockCount() / 2.f);
	std::size_t mushroomCount = static_cast<std::size_t>(biome.getMushroomCount() / 2.f);
	std::size_t crystalCount = static_cast<std::size_t>(biome.getCrystalCount() / 2);
	std::size_t groundRockCount = biome.getGroundRockCount();
	std::size_t totalCount = 0u;

	if (biome.canCreateRainbow())
	{
		for (std::size_t i = 0; i < rainbowCount; i++)
		{
			int x = biome.randomInt(1u, mapSizeX);
			m_decorManagerBack.add(new Rainbow(m_cycle));
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += rainbowCount;
	}

	if (biome.canCreateRock())
	{
		for (std::size_t i = 0; i < rockCount; i++)
		{
			int x = biome.randomInt(1u, mapSizeX);
			m_decorManagerBack.add(DecorManager::DecorTypes::Rock);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += rockCount;
	}

	if (biome.canCreateTree())
	{
		for (std::size_t i = 0; i < treeCount; i++)
		{
			int x = biome.randomInt(1u, mapSizeX);
			m_decorManagerBack.add(DecorManager::DecorTypes::Tree);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += treeCount;
	}

	if (biome.canCreateRock())
	{
		for (std::size_t i = 0; i < rockCount; i++)
		{
			int x = biome.randomInt(1u, mapSizeX);
			m_decorManagerBack.add(DecorManager::DecorTypes::Rock);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += rockCount;
	}

	if (biome.canCreateMushroom())
	{
		for (std::size_t i = 0; i < mushroomCount; i++)
		{
			int x = biome.randomInt(1u, mapSizeX);
			m_decorManagerBack.add(DecorManager::DecorTypes::Mushroom);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += mushroomCount;
		for (std::size_t i = 0; i < mushroomCount; i++)
		{
			int x = biome.randomInt(1u, mapSizeX);
			m_decorManagerFront.add(DecorManager::DecorTypes::Mushroom);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += mushroomCount;
	}

	if (biome.canCreateCrystal())
	{
		for (std::size_t i = 0; i < crystalCount; i++)
		{
			int x = biome.getCrystalPosX();
			m_decorManagerBack.add(DecorManager::DecorTypes::Crystal);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += crystalCount;
		for (std::size_t i = 0; i < crystalCount; i++)
		{
			int x = biome.getCrystalPosX();
			m_decorManagerFront.add(DecorManager::DecorTypes::Crystal);
			m_tiles->registerDecor(x);
			m_tilesPrev->registerDecor(x);
		}
		totalCount += crystalCount;
	}

	for (std::size_t i = 0; i < groundRockCount; i++)
	{
		int x = biome.randomInt(1u, mapSizeX);
		m_decorManagerGround.add(DecorManager::DecorTypes::GroundRock);
		m_tiles->registerDecor(x);
		m_tilesPrev->registerDecor(x);
	}
	totalCount += groundRockCount;

	m_decorPositions.resize(totalCount);
}

void GroundManager::setTransitionAppear(int x, int y)
{
	int i = 0;
	while (y + i < static_cast<int>(m_tiles->getRows() - 1) && m_tiles->get(x, y + i).isTransitionType(Tile::e_transition_appear))
		i++;
	for (std::size_t j = 0u; j < 4u; j++)
		m_tilesPrev->get(x, y).setStartTransitionY(j, m_tilesPrev->get(x, y + i).getStartTransition(j).y);
	m_tilesPrev->get(x, y).setStartColor(m_tiles->get(x, y).getStartColor());
	setTransitionModify(x, y);
}

void GroundManager::setTransitionDisappear(int x, int y)
{
	int i = 0;
	while (y + i < static_cast<int>(m_tiles->getRows() - 1) && m_tiles->get(x, y + i).isTransitionType(Tile::e_transition_disappear))
		i++;
	for (std::size_t j = 0u; j < 4u; j++)
		m_tiles->get(x, y).setStartTransitionY(j, m_tiles->get(x, y + i).getStartTransition(j).y);
	m_tiles->get(x, y).setStartColor(m_tilesPrev->get(x, y).getStartColor());
}

void GroundManager::setTransitionModify(int x, int y)
{
	// Define if it's a quad or a triangle
	if (y - 1 >= 0 && m_tiles->get(x, y - 1).isEmpty() && y + 1 < static_cast<int>(m_tiles->getRows()))
	{
		if (x - 1 >= 0 && m_tiles->get(x - 1, y).isEmpty())
			m_tiles->get(x, y).setStartTransitionY(0u, m_tiles->get(x, y + 1).getStartTransition(0u).y);
		if (x + 1 < static_cast<int>(m_tiles->getColumns()) && m_tiles->get(x + 1, y).isEmpty())
			m_tiles->get(x, y).setStartTransitionY(1u, m_tiles->get(x, y + 1).getStartTransition(1u).y);
	}
}

void GroundManager::defineTransition(void)
{
	defineTransitionRange(0, m_tiles->getColumns(), 0, m_tiles->getRows());
}

void GroundManager::defineTransition(int x, int y)
{
	int prev = m_tilesPrev->get(x, y).isEmpty();
	int current = m_tiles->get(x, y).isEmpty();

	if (prev && !current) // appear
		m_tiles->get(x, y).setTransitionType(Tile::e_transition_appear);
	else if (!prev && current) // disappear
		m_tiles->get(x, y).setTransitionType(Tile::e_transition_disappear);
	else if (!current && !prev) // already a tile
		m_tiles->get(x, y).setTransitionType(Tile::e_transition_already);
	else // no tile
		m_tiles->get(x, y).setTransitionType(Tile::e_transition_none);
}

void GroundManager::defineTransitionRange(int startX, int endX, int startY, int endY)
{
	// For each tile, define the type and transition type
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
			defineTransition(x, y);
	}
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			if (m_tiles->get(x, y).isTransitionType(Tile::e_transition_appear))
				setTransitionAppear(x, y);
		}
	}
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			if (m_tiles->get(x, y).isTransitionType(Tile::e_transition_already))
				setTransitionModify(x, y);
		}
	}
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			if (m_tiles->get(x, y).isTransitionType(Tile::e_transition_disappear))
				setTransitionDisappear(x, y);
		}
	}
}

void GroundManager::swapMap(void)
{
	m_tiles.swap(m_tilesPrev);
	m_tiles->computeMap();
	computeDecor();
	defineTransition();
}

void GroundManager::computeDecor(void)
{
	m_tiles->computeDecor();
	m_tiles->computeWideDecor();
}

void GroundManager::updateTransition(void)
{
	if (m_transitionTimer > m_transitionTimerMax)
		m_transitionTimer = m_transitionTimerMax;
	float transition = m_transitionTimer / m_transitionTimerMax;
	Tile * tile;
	Tile * tilePrev;
	TileShape * first;
	std::size_t height;
	bool isComputed;

	// Update tiles
	m_verticesCount = 0u;
	for (std::size_t x = 0u; x < m_tiles->getColumns(); x++)
	{
		first = nullptr;
		height = 0u;
		isComputed = false;
		for (std::size_t y = 0u; y < m_tiles->getRows(); y++)
		{
			tile = &m_tiles->get(x, y);
			if (tile->isTransitionType(Tile::e_transition_none))
			{
				if (first && !isComputed)
				{
					first->setHeight(static_cast<float>(height) * Tile::TileSize);
					isComputed = true;
				}
				continue;
			}
			tilePrev = &m_tilesPrev->get(x, y);

			// Update tile transition
			m_vertices[m_verticesCount].color = octo::linearInterpolation(tilePrev->getStartColor(), tile->getStartColor(), transition);
			for (std::size_t i = 0u; i < 3u; i++)
			{
				m_vertices[m_verticesCount + i].position.y = octo::linearInterpolation(tilePrev->getStartTransition(i).y, tile->getStartTransition(i).y, transition) - Map::OffsetY;
				m_vertices[m_verticesCount + i + 1u].color = m_vertices[m_verticesCount].color;
			}

			// This case is here to manage bottom transition from instance
			if (tile->getTileType() == octo::LevelMap::TileType::Square)
				m_vertices[m_verticesCount + 3u].position.y = m_vertices[m_verticesCount + 2u].position.y;
			else
				m_vertices[m_verticesCount + 3u].position.y = octo::linearInterpolation(tilePrev->getStartTransition(3u).y, tile->getStartTransition(3u).y, transition) - Map::OffsetY;

			m_vertices[m_verticesCount + 0u].position.x = tilePrev->getStartTransition(0u).x - Map::OffsetX;
			m_vertices[m_verticesCount + 1u].position.x = tilePrev->getStartTransition(1u).x - Map::OffsetX;
			m_vertices[m_verticesCount + 2u].position.x = m_vertices[m_verticesCount + 1u].position.x;
			m_vertices[m_verticesCount + 3u].position.x = m_vertices[m_verticesCount + 0u].position.x;

			// Update physics information
			if (!first)
			{
				first = m_tileShapes[x];
				m_tileShapes[x]->setVertex(&m_vertices[m_verticesCount]);
				m_tileShapes[x]->setGameObject(tilePrev);
			}
			height++;
			m_verticesCount += 4u;
		}
		if (first && !isComputed)
			first->setHeight(static_cast<float>(height) * Tile::TileSize);
	}

	// Update decors
	Map::Decors const & current = m_tiles->getDecorsPosition();
	Map::Decors const & prev = m_tilesPrev->getDecorsPosition();
	for (std::size_t i = 0u; i < m_tiles->getDecorsPosition().size(); i++)
	{
		m_decorPositions[i].y = octo::linearInterpolation(prev[i].second.y, current[i].second.y, transition);
		m_decorPositions[i].x = current[i].second.x - Tile::DoubleTileSize;
	}

	// Update wide decors
	Map::WideDecors const & currentWide = m_tiles->getWideDecorsPosition();
	Map::WideDecors const & prevWide = m_tilesPrev->getWideDecorsPosition();
	float min = 0;
	for (auto const & elevator : m_elevators)
	{
		for (std::size_t i = elevator.m_position; i < elevator.m_position + elevator.m_width; i++)
		{
			float tmp = octo::linearInterpolation(prevWide[i].second.y, currentWide[i].second.y, transition);
			if (tmp > min)
				min = tmp;
		}
		//TODO change 15 by something else (it's the height of the instance
		elevator.m_gameObject->setPosX(currentWide[elevator.m_position].second.x + Tile::DoubleTileSize);
		elevator.m_gameObject->setPosY(min);
		elevator.m_gameObject->setHeight(min - ((MapInstance::HeightOffset - 15) * Tile::TileSize));
	}
}

void GroundManager::defineTransitionBorderTileRange(int startX, int endX, int startY, int endY)
{
	for (int x = startX; x < endX; x++)
	{
		for (int y = startY; y < endY; y++)
		{
			if (y - 1 >= 0 && y + 1 < static_cast<int>(m_tiles->getRows()))
			{
				// Define triangle/quad
				if (m_tiles->get(x, y - 1).isEmpty())
				{
					if (x - 1 >= 0 && m_tiles->get(x - 1, y).isEmpty())
						m_tiles->get(x, y).setStartTransitionY(0u, m_tiles->get(x, y + 1).getStartTransition(0u).y);
					if (x + 1 < static_cast<int>(m_tiles->getColumns()) && m_tiles->get(x + 1, y).isEmpty())
						m_tiles->get(x, y).setStartTransitionY(1u, m_tiles->get(x, y + 1).getStartTransition(1u).y);
				}
				if (m_tilesPrev->get(x, y - 1).isEmpty())
				{
					if (x - 1 >= 0 && m_tilesPrev->get(x - 1, y).isEmpty())
						m_tilesPrev->get(x, y).setStartTransitionY(0u, m_tilesPrev->get(x, y + 1).getStartTransition(0u).y);
					if (x + 1 < static_cast<int>(m_tilesPrev->getColumns()) && m_tilesPrev->get(x + 1, y).isEmpty())
						m_tilesPrev->get(x, y).setStartTransitionY(1u, m_tilesPrev->get(x, y + 1).getStartTransition(1u).y);
				}
			}
		}
	}
	defineTransitionRange(startX, endX, startY, endY);
}

void GroundManager::updateOffset(float)
{
	int ofX = 0;
	int ofY = 0;
	int newOfX = static_cast<int>(m_offset.x / Tile::TileSize);
	int newOfY = static_cast<int>(m_offset.y / Tile::TileSize);
	if (m_oldOffset.x != newOfX)
		ofX = newOfX - m_oldOffset.x;
	if (m_oldOffset.y != newOfY)
		ofY = newOfY - m_oldOffset.y;

	if (ofX)
		computeDecor();
	if (ofX && ofY)
	{
		m_tiles->registerOffset();
		m_tilesPrev->registerOffset();
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetX(ofX);
		m_tilesPrev->addOffsetX(ofX);
		m_tiles->addOffsetY(ofY);
		m_tilesPrev->addOffsetY(ofY);
		if (ofX < 0)
		{
			m_tiles->computeMapRangeX(0, -ofX);
			m_tilesPrev->computeMapRangeX(0, -ofX);
			if (ofY < 0)
			{
				m_tiles->computeMapRangeY(0, -ofY);
				m_tilesPrev->computeMapRangeY(0, -ofY);
				defineTransitionBorderTileRange(0, -ofX + 1, -ofY + 1, m_tiles->getRows());
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), 0, -ofY + 1);
			}
			else
			{
				m_tiles->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
				m_tilesPrev->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
				defineTransitionBorderTileRange(0, ofY + 1, 0, m_tiles->getRows() - ofY - 1);
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), m_tiles->getRows() - ofY - 1, m_tiles->getRows());
			}
		}
		else
		{
			m_tiles->computeMapRangeX(m_tiles->getColumns() - ofX, m_tiles->getColumns());
			m_tilesPrev->computeMapRangeX(m_tiles->getColumns() - ofX, m_tiles->getColumns());
			if (ofY < 0)
			{
				m_tiles->computeMapRangeY(0, -ofY);
				m_tilesPrev->computeMapRangeY(0, -ofY);
				defineTransitionBorderTileRange(m_tiles->getColumns() - ofX - 1, m_tiles->getColumns(), -ofY + 1, m_tiles->getRows());
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), 0, -ofY + 1);
			}
			else
			{
				m_tiles->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
				m_tilesPrev->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
				defineTransitionBorderTileRange(m_tiles->getColumns() - ofX - 1, m_tiles->getColumns(), 0, m_tiles->getRows() - ofY - 1);
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), m_tiles->getRows() - ofY - 1, m_tiles->getRows());
			}
		}
		m_tilesPrev->swapDepth();
		m_oldOffset.x = newOfX;
		m_oldOffset.y = newOfY;
	}
	else if (ofX)
	{
		m_tiles->registerOffset();
		m_tilesPrev->registerOffset();
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetX(ofX);
		m_tilesPrev->addOffsetX(ofX);
		if (ofX < 0)
		{
			m_tiles->computeMapRangeX(0, -ofX);
			m_tilesPrev->computeMapRangeX(0, -ofX);
			defineTransitionBorderTileRange(0, -ofX + 1, 0, m_tiles->getRows());
		}
		else
		{
			m_tiles->computeMapRangeX(m_tiles->getColumns() - ofX, m_tiles->getColumns());
			m_tilesPrev->computeMapRangeX(m_tiles->getColumns() - ofX, m_tiles->getColumns());
			defineTransitionBorderTileRange(m_tiles->getColumns() - ofX - 1, m_tiles->getColumns(), 0, m_tiles->getRows());
		}
		m_tilesPrev->swapDepth();
		m_oldOffset.x = newOfX;
	}
	else if (ofY)
	{
		m_tiles->registerOffset();
		m_tilesPrev->registerOffset();
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetY(ofY);
		m_tilesPrev->addOffsetY(ofY);
		if (ofY < 0)
		{
			m_tiles->computeMapRangeY(0, -ofY);
			m_tilesPrev->computeMapRangeY(0, -ofY);
			defineTransitionBorderTileRange(0, m_tiles->getColumns(), 0, -ofY + 1);
		}
		else
		{
			m_tiles->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
			m_tilesPrev->computeMapRangeY(m_tiles->getRows() - ofY, m_tiles->getRows());
			defineTransitionBorderTileRange(0, m_tiles->getColumns(), m_tiles->getRows() - 10, m_tiles->getRows());
		}
		m_tilesPrev->swapDepth();
		m_oldOffset.y = newOfY;
	}
}

void GroundManager::updateDecors(sf::Time deltatime)
{
	std::size_t i = 0;
	for (auto it = m_decorManagerBack.begin(); it != m_decorManagerBack.end(); it++, i++)
		(*it)->setPosition(m_decorPositions[i]);
	for (auto it = m_decorManagerFront.begin(); it != m_decorManagerFront.end(); it++, i++)
		(*it)->setPosition(m_decorPositions[i]);
	for (auto it = m_decorManagerGround.begin(); it != m_decorManagerGround.end(); it++, i++)
		(*it)->setPosition(m_decorPositions[i]);


	octo::Camera& camera = octo::Application::getCamera();
	m_decorManagerBack.update(deltatime, camera);
	m_decorManagerFront.update(deltatime, camera);
	m_decorManagerGround.update(deltatime, camera);
}

void GroundManager::updateGameObjects(float deltatime)
{
	for (auto & elevator : m_elevators)
		elevator.m_gameObject->update(sf::seconds(deltatime));
}

void GroundManager::update(float deltatime)
{
	m_transitionTimer += deltatime;

	// Get the top left of the camera view
	sf::Rect<float> const & rect = octo::Application::getCamera().getRectangle();
	m_offset.x = rect.left;
	m_offset.y = rect.top;

	if (m_transitionTimer >= m_transitionTimerMax)
	{
		bool compute = false;
		if (m_nextState == GenerationState::Next)
		{
			compute = true;
			m_tilesPrev->nextStep();
			m_tiles->registerDepth();
			m_tiles->nextStep();
			m_nextState = GenerationState::None;
		}
		else if (m_nextState == GenerationState::Previous)
		{
			compute = true;
			m_tilesPrev->previousStep();
			m_tiles->registerDepth();
			m_tiles->previousStep();
			m_nextState = GenerationState::None;
		}
		if (compute)
		{
			m_transitionTimer = 0.f;
			swapMap();
		}
	}
	updateOffset(deltatime);
	updateTransition();
	updateDecors(sf::seconds(deltatime));
	updateGameObjects(deltatime);
	m_test.update(sf::seconds(deltatime));
}

void GroundManager::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	for (auto & elevator : m_elevators)
		elevator.m_gameObject->draw(render);
	render.draw(m_vertices.get(), m_verticesCount, sf::Quads, states);
	m_test.draw(render);
}

DecorManager const & GroundManager::getDecorsBack(void) const
{
	return m_decorManagerBack;
}

DecorManager const & GroundManager::getDecorsFront(void) const
{
	return m_decorManagerFront;
}

DecorManager const & GroundManager::getDecorsGround(void) const
{
	return m_decorManagerGround;
}
