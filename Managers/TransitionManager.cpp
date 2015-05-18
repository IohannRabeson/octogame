#include "TransitionManager.hpp"
#include "MapManager.hpp"

TransitionManager::TransitionManager(void) :
	m_mapManager(nullptr),
	m_tiles(nullptr),
	m_tilesPrev(nullptr),
	m_factoryMap(),
	mf_transitionTimer(1.f),
	mf_transitionTimerMax(0.4f),
	mf_offsetX(0.f),
	mf_offsetY(0.f),
	m_vertices(nullptr),
	mn_verticesCount(0u)
{}

TransitionManager::~TransitionManager(void)
{
	delete m_tiles;
	delete m_tilesPrev;
}

void TransitionManager::init(MapManager * p_mapManager, Biome * p_biome, Map::EMapType p_mapType)
{
	m_mapManager = p_mapManager;

	// TODO: get the right size from VideoMode or something like that
	// Init the factory
	m_factoryMap.init(120u, 68u);

	// Get a map from the factory
	m_tiles = m_factoryMap.getMap(p_mapType);
	m_tilesPrev = m_factoryMap.getMap(p_mapType);
	m_vertices.reset(new sf::Vertex[m_tiles->getRows() * m_tiles->getColumns() * 6u]);

	// Init maps
	m_tiles->init(p_biome);
	m_tilesPrev->init(p_biome);

	mf_transitionTimerMax = p_biome->mf_transitionTimerMax;

	m_baseValue.resize(m_tiles->getColumns(), m_tiles->getRows() * 4u, sf::Vector2f(0.f, 0.f));
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
	setTransitionFull(m_tiles, x, y);
	if (y - 1 >= 0 && m_tiles->get(x, y - 1).isEmpty())
	{
		if (x - 1 >= 0 && m_tiles->get(x - 1, y).isEmpty())
			m_tiles->get(x, y).m_startTransition[0].y += Tile::TileSize;
		if (x + 1 < static_cast<int>(m_tiles->getColumns()) && m_tiles->get(x + 1, y).isEmpty())
			m_tiles->get(x, y).m_startTransition[1].y += Tile::TileSize;
	}
}

void TransitionManager::setTransitionFull(Map * tiles, int x, int y)
{
	sf::Vector2f * ver = tiles->get(x, y).m_startTransition;

	ver[0] = m_baseValue.get(x, y * 4u);
	ver[1] = m_baseValue.get(x, y * 4u + 1u);
	ver[2] = m_baseValue.get(x, y * 4u + 2u);
	ver[3] = m_baseValue.get(x, y * 4u + 3u);
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
	computeDecor();
	defineTransition();
	m_mapManager->getDecorManager().setPosition();
}

// TODO: to be deleted
void lerpColor(sf::Color & p_result, sf::Color & p_start, sf::Color & p_end, float p_transition)
{
	p_result.r = p_start.r * (1.f - p_transition) + p_end.r * p_transition;
	p_result.g = p_start.g * (1.f - p_transition) + p_end.g * p_transition;
	p_result.b = p_start.b * (1.f - p_transition) + p_end.b * p_transition;
}

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
			// Update the vertex for the CollisionManager
			m_tiles->get(x, y).setUpLeft(&m_vertices[mn_verticesCount]);
			std::size_t index = mn_verticesCount;
			for (std::size_t i = 0u; i < 4u; i++)
			{
				// compute interpolation
				lerp(m_vertices[mn_verticesCount].position, m_tilesPrev->get(x, y).m_startTransition[i], m_tiles->get(x, y).m_startTransition[i], transition);
				// add offset to hide border transitions
				m_vertices[mn_verticesCount].position.x += mf_offsetX - Tile::DoubleTileSize;
				m_vertices[mn_verticesCount].position.y += mf_offsetY - Tile::DoubleTileSize;
				// compute interpolation for colors
				lerpColor(m_vertices[mn_verticesCount].color, m_tilesPrev->get(x, y).m_startColor, m_tiles->get(x, y).m_startColor, transition);
				mn_verticesCount++;
			}
			//TODO: find a better way to did it
			sf::Vertex p = m_vertices[index + 1];
			sf::Vertex p2 = m_vertices[index + 2];
			m_vertices[index + 1] = m_vertices[index + 2];
			m_vertices[index + 2] = m_vertices[index + 3];
			m_vertices[index + 3] = m_vertices[index];
			m_vertices[index + 4] = p;
			m_vertices[index + 5] = p2;
			m_vertices[index + 3].color.g += 25;
			m_vertices[index + 4].color.g += 25;
			m_vertices[index + 5].color.g += 25;
			mn_verticesCount += 2;
		}
	}

	auto tiles = m_tiles->getDecors();
	auto tilesPrev = m_tilesPrev->getDecors();
	for (auto it = tiles.begin(), itPrev = tilesPrev.begin(); it != tiles.end(); it++, itPrev++)
	{
		lerp(itPrev->second->mp_upLeft->position, itPrev->second->m_startTransition[0], it->second->m_startTransition[0], transition);
		itPrev->second->mp_upLeft->position.x = mf_offsetX + m_tilesPrev->getOffsetXDecor(it->first) * Tile::TileSize - Tile::DoubleTileSize;
		itPrev->second->mp_upLeft->position.y += mf_offsetY - Tile::DoubleTileSize - m_tilesPrev->getOffsetY() * Tile::TileSize;
		lerpColor(itPrev->second->mp_upLeft->color, itPrev->second->m_startColor, it->second->m_startColor, transition);
	}
}

void TransitionManager::defineTransitionBorderTileRange(int p_startX, int p_endX, int p_startY, int p_endY)
{
	for (int x = p_startX; x < p_endX; x++)
	{
		for (int y = p_startY; y < p_endY; y++)
		{
			setTransitionFull(m_tilesPrev, x, y);
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

void TransitionManager::updateOffset(float pf_deltatime)
{
	float speedbysecond = 400.f;
	float speed = speedbysecond * pf_deltatime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		mf_offsetX += speed;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		mf_offsetX -= speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		mf_offsetY += speed;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		mf_offsetY -= speed;

	if (mf_offsetX >= 16.f)
	{
		mf_offsetX -= 16.f;
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetX(-1);
		m_tilesPrev->addOffsetX(-1);
		m_tiles->computeMapRange(0, 1, 0, m_tiles->getRows());
		m_tilesPrev->computeMapRange(0, 1, 0, m_tiles->getRows());
		defineTransitionBorderTileRange(0, 2, 0, m_tiles->getRows());
		m_tilesPrev->swapDepth();
	}
	else if (mf_offsetX <= -16.f)
	{
		mf_offsetX += 16.f;
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetX(1);
		m_tilesPrev->addOffsetX(1);
		m_tiles->computeMapRange(m_tiles->getColumns() - 1, m_tiles->getColumns(), 0, m_tiles->getRows());
		m_tilesPrev->computeMapRange(m_tiles->getColumns() - 1, m_tiles->getColumns(), 0, m_tiles->getRows());
		defineTransitionBorderTileRange(m_tiles->getColumns() - 2, m_tiles->getColumns(), 0, m_tiles->getRows());
		m_tilesPrev->swapDepth();
	}
	if (mf_offsetY >= 16.f)
	{
		mf_offsetY -= 16.f;
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetY(-1);
		m_tilesPrev->addOffsetY(-1);
		m_tiles->computeMapRange(0, m_tiles->getColumns(), 0, 1);
		m_tilesPrev->computeMapRange(0, m_tiles->getColumns(), 0, 1);
		defineTransitionBorderTileRange(0, m_tiles->getColumns(), 0, 2);
		m_tilesPrev->swapDepth();
	}
	else if (mf_offsetY <= -16.f)
	{
		mf_offsetY += 16.f;
		m_tilesPrev->swapDepth();
		m_tiles->addOffsetY(1);
		m_tilesPrev->addOffsetY(1);
		m_tiles->computeMapRange(0, m_tiles->getColumns(), m_tiles->getRows() - 1, m_tiles->getRows());
		m_tilesPrev->computeMapRange(0, m_tiles->getColumns(), m_tiles->getRows() - 1, m_tiles->getRows());
		defineTransitionBorderTileRange(0, m_tiles->getColumns(), m_tiles->getRows() - (speedbysecond / Tile::TileSize), m_tiles->getRows());
		m_tilesPrev->swapDepth();
	}
}

void TransitionManager::update(float pf_deltatime)
{
	bool compute = false;
	mf_transitionTimer += pf_deltatime;

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

