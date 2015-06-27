#include "NewMap.hpp"
#include "OctoNoise.hpp"
#include "ABiome.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Interpolations.hpp>

NewMap::NewMap(void) :
	m_depth(0.f),
	m_oldDepth(0.f),
	m_width(0u),
	m_height(0u),
	m_offset(nullptr),
	m_decorTileCount(0u)
{}

NewMap::~NewMap(void)
{
	for (std::size_t x = 0; x < m_tiles.columns(); x++)
	{
		for (std::size_t y = 0; y < m_tiles.rows(); y++)
			delete m_tiles.get(x, y);
	}
}

void NewMap::init(ABiome & biome)
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
}

void NewMap::computeMapRange(int startX, int endX, int startY, int endY)
{
	float vec[3];
	int height;
	int offset;
	int offsetX = static_cast<int>(m_curOffset.x / Tile::TileSize);
	int offsetY;
	int offsetPosX;
	float v;
	for (int x = startX; x < endX; x++)
	{
		offset = x + offsetX;
		offsetPosX = offset;
		while (offset < 0)
			offset += m_mapSize.x;
		while (offset >= static_cast<int>(m_mapSize.x))
			offset -= m_mapSize.x;
		vec[0] = static_cast<float>(offset);
		vec[1] = m_depth;
		// firstCurve return a value b/tween -1 & 1
		// we normalize it betwen 0 & max_height
		v = (firstCurve(vec) + 1.f) * static_cast<float>(m_mapSize.y) / 2.f;
		height = static_cast<int>(v);
		for (int y = startY; y < endY; y++)
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

void NewMap::computeDecor(void)
{
	float vec[3];
	int offset;
	int offsetPosX;
	int height;
	int offsetX = static_cast<int>(m_curOffset.x / Tile::TileSize);
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
		vec[0] = static_cast<float>(it->first);
		vec[1] = m_depth;
		height = static_cast<int>((firstCurve(vec) + 1.f) * static_cast<float>(m_mapSize.y) / 2.f);
		it->second->setStartTransition(0, sf::Vector2f(offsetPosX * Tile::TileSize, height * Tile::TileSize));
		vec[0] = static_cast<float>(offsetPosX);
		vec[1] = static_cast<float>(height);
		vec[2] = m_depth;
		it->second->setNoiseValue((secondCurve(vec) + 1.f) / 2.f);
		setColor(*it->second);
	}
}

sf::Vertex * NewMap::getHeight(int x)
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

float NewMap::firstCurve(float * vec)
{
	vec[0] /= 100.f;
	vec[1] /= 100.f;
	return OctoNoise::getCurrent().fbm(vec, 3, 2.0f, 0.4f);
}

float NewMap::secondCurve(float * vec)
{
	vec[0] /= 10.f;
	vec[1] /= 10.f;
	vec[2] /= 10.f;
	return OctoNoise::getCurrent().noise3(vec);
	//return sin(vec[0] * 15.f + OctoNoise::getCurrent().noise3(vec) * sin(vec[1]) * 5.f);
}

void NewMap::setColor(Tile & tile)
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

void NewMap::swapDepth(void)
{
	float tmp = m_depth;
	m_depth = m_oldDepth;
	m_oldDepth = tmp;
}

void NewMap::registerDepth(void)
{
	m_oldDepth = m_depth;
}

void NewMap::addOffsetX(int offsetX)
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
	}
}

void NewMap::addOffsetY(int offsetY)
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
	}
}

void NewMap::nextStep(void)
{
	m_depth += 3.f;
}

void NewMap::previousStep(void)
{
	m_depth -= 3.f;
}
