#include "MapInstance.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <LevelMap.hpp>
#include <ResourceManager.hpp>

MapInstance::MapInstance(std::size_t position, std::string const & resourceId) :
	m_levelMap(octo::Application::getResourceManager().getLevelMap(resourceId)),
	m_depth(0),
	m_oldDepth(0),
	m_soundPtr(nullptr)
{
	m_cornerPositions.left = position;
	m_cornerPositions.top = -m_levelMap.getMapSize().y + m_levelMap.getMapPosY();
	m_cornerPositions.width = m_cornerPositions.left + m_levelMap.getMapSize().x;
	m_cornerPositions.height = m_cornerPositions.top + m_levelMap.getMapSize().y;
	m_isMapHighlight = m_levelMap.isMapHighlight();

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

void MapInstance::playSound(void)
{
	if (m_soundPtr == nullptr)
	{
		octo::AudioManager& audio = octo::Application::getAudioManager();
		octo::ResourceManager& resources = octo::Application::getResourceManager();
		m_soundPtr = audio.playSound(resources.getSound(OCTO_NO_OGG), 1.f);
	}
	else if (m_soundPtr->getStatus() == sf::SoundSource::Stopped)
		m_soundPtr = nullptr;
}

void MapInstance::nextStep(void)
{
	m_depth++;
	if (m_depth >= static_cast<int>(m_tiles.depth()))
	{
		if (Progress::getInstance().getNextDestination() != Level::JungleA && Progress::getInstance().getNextDestination() != Level::DemoJungleA)
		{
			m_depth = m_tiles.depth() - 1u;
			playSound();
		}
		else
			m_depth = 0;
	}
}

void MapInstance::previousStep(void)
{
	m_depth--;
	if (m_depth < 0)
	{
		if (Progress::getInstance().getNextDestination() != Level::JungleA && Progress::getInstance().getNextDestination() != Level::DemoJungleA)
		{
			m_depth = 0u;
			playSound();
		}
		else
			m_depth = m_tiles.depth() - 1u;
	}
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
