#include "Map.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>

Map::Map(void) :
	mf_depth(0.f),
	mf_oldDepth(0.f),
	mn_totalWidth(0),
	mn_offsetX(0),
	mn_offsetY(0),
	m_width(0u),
	m_height(0u),
	m_offset(),
	mn_decorTileCount(0u)
{
	m_vertices.reset(new sf::Vertex[MaxDecor]);
}

Map::~Map(void)
{
	for (std::size_t x = 0; x < m_tiles.columns(); x++)
	{
		for (std::size_t y = 0; y < m_tiles.rows(); y++)
			delete m_tiles.get(x, y);
	}
}

#include <iostream>
void Map::init(Biome * p_biome)
{
	m_biome = p_biome;
	m_width = octo::Application::getGraphicsManager().getVideoMode().width / Tile::TileSize + 4u;
	m_height  = octo::Application::getGraphicsManager().getVideoMode().height / Tile::TileSize + 4u;
	std::cout << octo::Application::getGraphicsManager().getVideoMode().width << std::endl;
	std::cout << octo::Application::getGraphicsManager().getVideoMode().height << std::endl;
	//m_width = 1920 / Tile::TileSize + 3u;
	//m_height = 1080 / Tile::TileSize + 5u;

	initBiome();

	m_tiles.resize(m_width -20u, m_height-20u, nullptr);

	for (std::size_t x = 0; x < m_tiles.columns(); x++)
	{
		for (std::size_t y = 0; y < m_tiles.rows(); y++)
			m_tiles.get(x, y) = new Tile();
	}
}

void Map::computeMapRange(int p_startX, int p_endX, int p_startY, int p_endY)
{
	float vec[3];
	int height;
	int offset;
	int offsetY;
	int offsetPosX;
	float v;
	for (int x = p_startX; x < p_endX; x++)
	{
		offset = x + static_cast<int>(m_offset.x / Tile::TileSize) + 2u;
		offsetPosX = offset;
		while (offset < 0)
			offset += m_biome->mn_width;
		while (offset >= static_cast<int>(m_biome->mn_width))
			offset -= m_biome->mn_width;
		vec[0] = static_cast<float>(offset);
		vec[1] = mf_depth;
		// firstCurve return a value b/tween -1 & 1
		// we normalize it betwen 0 & max_height
		v = (firstCurve(vec) + 1.f) * static_cast<float>(m_biome->mn_height) / 2.f;
		height = static_cast<int>(v);
		for (int y = p_startY; y < p_endY; y++)
		{
			offsetY = y + static_cast<int>(m_offset.y / Tile::TileSize) + 2u;
			// TODO: optimize
			// TODO: keep pointer to vector to avoid copy
			m_tiles.get(x, y)->m_startTransition[0] = sf::Vector2f((offsetPosX) * Tile::TileSize, (offsetY) * Tile::TileSize);
			m_tiles.get(x, y)->m_startTransition[1] = sf::Vector2f((offsetPosX + 1) * Tile::TileSize, (offsetY) * Tile::TileSize);
			m_tiles.get(x, y)->m_startTransition[2] = sf::Vector2f((offsetPosX + 1) * Tile::TileSize, (offsetY + 1) * Tile::TileSize);
			m_tiles.get(x, y)->m_startTransition[3] = sf::Vector2f((offsetPosX) * Tile::TileSize, (offsetY + 1) * Tile::TileSize);
			if (offsetY < height || offsetY < 0)
			{
				m_tiles.get(x, y)->mb_isEmpty = true;
				continue;
			}
			vec[0] = static_cast<float>(offset);
			vec[1] = static_cast<float>(offsetY);
			vec[2] = mf_depth;
			// secondCurve return a value between -1 & 1
			m_tiles.get(x, y)->mf_noiseValue = (secondCurve(vec) + 1.f) / 2.f;
			m_tiles.get(x, y)->mb_isEmpty = false;
			setColor(*m_tiles.get(x, y));
		}
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

void Map::addOffsetX(int p_offsetX)
{
	//TODO: change this
	Tile *	m_tmp[500];
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
		computeMapRangeX(m_tiles.columns() - 1, m_tiles.columns());
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
		computeMapRangeX(0, 1);
	}
}

// TODO: change this
void Map::addOffsetY(int p_offsetY)
{
	//TODO: change this
	Tile *	m_tmp[500];
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
		computeMapRangeY(m_tiles.rows() - 1, m_tiles.rows());
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
		computeMapRangeY(0, 1);
	}
}
