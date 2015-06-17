#include "Map.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>

Map::Map(void) :
	m_depth(0.f),
	m_oldDepth(0.f),
	m_biome(),
	m_width(0u),
	m_height(0u),
	m_offset(nullptr),
	m_curOffset(),
	mn_totalWidth(0),
	mn_decorTileCount(0u)
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
			m_tiles.get(x, y)->m_startTransition[0] = sf::Vector2f(x * Tile::TileSize, y * Tile::TileSize);
			m_tiles.get(x, y)->m_startTransition[1] = sf::Vector2f(x * Tile::TileSize + Tile::TileSize, y * Tile::TileSize);
			m_tiles.get(x, y)->m_startTransition[2] = sf::Vector2f(x * Tile::TileSize + Tile::TileSize, y * Tile::TileSize + Tile::TileSize);
			m_tiles.get(x, y)->m_startTransition[3] = sf::Vector2f(x * Tile::TileSize, y * Tile::TileSize + Tile::TileSize);
		}
	}
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
			m_tiles.get(x, y)->m_startTransition[0] = sf::Vector2f((offsetPosX) * Tile::TileSize, (offsetY) * Tile::TileSize);
			m_tiles.get(x, y)->m_startTransition[1] = sf::Vector2f((offsetPosX + 1) * Tile::TileSize, (offsetY) * Tile::TileSize);
			m_tiles.get(x, y)->m_startTransition[2] = sf::Vector2f((offsetPosX + 1) * Tile::TileSize, (offsetY + 1) * Tile::TileSize);
			m_tiles.get(x, y)->m_startTransition[3] = sf::Vector2f((offsetPosX) * Tile::TileSize, (offsetY + 1) * Tile::TileSize);
			if (offsetY < height || offsetY < 0)
			{
				m_tiles.get(x, y)->mb_isEmpty = true;
				continue;
			}
			vec[0] = static_cast<float>(x + offsetX);
			vec[1] = static_cast<float>(offsetY);
			vec[2] = m_depth;
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
			offset += mn_totalWidth;
			offsetPosX -= mn_totalWidth;
		}
		while (offset >= static_cast<int>(mn_totalWidth))
		{
			offset -= mn_totalWidth;
			offsetPosX += mn_totalWidth;
		}
		int border = offset + static_cast<int>(m_tiles.columns());
		if (border > static_cast<int>(mn_totalWidth))
		{
			if (it->first < (border % static_cast<int>(mn_totalWidth)) + 20)
				offsetPosX += mn_totalWidth;
		}
		vec[0] = static_cast<float>(it->first);
		vec[1] = m_depth;
		height = static_cast<int>((firstCurve(vec) + 1.f) * static_cast<float>(m_biome->mn_height) / 2.f);
		it->second->m_startTransition[0].x = offsetPosX * Tile::TileSize;
		it->second->m_startTransition[0].y = height * Tile::TileSize;
		vec[0] = static_cast<float>(offsetPosX);
		vec[1] = static_cast<float>(height);
		vec[2] = m_depth;
		it->second->mf_noiseValue = (secondCurve(vec) + 1.f) / 2.f;
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
