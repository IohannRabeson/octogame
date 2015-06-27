#include "StaticTileObject.hpp"
#include "Tile.hpp"

StaticTileObject::StaticTileObject(std::size_t width, std::size_t height, std::size_t depth) :
	m_depth(0),
	m_oldDepth(0)
{
	// Init 3D TileMap
	m_tiles.resize(width, height, depth, nullptr);

	for (std::size_t z = 0; z < m_tiles.depth(); z++)
	{
		for (std::size_t x = 0; x < m_tiles.columns(); x++)
		{
			for (std::size_t y = 0; y < m_tiles.rows(); y++)
				m_tiles(x, y, z) = new Tile();
		}
	}
}

StaticTileObject::~StaticTileObject(void)
{
	
	for (std::size_t z = 0; z < m_tiles.depth(); z++)
	{
		for (std::size_t x = 0; x < m_tiles.columns(); x++)
		{
			for (std::size_t y = 0; y < m_tiles.rows(); y++)
				delete m_tiles(x, y, z);
		}
	}
}

void StaticTileObject::load(void)
{
	for (std::size_t z = 0; z < m_tiles.depth(); z++)
	{
		for (std::size_t x = 0; x < m_tiles.columns(); x++)
		{
			for (std::size_t y = 0; y < m_tiles.rows(); y++)
			{
				if (z == 1)
					m_tiles(x, y, z)->setIsEmpty(false);
				else
				{
					if ((x % 3 == 0 || x % 4 == 0) && y < 8)
						m_tiles(x, y, z)->setIsEmpty(true);
					else
						m_tiles(x, y, z)->setIsEmpty(false);
				}
			}
		}
	}
}

void StaticTileObject::swapDepth(void)
{
	int tmp = m_depth;
	m_depth = m_oldDepth;
	m_oldDepth = tmp;
}

void StaticTileObject::registerDepth(void)
{
	m_oldDepth = m_depth;
}

void StaticTileObject::nextStep(void)
{
	m_depth++;
	if (m_depth >= static_cast<int>(m_tiles.depth()))
		m_depth = 0;
}

void StaticTileObject::previousStep(void)
{
	m_depth--;
	if (m_depth < 0)
		m_depth = static_cast<int>(m_tiles.depth()) - 1;
}

Tile const & StaticTileObject::get(std::size_t x, std::size_t y) const
{
	return *m_tiles(x, y, m_depth);
}

std::size_t StaticTileObject::getWidth(void) const
{
	return m_tiles.columns();
}

std::size_t StaticTileObject::getHeight(void) const
{
	return m_tiles.rows();
}
