#include "Map.hpp"
#include "OctoNoise.hpp"
#include "ABiome.hpp"
#include "MapInstance.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Interpolations.hpp>
#include <cassert>

Map::Map(void) :
	m_depth(10000.f),
	m_oldDepth(0.f),
	m_width(0u),
	m_height(0u),
	m_offset(nullptr),
	m_mapSurface(nullptr),
	m_decorTileCount(0u)
{}

Map::~Map(void)
{
	for (auto it = m_tiles.begin(); it != m_tiles.end(); it++)
		delete *it;
}

void Map::init(ABiome & biome)
{
	m_mapSize = biome.getMapSize();

	m_width = octo::Application::getGraphicsManager().getVideoMode().width / Tile::TileSize + 4u; // 4 tiles to add margin at left and right
	m_height  = octo::Application::getGraphicsManager().getVideoMode().height / Tile::TileSize + 6u; // 6 tiles to add margin at top and bottom

	// init vertices to place decors
	m_vertices.reset(new sf::Vertex[MaxDecor]);

	m_tiles.resize(m_width, m_height, nullptr);
	for (std::size_t x = 0; x < m_tiles.columns(); x++)
	{
		for (std::size_t y = 0; y < m_tiles.rows(); y++)
		{
			m_tiles.get(x, y) = new Tile();
			m_tiles.get(x, y)->setStartTransition(0u, sf::Vector2f(x * Tile::TileSize, y * Tile::TileSize));
			m_tiles.get(x, y)->setStartTransition(1u, sf::Vector2f(x * Tile::TileSize + Tile::TileSize, y * Tile::TileSize));
			m_tiles.get(x, y)->setStartTransition(2u, sf::Vector2f(x * Tile::TileSize + Tile::TileSize, y * Tile::TileSize + Tile::TileSize));
			m_tiles.get(x, y)->setStartTransition(3u, sf::Vector2f(x * Tile::TileSize, y * Tile::TileSize + Tile::TileSize));
		}
	}

	m_instances.push_back(std::unique_ptr<MapInstance>(new MapInstance(6u, 6u, 5u)));
	for (auto & instance : m_instances)
		instance->load();

	m_noise.setSeed(42);

	// Initialize mapSurface pointer
	m_mapSurface = [this](float x, float y)
	{
		return this->m_noise.fBm(x, y, 3, 3.f, 0.3f);
	};
}

void Map::computeMapRange(int startX, int endX, int startY, int endY)
{
	float vec[3];
	int height; // The height of the generated map
	int offsetX; // The tile position adjust to avoid negativ offset (because map is circular)
	int offsetY;
	int curOffsetX = static_cast<int>(m_curOffset.x / Tile::TileSize);
	int curOffsetY = static_cast<int>(m_curOffset.y / Tile::TileSize);
	int offsetPosX; // The real position of the tile (in the world)
	MapInstance * curInstance;

	assert(m_depth >= 0.f);
	assert(m_mapSurface);

	for (int x = startX; x < endX; x++)
	{
		curInstance = nullptr;
		offsetX = x + curOffsetX;
		offsetPosX = offsetX;
		while (offsetX < 0)
			offsetX += m_mapSize.x;
		while (offsetX >= static_cast<int>(m_mapSize.x))
			offsetX -= m_mapSize.x;
		vec[0] = static_cast<float>(offsetX) / static_cast<float>(m_mapSize.x);
		vec[1] = m_depth / static_cast<float>(m_mapSize.y);

		for (auto & instance : m_instances)
		{
			if (offsetX >= static_cast<int>(instance->getCornerPositions().left) && offsetX < static_cast<int>(instance->getCornerPositions().width))
			{
				curInstance = instance.get();
				break;
			}
		}
		// mapSurface return a value between -1 & 1
		// we normalize it betwen 0 & max_height
		height = static_cast<int>((m_mapSurface(vec[0], vec[1]) + 1.f) * static_cast<float>(m_mapSize.y) / 2.f);
		for (int y = startY; y < endY; y++)
		{
			offsetY = y + curOffsetY;
			// Init square
			m_tiles.get(x, y)->setStartTransition(0u, sf::Vector2f((offsetPosX) * Tile::TileSize, (offsetY) * Tile::TileSize));
			m_tiles.get(x, y)->setStartTransition(1u, sf::Vector2f((offsetPosX + 1) * Tile::TileSize, (offsetY) * Tile::TileSize));
			m_tiles.get(x, y)->setStartTransition(2u, sf::Vector2f((offsetPosX + 1) * Tile::TileSize, (offsetY + 1) * Tile::TileSize));
			m_tiles.get(x, y)->setStartTransition(3u, sf::Vector2f((offsetPosX) * Tile::TileSize, (offsetY + 1) * Tile::TileSize));
			if (curInstance && offsetY >= static_cast<int>(curInstance->getCornerPositions().top) && offsetY < static_cast<int>(curInstance->getCornerPositions().height))
			{
				m_tiles.get(x, y)->setIsEmpty(curInstance->get(offsetX - curInstance->getCornerPositions().left, offsetY - curInstance->getCornerPositions().top).isEmpty());
			}
			else if (offsetY < height)
			{
				m_tiles.get(x, y)->setIsEmpty(true);
				continue;
			}
			else
				m_tiles.get(x, y)->setIsEmpty(false);
			vec[0] = static_cast<float>(x + curOffsetX);
			vec[1] = static_cast<float>(offsetY);
			vec[2] = m_depth;
			setTileColor(vec, *m_tiles.get(x, y));
		}
	}
}

void Map::computeDecor(void)
{
	float vec[3];
	int offset;
	int offsetPosX;
	int height;
	int offsetX = static_cast<int>(m_curOffset.x / Tile::TileSize);

	assert(m_depth >= 0.f);
	assert(m_mapSurface);

	for (auto it = m_decors.begin(); it != m_decors.end(); it++)
	{
		offset = offsetX;
		offsetPosX = it->first;
		while (offset < 0)
		{
			offset += m_mapSize.x;
			offsetPosX -= m_mapSize.x;
		}
		while (offset >= static_cast<int>(m_mapSize.x))
		{
			offset -= m_mapSize.x;
			offsetPosX += m_mapSize.x;
		}
		int border = offset + static_cast<int>(m_tiles.columns());
		if (border > static_cast<int>(m_mapSize.x))
		{
			if (it->first < (border % static_cast<int>(m_mapSize.x)) + 20)
				offsetPosX += m_mapSize.x;
		}
		vec[0] = static_cast<float>(it->first) / static_cast<float>(m_mapSize.x);
		vec[1] = m_depth / static_cast<float>(m_mapSize.y);
		height = static_cast<int>((m_mapSurface(vec[0], vec[1]) + 1.f) * static_cast<float>(m_mapSize.y) / 2.f);
		it->second->setStartTransition(0u, sf::Vector2f(offsetPosX * Tile::TileSize, height * Tile::TileSize));
		vec[0] = static_cast<float>(offsetPosX);
		vec[1] = static_cast<float>(height);
		vec[2] = m_depth;
		setTileColor(vec, *it->second);
	}
}

sf::Vertex * Map::getHeight(int x)
{
	//TODO: change find
	auto it = m_decors.find(x);
	if (it == m_decors.end())
	{
		m_decors[x] = &m_reserveTile[m_decorTileCount];
		m_decors[x]->setUpLeft(&m_vertices[m_decorTileCount]);
		m_decorTileCount++;
	}
	return m_decors[x]->getUpLeft();
}

void Map::setTileColor(float * vec, Tile & tile)
{
	vec[0] /= 10.f;
	vec[1] /= 10.f;
	vec[2] /= 10.f;
	float noise = (OctoNoise::getCurrent().noise3(vec) + 1.f) / 2.f;

	sf::Color end = sf::Color(250.f, 150.f, 0.f);
	sf::Color start = sf::Color(250.f, 0.f, 0.f);

	tile.setStartColor(octo::linearInterpolation(start, end, noise));
}

void Map::swapDepth(void)
{
	float tmp = m_depth;
	m_depth = m_oldDepth;
	m_oldDepth = tmp;
	for (auto & instance : m_instances)
		instance->swapDepth();
}

void Map::registerDepth(void)
{
	m_oldDepth = m_depth;
	for (auto & instance : m_instances)
		instance->registerDepth();
}

void Map::nextStep(void)
{
	m_depth += 3.f;
	for (auto & instance : m_instances)
		instance->nextStep();
}

void Map::previousStep(void)
{
	m_depth -= 3.f;
	for (auto & instance : m_instances)
		instance->previousStep();
}

void Map::addOffsetX(int offsetX)
{
	Tile *	m_tmp[m_tiles.rows()];

	if (offsetX > 0)
	{
		for (std::size_t y = 0; y < m_tiles.rows(); y++)
			m_tmp[y] = m_tiles(0, y);
		for (std::size_t x = 0; x < m_tiles.columns() - 1; x++)
		{
			for (std::size_t y = 0; y < m_tiles.rows(); y++)
				m_tiles(x, y) = m_tiles(x + 1, y);
		}
		for (std::size_t y = 0; y < m_tiles.rows(); y++)
			m_tiles(m_tiles.columns() - 1, y) = m_tmp[y];
		addOffsetX(offsetX - 1);
	}
	else if (offsetX < 0)
	{
		for (std::size_t y = 0; y < m_tiles.rows(); y++)
			m_tmp[y] = m_tiles(m_tiles.columns() - 1, y);
		for (std::size_t x = m_tiles.columns() - 1; x > 0; x--)
		{
			for (std::size_t y = 0; y < m_tiles.rows(); y++)
				m_tiles(x, y) = m_tiles(x - 1, y);
		}
		for (std::size_t y = 0; y < m_tiles.rows(); y++)
			m_tiles(0, y) = m_tmp[y];
		addOffsetX(offsetX + 1);
	}
}

void Map::addOffsetY(int offsetY)
{
	Tile *	m_tmp[m_tiles.columns()];

	if (offsetY > 0)
	{
		for (std::size_t x = 0; x < m_tiles.columns(); x++)
			m_tmp[x] = m_tiles(x, 0);
		for (std::size_t y = 0; y < m_tiles.rows() - 1; y++)
		{
			for (std::size_t x = 0; x < m_tiles.columns(); x++)
				m_tiles(x, y) = m_tiles(x, y + 1);
		}
		for (std::size_t x = 0; x < m_tiles.columns(); x++)
			m_tiles(x, m_tiles.rows() - 1) = m_tmp[x];
		addOffsetY(offsetY - 1);
	}
	else if (offsetY < 0)
	{
		for (std::size_t x = 0; x < m_tiles.columns(); x++)
			m_tmp[x] = m_tiles(x, m_tiles.rows() - 1);
		for (std::size_t y = m_tiles.rows() - 1; y > 0; y--)
		{
			for (std::size_t x = 0; x < m_tiles.columns(); x++)
				m_tiles(x, y) = m_tiles(x, y - 1);
		}
		for (std::size_t x = 0; x < m_tiles.columns(); x++)
			m_tiles(x, 0) = m_tmp[x];
		addOffsetY(offsetY + 1);
	}
}
