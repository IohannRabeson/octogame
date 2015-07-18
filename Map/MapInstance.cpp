#include "MapInstance.hpp"
#include "Tile.hpp"

MapInstance::MapInstance(std::size_t width, std::size_t height, std::size_t depth) :
	m_maxDepth(depth),
	m_depth(0),
	m_oldDepth(0)
{
	// Init 3D TileMap
	m_tiles = new octo::Array2D<Tile*>[m_maxDepth];
	for (std::size_t i = 0; i < m_maxDepth; i++)
		m_tiles[i].resize(width, height, nullptr);

	for (std::size_t i = 0; i < m_maxDepth; i++)
	{
		for (std::size_t x = 0; x < m_tiles[i].columns(); x++)
		{
			for (std::size_t y = 0; y < m_tiles[i].rows(); y++)
			{
				m_tiles[i](x, y) = new Tile();
			}
		}
	}
}

MapInstance::~MapInstance(void)
{
	for (std::size_t i = 0; i < m_maxDepth; i++)
	{
		for (std::size_t x = 0; x < m_tiles[i].columns(); x++)
		{
			for (std::size_t y = 0; y < m_tiles[i].rows(); y++)
				delete m_tiles[i](x, y);
		}
	}
	delete [] m_tiles;
}

void MapInstance::load(void)
{
		//TODO: load file from resources
	// The x position must be > 0
	// The y position is set by using the height of the map
	m_cornerPositions.left = 20;
	m_cornerPositions.top = -5;
	m_cornerPositions.width = m_cornerPositions.left + m_tiles[0].columns();
	m_cornerPositions.height = m_cornerPositions.top + m_tiles[0].rows();

	for (std::size_t i = 0; i < m_maxDepth; i++)
	{
		for (std::size_t x = 0; x < m_tiles[i].columns(); x++)
		{
			for (std::size_t y = 0; y < m_tiles[i].rows(); y++)
			{
				if (i == 1)
					m_tiles[i](x, y)->setIsEmpty(false);
				else
				{
					if (y < 1)
						m_tiles[i](x, y)->setIsEmpty(true);
					else
						m_tiles[i](x, y)->setIsEmpty(false);
				}
			}
		}
	}
}

void MapInstance::swapDepth(void)
{
	int tmp = m_depth;
	m_depth = m_oldDepth;
	m_oldDepth = tmp;
}

void MapInstance::registerDepth(void)
{
	m_oldDepth = m_depth;
}

void MapInstance::nextStep(void)
{
	m_depth++;
	if (m_depth >= static_cast<int>(m_maxDepth))
		m_depth = 0;
}

void MapInstance::previousStep(void)
{
	m_depth--;
	if (m_depth < 0)
		m_depth = m_maxDepth - 1;
}
