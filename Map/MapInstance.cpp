#include "MapInstance.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <LevelMap.hpp>
#include <ResourceManager.hpp>

MapInstance::MapInstance(std::size_t position, std::string const & resourceId) :
	m_levelMap(octo::Application::getResourceManager().getLevelMap(resourceId)),
	m_reverse(false),
	m_isMapHighlight(true), // TODO: Useless, to remove
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
	m_movementMask.resize(map.columns(), map.rows(), 2u);

	for (std::size_t x = 0; x < m_tiles.columns(); x++)
	{
		for (std::size_t y = 0; y < m_tiles.rows(); y++)
		{
			octo::LevelMap::TileType type = map(x, y, 0);
			for (std::size_t z = 0; z < m_tiles.depth(); z++)
			{
				//TODO use TileType in Tile
				m_tiles(x, y, z).setTileType(map(x, y, z));
				if (map(x, y, z) == octo::LevelMap::TileType::Empty)
					m_tiles(x, y, z).setIsEmpty(true);
				else
					m_tiles(x, y, z).setIsEmpty(false);

				// Pre compute radiance mask
				if (map(x, y, z) != type || m_movementMask(x, y, 1) == 1.f)
				{
					m_movementMask(x, y, 0) = 1.f; // Tile changing
					m_movementMask(x, y, 1) = 1.f; // Tile has changed
				}
				else if (map(x, y, z) == octo::LevelMap::TileType::Empty)
					m_movementMask(x, y, 0) = -1.f; // Tile empty and not changing
				else
					m_movementMask(x, y, 0) = 0.f; // Tile full not changing
			}
		}
	}
	computeRadianceMask(2u, 0.1f);
	smoothBorder();
}

//TODO: Can be factorized if performance gain is necessary
void MapInstance::smoothBorder(void)
{
	std::size_t attenuateBorder = 20u;
	for (std::size_t x = 0; x < m_movementMask.columns(); x++)
	{
		for (std::size_t y = 0; y < m_movementMask.rows(); y++)
		{
			if (x < attenuateBorder)
				m_movementMask(x, y, 0) = m_movementMask(x, y, 0) * (1.f - (x / attenuateBorder));
			else if (x - m_movementMask.columns() < attenuateBorder)
				m_movementMask(x, y, 0) = m_movementMask(x, y, 0) * (1.f - ((x - m_movementMask.columns()) / attenuateBorder));
			if (y < attenuateBorder)
				m_movementMask(x, y, 0) = m_movementMask(x, y, 0) * (1.f - (y / attenuateBorder));
			else if (y - m_movementMask.rows() < attenuateBorder)
				m_movementMask(x, y, 0) = m_movementMask(x, y, 0) * (1.f - ((y - m_movementMask.columns()) / attenuateBorder));
		}
	}
}

void MapInstance::computeRadianceMask(std::size_t depth, float attenuate)
{
	bool invert = false;
	for (std::size_t i = 0u; i < depth; i++)
	{
		if (invert == false)
		{
			for (std::size_t x = 1; x < m_movementMask.columns() - 1; x++)
			{
				for (std::size_t y = 1; y < m_movementMask.rows() - 1; y++)
					averageVarianceFour(x, y, attenuate);
			}
			invert = true;
		}
		else
		{
			for (std::size_t x = m_movementMask.columns() - 1; x > 0 ; x--)
			{
				for (std::size_t y = 1; y < m_movementMask.rows() - 1; y++)
					averageVarianceFour(x, y, attenuate);
			}
			invert = false;
		}
	}
}

//average value on the 4 point around
void MapInstance::averageVarianceFour(std::size_t x, std::size_t y, float attenuate)
{
	if (m_movementMask(x, y, 0) > 0.f && m_movementMask(x, y, 0) <= 1.f)
	{
		averageVarianceOne(x, y, x - 1, y, attenuate);
		averageVarianceOne(x, y, x + 1, y, attenuate);
		averageVarianceOne(x, y, x, y - 1, attenuate);
		averageVarianceOne(x, y, x, y + 1, attenuate);
	}
}

void MapInstance::averageVarianceOne(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY, float attenuate)
{
	if (m_movementMask(destX, destY, 0) >= 0.f && (m_movementMask(srcX, srcY, 0) + m_movementMask(destX, destY, 0)) / (2.f - attenuate) < 1.f)
		m_movementMask(destX, destY, 0) = (m_movementMask(srcX, srcY, 0) + m_movementMask(destX, destY, 0)) / (2.f - attenuate);
	else if (m_movementMask(destX, destY, 0) > 0.f)
		m_movementMask(destX, destY, 0) = 1.f;
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

bool MapInstance::nextStep(void)
{
	Progress & progress = Progress::getInstance();

	if (m_depth >= static_cast<int>(m_tiles.depth() - 1)
		&& (progress.getCurrentDestination() != Level::Random || m_depth < static_cast<int>(progress.countRandomDiscover())))
		m_reverse = true;
	else if (m_depth <= 0)
		m_reverse = false;

	if (m_reverse == false)
		m_depth++;
	else
		m_depth--;

	return true;
}

bool MapInstance::previousStep(void)
{
	Progress & progress = Progress::getInstance();

	if (m_depth <= 0)
		m_reverse = true;
	else if (m_depth >= static_cast<int>(m_tiles.depth() - 1)  && (progress.getCurrentDestination() != Level::Random || m_depth < static_cast<int>(progress.countRandomDiscover())))
		m_reverse = false;

	if (m_reverse == false)
		m_depth--;
	else
		m_depth++;

	return true;

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
