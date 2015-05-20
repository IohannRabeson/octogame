#include "TransitionManager.hpp"
#include "MapManager.hpp"
#include "MapInstance.hpp"
#include "MapHigh.hpp"

TransitionManager::TransitionManager(void) :
	m_mapManager(nullptr),
	m_tiles(nullptr),
	m_tilesPrev(nullptr),
	mf_transitionTimer(1.f),
	mf_transitionTimerMax(0.4f),
	mf_offsetX(0.f),
	mf_offsetY(0.f),
	m_offset(),
	m_vertices(nullptr),
	mn_verticesCount(0u)
{}

TransitionManager::~TransitionManager(void)
{
	delete m_tiles;
	delete m_tilesPrev;
}

void TransitionManager::init(MapManager * p_mapManager, Biome * p_biome)
{
	m_mapManager = p_mapManager;

	m_tiles = new MapHigh();
	m_tilesPrev = new MapHigh();

	// Init maps
	m_tiles->init(p_biome);
	m_tilesPrev->init(p_biome);

	m_vertices.reset(new sf::Vertex[m_tiles->getRows() * m_tiles->getColumns() * 6u]);

	//m_tiles->setOffset(p_biome->mn_startOffsetX - 60, 0);
	//m_tilesPrev->setOffset(p_biome->mn_startOffsetX - 60, 0);

	mf_transitionTimerMax = p_biome->mf_transitionTimerMax;

	m_baseValue.resize(m_tiles->getColumns(), m_tiles->getRows() * 4u, sf::Vector2f());
	// Init base values
	for (std::size_t x = 0u; x < m_tiles->getColumns(); x++)
	{
		for (std::size_t y = 0u; y < m_tiles->getRows(); y++)
		{
			m_baseValue.get(x, y * 4u + 0u) = sf::Vector2f(x * Tile::TileSize, y * Tile::TileSize);
			m_baseValue.get(x, y * 4u + 1u) = sf::Vector2f(x * Tile::TileSize + Tile::TileSize, y * Tile::TileSize);
			m_baseValue.get(x, y * 4u + 2u) = sf::Vector2f(x * Tile::TileSize + Tile::TileSize, y * Tile::TileSize + Tile::TileSize);
			m_baseValue.get(x, y * 4u + 3u) = sf::Vector2f(x * Tile::TileSize, y * Tile::TileSize + Tile::TileSize);
		}
	}
}

void TransitionManager::setTransitionAppear(int x, int y)
{
	int i = 0;
	while (y + i < static_cast<int>(m_tiles->getRows() - 1) && m_tiles->get(x, y + i).me_transition == Tile::e_transition_appear)
		i++;
	for (std::size_t j = 0u; j < 4u; j++)
		m_tilesPrev->get(x, y).m_startTransition[j].y = m_tilesPrev->get(x, y + i).m_startTransition[j].y;
	setTransitionModify(x, y);
	// TODO: store color in pointer to avoid copy
	m_tilesPrev->get(x, y).m_startColor = m_tiles->get(x, y).m_startColor;
}

void TransitionManager::setTransitionDisappear(int x, int y)
{
	int i = 0;
	while (y + i < static_cast<int>(m_tiles->getRows() - 1) && m_tiles->get(x, y + i).me_transition == Tile::e_transition_disappear)
		i++;
	for (std::size_t j = 0u; j < 4u; j++)
		m_tiles->get(x, y).m_startTransition[j].y = m_tiles->get(x, y + i).m_startTransition[j].y;
}

void TransitionManager::setTransitionModify(int x, int y)
{
	// define if it's a quad or a triangle
	if (y - 1 >= 0 && m_tiles->get(x, y - 1).isEmpty())
	{
		if (x - 1 >= 0 && m_tiles->get(x - 1, y).isEmpty())
			m_tiles->get(x, y).m_startTransition[0].y += Tile::TileSize;
		if (x + 1 < static_cast<int>(m_tiles->getColumns()) && m_tiles->get(x + 1, y).isEmpty())
			m_tiles->get(x, y).m_startTransition[1].y += Tile::TileSize;
	}
}

void TransitionManager::defineTransition(int x, int y)
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

void TransitionManager::defineTransition(void)
{
	defineTransitionRange(0, m_tiles->getColumns(), 0, m_tiles->getRows());
}

void TransitionManager::defineTransitionRange(int p_startX, int p_endX, int p_startY, int p_endY)
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

// TODO: use octo::linearInterpolation
void TransitionManager::lerp(sf::Vector2f & p_result, sf::Vector2f & p_start, sf::Vector2f & p_end, float p_transition)
{
	p_result = p_start * (1.f - p_transition) + p_end * p_transition;
}

void TransitionManager::swapMap(void)
{
	Map * tmp = m_tilesPrev;
	m_tilesPrev = m_tiles;
	m_tiles = tmp;
	m_tiles->computeMap();
	//computeDecor();
	defineTransition();
	//m_mapManager->getDecorManager().setPosition();
}

// TODO: to be deleted
void lerpColor(sf::Color & p_result, sf::Color & p_start, sf::Color & p_end, float p_transition)
{
	p_result.r = p_start.r * (1.f - p_transition) + p_end.r * p_transition;
	p_result.g = p_start.g * (1.f - p_transition) + p_end.g * p_transition;
	p_result.b = p_start.b * (1.f - p_transition) + p_end.b * p_transition;
}

#include <iostream>
void TransitionManager::updateTransition(float pf_deltatime)
{
	if (mf_transitionTimer > mf_transitionTimerMax)
	{
		pf_deltatime = mf_transitionTimer - mf_transitionTimerMax;
		mf_transitionTimer = mf_transitionTimerMax;
	}
	float transition = mf_transitionTimer / mf_transitionTimerMax;

	mn_verticesCount = 0u;
	for (std::size_t x = 0u; x < m_tiles->getColumns(); x++)
	{
		for (std::size_t y = 0u; y < m_tiles->getRows(); y++)
		{
			if (m_tiles->get(x, y).me_transition == Tile::e_transition_none)
				continue;
			lerp(m_vertices[mn_verticesCount].position, m_tilesPrev->get(x, y).m_startTransition[0u], m_tiles->get(x, y).m_startTransition[0u], transition);
			lerp(m_vertices[mn_verticesCount + 1u].position, m_tilesPrev->get(x, y).m_startTransition[2u], m_tiles->get(x, y).m_startTransition[2u], transition);
			lerp(m_vertices[mn_verticesCount + 2u].position, m_tilesPrev->get(x, y).m_startTransition[3u], m_tiles->get(x, y).m_startTransition[3u], transition);
			lerp(m_vertices[mn_verticesCount + 4u].position, m_tilesPrev->get(x, y).m_startTransition[1u], m_tiles->get(x, y).m_startTransition[1u], transition);
			m_vertices[mn_verticesCount + 3u].position = m_vertices[mn_verticesCount].position;
			m_vertices[mn_verticesCount + 5u].position = m_vertices[mn_verticesCount + 1u].position;
			for (std::size_t i = 0; i < 6u; i++)
				lerpColor(m_vertices[mn_verticesCount + i].color, m_tilesPrev->get(x, y).m_startColor, m_tiles->get(x, y).m_startColor, transition);
			mn_verticesCount += 6u;
		}
	}

/*
	auto tiles = m_tiles->getDecors();
	auto tilesPrev = m_tilesPrev->getDecors();
	for (auto it = tiles.begin(), itPrev = tilesPrev.begin(); it != tiles.end(); it++, itPrev++)
	{
		lerp(itPrev->second->mp_upLeft->position, itPrev->second->m_startTransition[0], it->second->m_startTransition[0], transition);
		itPrev->second->mp_upLeft->position.x = mf_offsetX + m_tilesPrev->getOffsetXDecor(it->first) * Tile::TileSize - Tile::DoubleTileSize;
		itPrev->second->mp_upLeft->position.y += mf_offsetY - Tile::DoubleTileSize - m_tilesPrev->getOffsetY() * Tile::TileSize;
		lerpColor(itPrev->second->mp_upLeft->color, itPrev->second->m_startColor, it->second->m_startColor, transition);
	}*/
}

void TransitionManager::defineTransitionBorderTileRange(int p_startX, int p_endX, int p_startY, int p_endY)
{
	for (int x = p_startX; x < p_endX; x++)
	{
		for (int y = p_startY; y < p_endY; y++)
		{
			if (y - 1 >= 0 && m_tilesPrev->get(x, y - 1).isEmpty())
			{
				if (x - 1 >= 0 && m_tilesPrev->get(x - 1, y).isEmpty())
					m_tilesPrev->get(x, y).m_startTransition[0].y += Tile::TileSize;
				if (x + 1 < static_cast<int>(m_tilesPrev->getColumns()) && m_tilesPrev->get(x + 1, y).isEmpty())
					m_tilesPrev->get(x, y).m_startTransition[1].y += Tile::TileSize;
			}
		}
	}
	defineTransitionRange(p_startX, p_endX, p_startY, p_endY);
}

void TransitionManager::updateOffset(float)
{
	//float speed = speedbysecond * pf_deltatime;
	static sf::Vector2f old = m_offset;
	// TODO: compute border when we move
	// TODO: use getter for tiles instead of setter to avoid copy
/*	if (m_offsetX != static_cast<int>(m_offset.x) || m_offsetY != static_cast<int>(m_offset.y))
	{
		m_offsetX = static_cast<int>(m_offset.x);
		m_offsetY = static_cast<int>(m_offset.y);
		m_tilesPrev->swapDepth();
		m_tiles->computeMap();
		m_tilesPrev->computeMap();
		defineTransition();
		m_tilesPrev->swapDepth();
	}
*/
	mf_offsetX += old.x - m_offset.x;
	mf_offsetY += old.y - m_offset.y;
	old = m_offset;

	if (mf_offsetX >= Tile::TileSize)
	{
		mf_offsetX -= Tile::TileSize;
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetX(-1);
		m_tilesPrev->addOffsetX(-1);
		defineTransitionBorderTileRange(0, 2, 0, m_tiles->getRows());
		m_tilesPrev->swapDepth();
	}
	else if (mf_offsetX <= -Tile::TileSize)
	{
		mf_offsetX += Tile::TileSize;
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetX(1);
		m_tilesPrev->addOffsetX(1);
		defineTransitionBorderTileRange(m_tiles->getColumns() - 2, m_tiles->getColumns(), 0, m_tiles->getRows());
		m_tilesPrev->swapDepth();
	}
	if (mf_offsetY >= Tile::TileSize)
	{
		mf_offsetY -= Tile::TileSize;
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetY(-1);
		m_tilesPrev->addOffsetY(-1);
		defineTransitionBorderTileRange(0, m_tiles->getColumns(), 0, 2);
		m_tilesPrev->swapDepth();
	}
	else if (mf_offsetY <= -Tile::TileSize)
	{
		mf_offsetY += Tile::TileSize;
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetY(1);
		m_tilesPrev->addOffsetY(1);
		defineTransitionBorderTileRange(0, m_tiles->getColumns(), m_tiles->getRows() - 2, m_tiles->getRows());
		m_tilesPrev->swapDepth();
	}
}

void TransitionManager::update(float pf_deltatime)
{
	bool compute = false;
	mf_transitionTimer += pf_deltatime;

	m_offset = m_mapManager->getCameraManager().getUpLeft();
	m_tiles->setOffset(m_offset);
	m_tilesPrev->setOffset(m_offset);

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
	updateTransition(pf_deltatime);
}

void TransitionManager::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), mn_verticesCount, sf::Triangles, states);
}

