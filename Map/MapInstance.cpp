#include "MapInstance.hpp"
#include "Tile.hpp"
#include <Application.hpp>
#include <LevelMap.hpp>
#include <ResourceManager.hpp>

MapInstance::MapInstance(std::size_t position, std::string const & resourceId) :
	m_levelMap(octo::Application::getResourceManager().getLevelMap(resourceId)),
	m_depth(0),
	m_oldDepth(0)
{
	m_cornerPositions.left = position;
	m_cornerPositions.top = -m_levelMap.getMapSize().y + MapInstance::HeightOffset;
	m_cornerPositions.width = m_cornerPositions.left + m_levelMap.getMapSize().x;
	m_cornerPositions.height = m_cornerPositions.top + m_levelMap.getMapSize().y;

	// Init 3D TileMap
	octo::Array3D<octo::LevelMap::TileType> const & map = m_levelMap.getMap();
	m_tiles.resize(map.columns(), map.rows(), map.depth());

	for (std::size_t z = 0; z < m_tiles.depth(); z++)
	{
		for (std::size_t x = 0; x < m_tiles.columns(); x++)
		{
			for (std::size_t y = 0; y < m_tiles.rows(); y++)
			{
				//TODO use TileType in Tile
				m_tiles(x, y, z).setTileType(map(x, y, z));
				if (map(x, y, z) == octo::LevelMap::TileType::Empty)
					m_tiles(x, y, z).setIsEmpty(true);
				else
					m_tiles(x, y, z).setIsEmpty(false);
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
	if (m_depth >= static_cast<int>(m_tiles.depth()))
		m_depth = 0;
}

void MapInstance::previousStep(void)
{
	m_depth--;
	if (m_depth < 0)
		m_depth = m_tiles.depth() - 1u;
}

void MapInstance::setTransitionType(Tile & tile)
{
	switch (tile.getTileType())
	{
		case octo::LevelMap::TileType::TopRightCorner:
			tile.setStartTransition(3u, tile.getStartTransition(3u) + sf::Vector2f(0.f, -Tile::TileSize));
			break;
		case octo::LevelMap::TileType::TopLeftCorner:
			tile.setStartTransition(2u, tile.getStartTransition(2u) + sf::Vector2f(0.f, -Tile::TileSize));
			break;
		default:
			break;
	}
}
