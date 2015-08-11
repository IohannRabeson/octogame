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
	m_tiles.resize(m_levelMap.getMapSize().x, m_levelMap.getMapSize().y, m_maxDepth, nullptr);

	octo::Array3D<octo::LevelMap::TileType> const & map = m_levelMap.getMap();
	for (std::size_t z = 0; z < m_tiles.depth(); z++)
	{
		for (std::size_t x = 0; x < m_tiles.columns(); x++)
		{
			for (std::size_t y = 0; y < m_tiles.rows(); y++)
			{
				m_tiles(x, y, z) = new Tile();
				//TODO use TileType in Tile
				m_tiles(x, y, z)->setTileType(static_cast<int>(map(x, y, z)));
				if (map(x, y, z) == octo::LevelMap::TileType::Empty)
					m_tiles(x, y, z)->setIsEmpty(true);
				else
					m_tiles(x, y, z)->setIsEmpty(false);
			}
		}
	}
}

MapInstance::~MapInstance(void)
{
	//for (std::size_t x = 0; x < m_maxDepth; x++)
	//{
	//	for (std::size_t x = 0; x < m_tiles[i].columns(); x++)
	//	{
	//		for (std::size_t y = 0; y < m_tiles[i].rows(); y++)
	//			delete m_tiles[i](x, y);
	//	}
	//}
	//delete [] m_tiles;
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
	switch (tile.getTileType())
	{
		case 2:
			tile.setStartTransition(3u, tile.getStartTransition(3u) + sf::Vector2f(0.f, -Tile::TileSize));
			break;
		case 3:
			tile.setStartTransition(2u, tile.getStartTransition(2u) + sf::Vector2f(0.f, -Tile::TileSize));
			break;
		case 4:
		case 5:
		case 1:
		default:
			break;
	}
}
