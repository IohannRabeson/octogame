#include "StaticTileObject.hpp"

StaticTileObject::StaticTileObject(unsigned int p_width, unsigned int p_height, unsigned int p_depth) :
	GameObject(),
	mn_maxDepth(p_depth),
	mn_depth(0),
	mn_oldDepth(0),
	mn_width(p_width),
	mn_height(p_height)
{
	// Init 3D TileMap
	m_tiles = new Map::TileMap[mn_maxDepth];
	for (unsigned int i = 0; i < mn_maxDepth; i++)
		m_tiles[i].resize(p_width, p_height, nullptr);

	for (unsigned int i = 0; i < mn_maxDepth; i++)
	{
		for (unsigned int x = 0; x < m_tiles[i].columns(); x++)
		{
			for (unsigned int y = 0; y < m_tiles[i].rows(); y++)
			{
				m_tiles[i](x, y) = new Tile();
			}
		}
	}
}

StaticTileObject::~StaticTileObject(void)
{
	
	for (unsigned int i = 0; i < mn_maxDepth; i++)
	{
		for (unsigned int x = 0; x < m_tiles[i].columns(); x++)
		{
			for (unsigned int y = 0; y < m_tiles[i].rows(); y++)
				delete m_tiles[i](x, y);
		}
	}
	delete [] m_tiles;
}

void StaticTileObject::load(void)
{
	for (unsigned int i = 0; i < mn_maxDepth; i++)
	{
		for (unsigned int x = 0; x < m_tiles[i].columns(); x++)
		{
			for (unsigned int y = 0; y < m_tiles[i].rows(); y++)
			{
				if (i == 1)
					m_tiles[i](x, y)->mb_isEmpty = false;
				else
				{
					if ((x % 3 == 0 || x % 4 == 0) && y < 8)
						m_tiles[i](x, y)->mb_isEmpty = true;
					else
						m_tiles[i](x, y)->mb_isEmpty = false;
				}
			}
		}
	}
}

void StaticTileObject::swapDepth(void)
{
	int tmp = mn_depth;
	mn_depth = mn_oldDepth;
	mn_oldDepth = tmp;
}

void StaticTileObject::registerDepth(void)
{
	mn_oldDepth = mn_depth;
}

void StaticTileObject::nextStep(void)
{
	mn_depth++;
	if (mn_depth >= static_cast<int>(mn_maxDepth))
		mn_depth = 0;
}

void StaticTileObject::previousStep(void)
{
	mn_depth--;
	if (mn_depth < 0)
		mn_depth = mn_maxDepth - 1;
}

Tile const & StaticTileObject::get(unsigned int x, unsigned int y) const
{
	return *m_tiles[mn_depth](x, y);
}

unsigned int StaticTileObject::getWidth(void) const
{
	return mn_width;
}

unsigned int StaticTileObject::getHeight(void) const
{
	return mn_height;
}
