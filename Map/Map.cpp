#include "Map.hpp"
#include "OctoNoise.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Interpolations.hpp>

Map::Map(void) :
	m_depth(0.f),
	m_oldDepth(0.f),
	m_biome(),
	m_width(0u),
	m_height(0u),
	m_offset(nullptr),
	m_curOffset(),
	m_totalWidth(0),
	m_decorTileCount(0u)
{}

Map::~Map(void)
{
	for (std::size_t x = 0; x < m_tiles.columns(); x++)
	{
		for (std::size_t y = 0; y < m_tiles.rows(); y++)
			delete m_tiles.get(x, y);
	}
}

void Map::init(Biome * p_biome)
{
	m_biome = p_biome;
	m_width = octo::Application::getGraphicsManager().getVideoMode().width / Tile::TileSize + 4u; // 4 tiles to add margin at left and right
	m_height  = octo::Application::getGraphicsManager().getVideoMode().height / Tile::TileSize + 6u; // 6 tiles to add margin at top and bottom
	initBiome();

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
}

void Map::initBiome(void)
{
	m_biome->mn_height = 128u;
	m_biome->mn_width = 512u;
	m_biome->mn_startOffsetX = 0u;
	m_biome->mf_transitionTimerMax = 0.5f;
	m_biome->mn_nbDecor = 15u;
	m_biome->mn_temperature = 1;
	m_biome->mn_wind = 300;
	m_totalWidth = m_biome->mn_width;
	m_biome->m_totalWidth = m_totalWidth;
}

void Map::computeMapRange(int p_startX, int p_endX, int p_startY, int p_endY)
{
	float vec[3];
	int height;
	int offset;
	int offsetX = static_cast<int>(m_curOffset.x / Tile::TileSize);
	int offsetY;
	int offsetPosX;
	float v;
	for (int x = p_startX; x < p_endX; x++)
	{
		offset = x + offsetX;
		offsetPosX = offset;
		while (offset < 0)
			offset += m_biome->mn_width;
		while (offset >= static_cast<int>(m_biome->mn_width))
			offset -= m_biome->mn_width;
		vec[0] = static_cast<float>(offset);
		vec[1] = m_depth;
		// firstCurve return a value b/tween -1 & 1
		// we normalize it betwen 0 & max_height
		v = (firstCurve(vec) + 1.f) * static_cast<float>(m_biome->mn_height) / 2.f;
		height = static_cast<int>(v);
		for (int y = p_startY; y < p_endY; y++)
		{
			offsetY = y + static_cast<int>(m_curOffset.y / Tile::TileSize);
			// Init square
			m_tiles.get(x, y)->setStartTransition(0, sf::Vector2f((offsetPosX) * Tile::TileSize, (offsetY) * Tile::TileSize));
			m_tiles.get(x, y)->setStartTransition(1, sf::Vector2f((offsetPosX + 1) * Tile::TileSize, (offsetY) * Tile::TileSize));
			m_tiles.get(x, y)->setStartTransition(2, sf::Vector2f((offsetPosX + 1) * Tile::TileSize, (offsetY + 1) * Tile::TileSize));
			m_tiles.get(x, y)->setStartTransition(3, sf::Vector2f((offsetPosX) * Tile::TileSize, (offsetY + 1) * Tile::TileSize));
			if (offsetY < height || offsetY < 0)
			{
				m_tiles.get(x, y)->setIsEmpty(true);
				continue;
			}
			vec[0] = static_cast<float>(x + offsetX);
			vec[1] = static_cast<float>(offsetY);
			vec[2] = m_depth;
			// secondCurve return a value between -1 & 1
			m_tiles.get(x, y)->setNoiseValue((secondCurve(vec) + 1.f) / 2.f);
			m_tiles.get(x, y)->setIsEmpty(false);
			setColor(*m_tiles.get(x, y));
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
	for (auto it = m_decorPositions.begin(); it != m_decorPositions.end(); it++)
	{
		offset = offsetX;
		offsetPosX = it->first;
		while (offset < 0)
		{
			offset += m_totalWidth;
			offsetPosX -= m_totalWidth;
		}
		while (offset >= static_cast<int>(m_totalWidth))
		{
			offset -= m_totalWidth;
			offsetPosX += m_totalWidth;
		}
		int border = offset + static_cast<int>(m_tiles.columns());
		if (border > static_cast<int>(m_totalWidth))
		{
			if (it->first < (border % static_cast<int>(m_totalWidth)) + 20)
				offsetPosX += m_totalWidth;
		}
		vec[0] = static_cast<float>(it->first);
		vec[1] = m_depth;
		height = static_cast<int>((firstCurve(vec) + 1.f) * static_cast<float>(m_biome->mn_height) / 2.f);
		it->second.x = offsetPosX * Tile::TileSize;
		it->second.y = height * Tile::TileSize;
		//vec[0] = static_cast<float>(offsetPosX);
		//vec[1] = static_cast<float>(height);
		//vec[2] = m_depth;
		//it->second->setNoiseValue((secondCurve(vec) + 1.f) / 2.f);
		//setColor(*it->second);
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
}

void Map::registerDepth(void)
{
	m_oldDepth = m_depth;
}

void Map::addOffsetX(int p_offsetX)
{
	Tile *	m_tmp[m_tiles.rows()];
	if (p_offsetX > 0)
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
	}
	else if (p_offsetX < 0)
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
	}
}

void Map::addOffsetY(int p_offsetY)
{
	Tile *	m_tmp[m_tiles.columns()];
	if (p_offsetY > 0)
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
	}
	else if (p_offsetY < 0)
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
	}
}

void Map::nextStep(void)
{
	m_depth += 3.f;
}

void Map::previousStep(void)
{
	m_depth -= 3.f;
}
