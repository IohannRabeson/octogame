#include "TerrainManager.hpp"
#include "MapManager.hpp"
#include "MapInstance.hpp"
#include "ConvexShape.hpp"
#include "TileShape.hpp"
#include "PhysicsEngine.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <Camera.hpp>

TerrainManager::TerrainManager(void) :
	m_tiles(nullptr),
	m_tilesPrev(nullptr),
	mf_transitionTimer(1.f),
	mf_transitionTimerMax(0.4f),
	m_offset(),
	m_vertices(nullptr),
	mn_verticesCount(0u),
	m_oldOffset(0, 0)
{}

TerrainManager::~TerrainManager(void)
{
	delete m_tiles;
	delete m_tilesPrev;
}

//TODO: remove mapManager
void TerrainManager::init(MapManager *, Biome * p_biome)
{
	m_tiles = new MapInstance();
	m_tilesPrev = new MapInstance();

	// Init maps and biome
	m_tiles->init(p_biome);
	m_tilesPrev->init(p_biome);

	// Set pointer to the offset of the camera
	m_tiles->setCameraView(&m_offset);
	m_tilesPrev->setCameraView(&m_offset);

	// Init vertices
	m_vertices.reset(new sf::Vertex[m_tiles->getRows() * m_tiles->getColumns() * 4u]);

	ShapeBuilder & builder = PhysicsEngine::getShapeBuilder();
	m_tileShapes.resize(m_tiles->getColumns(), m_tiles->getRows(), nullptr);

	for (std::size_t x = 0u; x < m_tiles->getColumns(); x++)
	{
		for (std::size_t y = 0u; y < m_tiles->getRows(); y++)
		{
			m_tileShapes(x, y) = builder.createTileS(x, y);
			m_tileShapes(x, y)->setVertex(&m_vertices[0u]);
		}
	}

	mf_transitionTimerMax = p_biome->mf_transitionTimerMax;
}

void TerrainManager::setTransitionAppear(int x, int y)
{
	int i = 0;
	while (y + i < static_cast<int>(m_tiles->getRows() - 1) && m_tiles->get(x, y + i).me_transition == Tile::e_transition_appear)
		i++;
	for (std::size_t j = 0u; j < 4u; j++)
		m_tilesPrev->get(x, y).m_startTransition[j].y = m_tilesPrev->get(x, y + i).m_startTransition[j].y;
	m_tilesPrev->get(x, y).m_startColor = m_tiles->get(x, y).m_startColor;
	setTransitionModify(x, y);
	// TODO: store color in pointer to avoid copy
}

void TerrainManager::setTransitionDisappear(int x, int y)
{
	int i = 0;
	while (y + i < static_cast<int>(m_tiles->getRows() - 1) && m_tiles->get(x, y + i).me_transition == Tile::e_transition_disappear)
		i++;
	for (std::size_t j = 0u; j < 4u; j++)
		m_tiles->get(x, y).m_startTransition[j].y = m_tiles->get(x, y + i).m_startTransition[j].y;
	m_tiles->get(x, y).m_startColor = m_tilesPrev->get(x, y).m_startColor;
}

void TerrainManager::setTransitionModify(int x, int y)
{
	// Define if it's a quad or a triangle
	if (y - 1 >= 0 && m_tiles->get(x, y - 1).isEmpty() && y + 1 < static_cast<int>(m_tiles->getRows()))
	{
		if (x - 1 >= 0 && m_tiles->get(x - 1, y).isEmpty())
			m_tiles->get(x, y).m_startTransition[0].y = m_tiles->get(x, y + 1).m_startTransition[0].y;
		if (x + 1 < static_cast<int>(m_tiles->getColumns()) && m_tiles->get(x + 1, y).isEmpty())
			m_tiles->get(x, y).m_startTransition[1].y = m_tiles->get(x, y + 1).m_startTransition[1].y;
	}
}

void TerrainManager::defineTransition(int x, int y)
{
	int prev = m_tilesPrev->get(x, y).isEmpty();
	int current = m_tiles->get(x, y).isEmpty();

	if (prev && !current) // appear
		m_tiles->get(x, y).me_transition = Tile::e_transition_appear;
	else if (!prev && current) // disappear
		m_tiles->get(x, y).me_transition = Tile::e_transition_disappear;
	else if (!current && !prev) // already a tile
		m_tiles->get(x, y).me_transition = Tile::e_transition_already;
	else // no tile
		m_tiles->get(x, y).me_transition = Tile::e_transition_none;
}

void TerrainManager::defineTransitionRange(int p_startX, int p_endX, int p_startY, int p_endY)
{
	// For each tile, define the type and transition type
	for (int x = p_startX; x < p_endX; x++)
	{
		for (int y = p_startY; y < p_endY; y++)
			defineTransition(x, y);
	}
	for (int x = p_startX; x < p_endX; x++)
	{
		for (int y = p_startY; y < p_endY; y++)
		{
			if (m_tiles->get(x, y).me_transition == Tile::e_transition_appear)
				setTransitionAppear(x, y);
		}
	}
	for (int x = p_startX; x < p_endX; x++)
	{
		for (int y = p_startY; y < p_endY; y++)
		{
			if (m_tiles->get(x, y).me_transition == Tile::e_transition_already)
				setTransitionModify(x, y);
		}
	}
	for (int x = p_startX; x < p_endX; x++)
	{
		for (int y = p_startY; y < p_endY; y++)
		{
			if (m_tiles->get(x, y).me_transition == Tile::e_transition_disappear)
				setTransitionDisappear(x, y);
		}
	}
}

void TerrainManager::swapMap(void)
{
	Map * tmp = m_tilesPrev;
	m_tilesPrev = m_tiles;
	m_tiles = tmp;
	m_tiles->computeMap();
	computeDecor();
	defineTransition();
}

void TerrainManager::updateTransition(void)
{
	if (mf_transitionTimer > mf_transitionTimerMax)
		mf_transitionTimer = mf_transitionTimerMax;
	float transition = mf_transitionTimer / mf_transitionTimerMax;
	Tile * tile;
	Tile * tilePrev;

	// Update tiles
	mn_verticesCount = 0u;
	for (std::size_t x = 0u; x < m_tiles->getColumns(); x++)
	{
		bool isFirst = false;
		for (std::size_t y = 0u; y < m_tiles->getRows(); y++)
		{
			tile = &m_tiles->get(x, y);
			if (m_tiles->get(x, y).me_transition == Tile::e_transition_none)
			{
				m_tileShapes.get(x, y)->setSleep(true);
				continue;
			}
			tilePrev = &m_tilesPrev->get(x, y);
			//TODO: better managment of tiles
			m_vertices[mn_verticesCount].position = octo::linearInterpolation(tilePrev->m_startTransition[0u], tile->m_startTransition[0u], transition);
			m_vertices[mn_verticesCount + 1u].position = octo::linearInterpolation(tilePrev->m_startTransition[1u], tile->m_startTransition[1u], transition);
			m_vertices[mn_verticesCount + 2u].position = octo::linearInterpolation(tilePrev->m_startTransition[2u], tile->m_startTransition[2u], transition);
			m_vertices[mn_verticesCount + 3u].position = octo::linearInterpolation(tilePrev->m_startTransition[3u], tile->m_startTransition[3u], transition);
			m_vertices[mn_verticesCount].color = octo::linearInterpolation(tilePrev->m_startColor, tile->m_startColor, transition);
			for (std::size_t i = 0u; i < 4u; i++)
			{
				m_vertices[mn_verticesCount + i].position.x -= Tile::DoubleTileSize;
				m_vertices[mn_verticesCount + i].position.y -= Tile::DoubleTileSize;
				m_vertices[mn_verticesCount + i].color = m_vertices[mn_verticesCount].color;
			}
			if (!isFirst)
			{
				isFirst = true;
				m_tiles->get(x, y).setPolygonVertices(&m_vertices[mn_verticesCount]);
				m_tileShapes.get(x, y)->setSleep(false);
				m_tileShapes.get(x, y)->setVertex(&m_vertices[mn_verticesCount]);
				m_tiles->get(x, y).m_startColor = sf::Color::Blue;
			}
			else
			{
				if (x + 1u < m_tiles->getColumns() && m_tiles->get(x + 1, y).me_transition == Tile::e_transition_none)
				{
					m_tiles->get(x, y).setPolygonVertices(&m_vertices[mn_verticesCount]);
					m_tileShapes.get(x, y)->setSleep(false);
					m_tileShapes.get(x, y)->setVertex(&m_vertices[mn_verticesCount]);
					m_tiles->get(x, y).m_startColor = sf::Color::Red;
				}
				else if (x > 0u && m_tiles->get(x - 1, y).me_transition == Tile::e_transition_none)
				{
					m_tiles->get(x, y).setPolygonVertices(&m_vertices[mn_verticesCount]);
					m_tileShapes.get(x, y)->setSleep(false);
					m_tileShapes.get(x, y)->setVertex(&m_vertices[mn_verticesCount]);
					m_tiles->get(x, y).m_startColor = sf::Color::Red;
				}
				else
					m_tileShapes.get(x, y)->setSleep(false);
			}
			mn_verticesCount += 4u;
		}
	}

	// Update decors
	auto tiles = m_tiles->getDecors();
	auto tilesPrev = m_tilesPrev->getDecors();
	for (auto it = tiles.begin(), itPrev = tilesPrev.begin(); it != tiles.end(); it++, itPrev++)
	{
		itPrev->second->mp_upLeft->position.y = octo::linearInterpolation(itPrev->second->m_startTransition[0].y, it->second->m_startTransition[0].y, transition) - Tile::DoubleTileSize;
		itPrev->second->mp_upLeft->position.x = it->second->m_startTransition[0].x - Tile::DoubleTileSize;
		itPrev->second->mp_upLeft->color = octo::linearInterpolation(itPrev->second->m_startColor, it->second->m_startColor, transition);
	}
}

void TerrainManager::defineTransitionBorderTileRange(int p_startX, int p_endX, int p_startY, int p_endY)
{
	for (int x = p_startX; x < p_endX; x++)
	{
		for (int y = p_startY; y < p_endY; y++)
		{
			if (y - 1 >= 0 && y + 1 < static_cast<int>(m_tiles->getRows()))
			{
				// Define triangle/quad
				if (m_tiles->get(x, y - 1).isEmpty())
				{
					if (x - 1 >= 0 && m_tiles->get(x - 1, y).isEmpty())
						m_tiles->get(x, y).m_startTransition[0].y = m_tiles->get(x, y + 1).m_startTransition[0].y;
					if (x + 1 < static_cast<int>(m_tiles->getColumns()) && m_tiles->get(x + 1, y).isEmpty())
						m_tiles->get(x, y).m_startTransition[1].y = m_tiles->get(x, y + 1).m_startTransition[1].y;
				}
				if (m_tilesPrev->get(x, y - 1).isEmpty())
				{
					if (x - 1 >= 0 && m_tilesPrev->get(x - 1, y).isEmpty())
						m_tilesPrev->get(x, y).m_startTransition[0].y = m_tilesPrev->get(x, y + 1).m_startTransition[0].y;
					if (x + 1 < static_cast<int>(m_tilesPrev->getColumns()) && m_tilesPrev->get(x + 1, y).isEmpty())
						m_tilesPrev->get(x, y).m_startTransition[1].y = m_tilesPrev->get(x, y + 1).m_startTransition[1].y;
				}
			}
		}
	}
	defineTransitionRange(p_startX, p_endX, p_startY, p_endY);
}

void TerrainManager::updateOffset(float)
{
	int ofX = 0;
	int ofY = 0;
	int newOfX = static_cast<int>(m_offset.x / Tile::TileSize);
	int newOfY = static_cast<int>(m_offset.y / Tile::TileSize);
	if (m_oldOffset.x > newOfX)
		ofX = -1;
	else if (m_oldOffset.x < newOfX)
		ofX = 1;
	if (m_oldOffset.y > newOfY)
		ofY = -1;
	else if (m_oldOffset.y < newOfY)
		ofY = 1;

	// TODO: there is a bug if the speed > 16.f / second
	// if speed = 600.f at 60fps, speed = 600.f / 60.f = 10.f per frame
	// if speed per frame > 16.f the bug occur
	// to trigger the bug, at 60 fps, speed must be 60 * 16 = 960
	if (ofX)
		computeDecor();
	if (ofX && ofY)
	{
		m_tiles->registerOffset();
		m_tilesPrev->registerOffset();
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetX(ofX);
		m_tilesPrev->addOffsetX(ofX);
		m_tiles->addOffsetY(ofY);
		m_tilesPrev->addOffsetY(ofY);
		if (ofX < 0)
		{
			m_tiles->computeMapRangeX(0, 1);
			m_tilesPrev->computeMapRangeX(0, 1);
			if (ofY < 0)
			{
				m_tiles->computeMapRangeY(0, 1);
				m_tilesPrev->computeMapRangeY(0, 1);
				defineTransitionBorderTileRange(0, 2, 2, m_tiles->getRows());
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), 0, 2);
			}
			else
			{
				m_tiles->computeMapRangeY(m_tiles->getRows() - 1, m_tiles->getRows());
				m_tilesPrev->computeMapRangeY(m_tiles->getRows() - 1, m_tiles->getRows());
				defineTransitionBorderTileRange(0, 2, 0, m_tiles->getRows() - 2);
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), m_tiles->getRows() - 2, m_tiles->getRows());
			}
		}
		else
		{
			m_tiles->computeMapRangeX(m_tiles->getColumns() - 1, m_tiles->getColumns());
			m_tilesPrev->computeMapRangeX(m_tiles->getColumns() - 1, m_tiles->getColumns());
			if (ofY < 0)
			{
				m_tiles->computeMapRangeY(0, 1);
				m_tilesPrev->computeMapRangeY(0, 1);
				defineTransitionBorderTileRange(m_tiles->getColumns() - 2, m_tiles->getColumns(), 2, m_tiles->getRows());
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), 0, 2);
			}
			else
			{
				m_tiles->computeMapRangeY(m_tiles->getRows() - 1, m_tiles->getRows());
				m_tilesPrev->computeMapRangeY(m_tiles->getRows() - 1, m_tiles->getRows());
				defineTransitionBorderTileRange(m_tiles->getColumns() - 2, m_tiles->getColumns(), 0, m_tiles->getRows() - 2);
				defineTransitionBorderTileRange(0, m_tiles->getColumns(), m_tiles->getRows() - 2, m_tiles->getRows());
			}
		}
		m_tilesPrev->swapDepth();
		m_oldOffset.x = newOfX;
		m_oldOffset.y = newOfY;
	}
	else if (ofX)
	{
		m_tiles->registerOffset();
		m_tilesPrev->registerOffset();
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetX(ofX);
		m_tilesPrev->addOffsetX(ofX);
		if (ofX < 0)
		{
			m_tiles->computeMapRangeX(0, 1);
			m_tilesPrev->computeMapRangeX(0, 1);
			defineTransitionBorderTileRange(0, 2, 0, m_tiles->getRows());
		}
		else
		{
			m_tiles->computeMapRangeX(m_tiles->getColumns() - 1, m_tiles->getColumns());
			m_tilesPrev->computeMapRangeX(m_tiles->getColumns() - 1, m_tiles->getColumns());
			defineTransitionBorderTileRange(m_tiles->getColumns() - 2, m_tiles->getColumns(), 0, m_tiles->getRows());
		}
		m_tilesPrev->swapDepth();
		m_oldOffset.x = newOfX;
	}
	else if (ofY)
	{
		m_tiles->registerOffset();
		m_tilesPrev->registerOffset();
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetY(ofY);
		m_tilesPrev->addOffsetY(ofY);
		if (ofY < 0)
		{
			m_tiles->computeMapRangeY(0, 1);
			m_tilesPrev->computeMapRangeY(0, 1);
			defineTransitionBorderTileRange(0, m_tiles->getColumns(), 0, 2);
		}
		else
		{
			m_tiles->computeMapRangeY(m_tiles->getRows() - 1, m_tiles->getRows());
			m_tilesPrev->computeMapRangeY(m_tiles->getRows() - 1, m_tiles->getRows());
			defineTransitionBorderTileRange(0, m_tiles->getColumns(), m_tiles->getRows() - 10, m_tiles->getRows());
		}
		m_tilesPrev->swapDepth();
		m_oldOffset.y = newOfY;
	}
}

void TerrainManager::update(float pf_deltatime)
{
	bool compute = false;
	mf_transitionTimer += pf_deltatime;

	// Get the top left of the camera view
	sf::Rect<float> const & rect = octo::Application::getCamera().getRectangle();
	m_offset.x = rect.left;
	m_offset.y = rect.top;

	if (mf_transitionTimer >= mf_transitionTimerMax)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			compute = true;
			m_tilesPrev->nextStep();
			m_tiles->registerDepth();
			m_tiles->nextStep();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			compute = true;
			m_tilesPrev->previousStep();
			m_tiles->registerDepth();
			m_tiles->previousStep();
		}
		if (compute)
		{
			mf_transitionTimer = 0.f;
			swapMap();
		}
	}
	updateOffset(pf_deltatime);
	updateTransition();
}

void TerrainManager::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), mn_verticesCount, sf::Quads, states);
}
