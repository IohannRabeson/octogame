#include "MapInstance.hpp"
#include "Tile.hpp"
#include <Application.hpp>
#include <LevelMap.hpp>
#include <ResourceManager.hpp>

MapInstance::MapInstance(std::size_t position, std::string const & resourceId) :
	m_levelMap(octo::Application::getResourceManager().getLevelMap(resourceId)),
	m_maxDepth(m_levelMap.getMapCount()),
	m_depth(0),
	m_oldDepth(0)
{
	m_cornerPositions.left = position;
	m_cornerPositions.top = -m_levelMap.getMapSize().y + MapInstance::HeightOffset;
	m_cornerPositions.width = m_cornerPositions.left + m_levelMap.getMapSize().x;
	m_cornerPositions.height = m_cornerPositions.top + m_levelMap.getMapSize().y;
	// Init 3D TileMap
	m_tiles = new octo::Array2D<Tile*>[m_maxDepth];
	for (std::size_t i = 0; i < m_maxDepth; i++)
		m_tiles[i].resize(m_levelMap.getMapSize().x, m_levelMap.getMapSize().y, nullptr);

	for (std::size_t i = 0; i < m_maxDepth; i++)
	{
		int * map = m_levelMap.getMap(i);
		for (std::size_t x = 0; x < m_tiles[i].columns(); x++)
		{
			for (std::size_t y = 0; y < m_tiles[i].rows(); y++)
			{
				m_tiles[i](x, y) = new Tile();
				if (map[y * m_tiles[i].columns() + x] == 0)
					m_tiles[i](x, y)->setIsEmpty(true);
				else
					m_tiles[i](x, y)->setIsEmpty(false);
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

void MapInstance::setTransitionType(Tile & tile)
{
	(void)tile;
	/*
	switch (tile.tileType)
	{
		case 0:
			break;
		default:
			break;
	}*/
}
