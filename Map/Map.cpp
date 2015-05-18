#include "Map.hpp"
#include <iostream>

Map::Map(std::size_t pn_width, std::size_t pn_height) :
	mf_depth(0.f),
	mf_oldDepth(0.f),
	mn_totalWidth(0),
	mn_offsetX(0),
	mn_offsetY(0),
	mn_colorOffsetX(0),
	mn_decorTileCount(0u)
{
	pn_width += 3;
	pn_height += 5;
	m_tiles.resize(pn_width, pn_height, nullptr);

	m_vertices.reset(new sf::Vertex[MaxDecor]);
	for (std::size_t x = 0; x < m_tiles.columns(); x++)
	{
		for (std::size_t y = 0; y < m_tiles.rows(); y++)
		{
			m_tiles.get(x, y) = new Tile();
			initQuad(x, y);
		}
	}
}

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
	initBiome();
}

void Map::initQuad(int x, int y)
{
	sf::Vertex ver;
	ver.position = sf::Vector2f(x * Tile::TileSize, y * Tile::TileSize + Tile::TileSize);
	ver.color = sf::Color(128.f, 0.f, 128.f);
	m_tiles.get(x, y)->m_startTransition[0] = ver.position;

	ver.position = sf::Vector2f(x * Tile::TileSize + Tile::TileSize, y * Tile::TileSize + Tile::TileSize);
	ver.color = sf::Color(128.f, 0.f, 128.f);
	m_tiles.get(x, y)->m_startTransition[1] = ver.position;

	ver.position = sf::Vector2f(x * Tile::TileSize + Tile::TileSize, y * Tile::TileSize + Tile::TileSize);
	ver.color = sf::Color(128.f, 0.f, 128.f);
	m_tiles.get(x, y)->m_startTransition[2] = ver.position;

	ver.position = sf::Vector2f(x * Tile::TileSize, y * Tile::TileSize + Tile::TileSize);
	ver.color = sf::Color(128.f, 0.f, 128.f);
	m_tiles.get(x, y)->m_startTransition[3] = ver.position;
}

void Map::computeMapRange(int p_startX, int p_endX, int p_startY, int p_endY)
{
	float vec[3];
	int height;
	std::size_t offset;
	float v;
	// Init perlin value
	for (int x = p_startX; x < p_endX; x++)
	{
		offset = x + mn_offsetX;
		if (offset >= m_biome->mn_width)
		{
			if (offset >= mn_totalWidth)
				offset -= mn_totalWidth;
		}
		vec[0] = static_cast<float>(offset);
		vec[1] = mf_depth;
		// firstCurve return a value b/tween -1 & 1
		// we normalize it betwen 0 & max_height
		v = (firstCurve(vec) + 1.f) * static_cast<float>(m_biome->mn_height) / 2.f;
		height = static_cast<int>(v) - mn_offsetY;
		if (height < 0)
			height = 0;
		else if (height > static_cast<int>(m_tiles.rows()))
			height = m_tiles.rows();
		for (int y = height; y < p_endY; y++)
		{
			vec[0] = static_cast<float>(x + mn_colorOffsetX);
			vec[1] = static_cast<float>(y + mn_offsetY);
			vec[2] = mf_depth;
			// secondCurve return a value between -1 & 1
			m_tiles.get(x, y)->mf_noiseValue = (secondCurve(vec) + 1.f) / 2.f;
			m_tiles.get(x, y)->mb_isEmpty = false;
			setColor(*m_tiles.get(x, y));
		}
		for (int y = p_startY; y < height; y++)
			m_tiles.get(x, y)->mb_isEmpty = true;
	}
}

void Map::computeDecor(void)
{
	float vec[3];
	float v;
	int height;
	for (auto it = m_decors.begin(); it != m_decors.end(); it++)
	{
		vec[0] = static_cast<float>(it->first);
		vec[1] = mf_depth;
		v = (firstCurve(vec) + 1.f) * static_cast<float>(m_biome->mn_height) / 2.f;
		height = static_cast<int>(v);
		it->second->m_startTransition[0].y = height * Tile::TileSize + Tile::TileSize;
		vec[0] = static_cast<float>(it->first);
		vec[1] = static_cast<float>(height);
		vec[2] = mf_depth;
		it->second->mf_noiseValue = (secondCurve(vec) + 1.f) / 2.f;
		it->second->mb_isEmpty = false;
		setColor(*it->second);
	}
}

void Map::addOffsetX(int p_offsetX)
{
	mn_offsetX += p_offsetX;
	mn_colorOffsetX += p_offsetX;
	if (mn_offsetX < 0)
		mn_offsetX += mn_totalWidth;
	else if (mn_offsetX >= static_cast<int>(mn_totalWidth))
		mn_offsetX -= static_cast<int>(mn_totalWidth);

	if (p_offsetX > 0)
	{
		for (std::size_t x = 0; x < m_tiles.columns() - 1; x++)
		{
			for (std::size_t y = 0; y < m_tiles.rows(); y++)
				m_tiles.get(x, y)->copy(*m_tiles.get(x + 1, y), -Tile::TileSize, 0.f);
		}
	}
	else if (p_offsetX < 0)
	{
		for (int x = m_tiles.columns() - 1; x > 0; x--)
		{
			for (std::size_t y = 0; y < m_tiles.rows(); y++)
				m_tiles.get(x, y)->copy(*m_tiles.get(x - 1, y), Tile::TileSize, 0.f);
		}
	}
}

void Map::addOffsetY(int p_offsetY)
{
	mn_offsetY += p_offsetY;

	if (p_offsetY > 0)
	{
		for (std::size_t x = 0; x < m_tiles.columns(); x++)
		{
			for (std::size_t y = 0; y < m_tiles.rows() - 1; y++)
				m_tiles.get(x, y)->copy(*m_tiles.get(x, y + 1), 0.f, -Tile::TileSize);
		}
	}
	else if (p_offsetY < 0)
	{
		for (std::size_t x = 0; x < m_tiles.columns(); x++)
		{
			for (int y = m_tiles.rows() - 1; y > 0; y--)
				m_tiles.get(x, y)->copy(*m_tiles.get(x, y - 1), 0.f, Tile::TileSize);
		}
	}
}

sf::Vertex * Map::getHeight(int x)
{
	auto it = m_decors.find(x);
	if (it == m_decors.end())
	{
		m_decors[x] = &m_reserveTile[mn_decorTileCount];
		m_decors[x]->mp_upLeft = &m_vertices[mn_decorTileCount];
		mn_decorTileCount++;
	}
	return m_decors[x]->mp_upLeft;
}

void Map::swapDepth(void)
{
	float tmp = mf_depth;
	mf_depth = mf_oldDepth;
	mf_oldDepth = tmp;
}

void Map::registerDepth(void)
{
	mf_oldDepth = mf_depth;
}

// TODO: care if decors are larger than 20*TileSize
int Map::getOffsetXDecor(int p_decorOffsetX) const
{
	int i = p_decorOffsetX - mn_offsetX;
	if (i < -20)
		return i + mn_totalWidth;
	return i;
}
