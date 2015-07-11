#include "Map.hpp"
#include "OctoNoise.hpp"
#include "ABiome.hpp"
#include "MapInstance.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Interpolations.hpp>

Map::Map(void) :
	m_depth(0.f),
	m_oldDepth(0.f),
	m_width(0u),
	m_height(0u),
	m_offset(nullptr)
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
	for (int x = startX; x < endX; x++)
	{
		curInstance = nullptr;
		offsetX = x + curOffsetX;
		offsetPosX = offsetX;
		while (offsetX < 0)
			offsetX += m_mapSize.x;
		while (offsetX >= static_cast<int>(m_mapSize.x))
			offsetX -= m_mapSize.x;
		vec[0] = static_cast<float>(offsetX);
		vec[1] = m_depth;

		for (auto & instance : m_instances)
		{
			if (offsetX >= static_cast<int>(instance->getCornerPositions().left) && offsetX < static_cast<int>(instance->getCornerPositions().width))
			{
				curInstance = instance.get();
				break;
			}
		}
		// firstCurve return a value between -1 & 1
		// we normalize it betwen 0 & max_height
		height = static_cast<int>((firstCurve(vec) + 1.f) * static_cast<float>(m_mapSize.y) / 2.f);
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
			// secondCurve return a value between -1 & 1
			m_tiles.get(x, y)->setNoiseValue((secondCurve(vec) + 1.f) / 2.f);
			setColor(*m_tiles.get(x, y));
		}
	}
}

void Map::computeDecor(void)
{
	float vec[3];
	int height;
	int offsetX;
	int offsetPosX;
	int curOffsetX = static_cast<int>(m_curOffset.x / Tile::TileSize);
	for (auto it = m_decorPositions.begin(); it != m_decorPositions.end(); it++)
	{
		offsetX = curOffsetX;
		offsetPosX = it->first;
		while (offsetX < 0)
		{
			offsetX += m_mapSize.x;
			offsetPosX -= m_mapSize.x;
		}
		while (offsetX >= static_cast<int>(m_mapSize.x))
		{
			offsetX -= m_mapSize.x;
			offsetPosX += m_mapSize.x;
		}
		int border = offsetX + static_cast<int>(m_tiles.columns()) + 20;
		if (border > static_cast<int>(m_mapSize.x))
		{
			if (it->first < (border % static_cast<int>(m_mapSize.x)) + static_cast<int>(m_tiles.columns()))
				offsetPosX += m_mapSize.x;
			else if (it->first < 40)
			{
				offsetPosX += m_mapSize.x;
			}
		}
		else if (offsetX < 60)
		{
			if (it->first > static_cast<int>(m_mapSize.x) - 40)
				offsetPosX -= m_mapSize.x;
		}
		vec[0] = static_cast<float>(it->first);
		vec[1] = m_depth;
		height = static_cast<int>((firstCurve(vec) + 1.f) * static_cast<float>(m_mapSize.y) / 2.f);
		it->second.x = offsetPosX * Tile::TileSize;
		it->second.y = height * Tile::TileSize;
	}
}

void Map::registerDecor(int x)
{
	m_decorPositions.emplace_back(std::pair<int, sf::Vector2f>(x, sf::Vector2f()));
}

float Map::firstCurve(float * vec)
{
	vec[0] /= 100.f;
	vec[1] /= 100.f;
	return OctoNoise::getCurrent().fbm(vec, 3, 2.0f, 0.4f);
}

float Map::secondCurve(float * vec)
{
	vec[0] /= 10.f;
	vec[1] /= 10.f;
	vec[2] /= 10.f;
	return OctoNoise::getCurrent().noise3(vec);
	//return sin(vec[0] * 15.f + OctoNoise::getCurrent().noise3(vec) * sin(vec[1]) * 5.f);
}

void Map::setColor(Tile & tile)
{
	sf::Color start = sf::Color(178.f, 0.f, 86.f);
	sf::Color end = sf::Color(178.f, 162.f, 32.f);
	sf::Color mid = sf::Color(0.f, 74.f, 213.f);

	start = octo::linearInterpolation(end, mid, tile.getNoiseValue());

	if (tile.getNoiseValue() < 0.4f)
		tile.setStartColor(start);
	else if (tile.getNoiseValue() < 0.6f)
	{
		tile.setStartColor(octo::linearInterpolation(start, end, (tile.getNoiseValue() - 0.4f) * 5.f));
	}
	else
		tile.setStartColor(end);
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
