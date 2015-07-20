#include "GenerativeLayer.hpp"
#include "Tile.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>

GenerativeLayer::GenerativeLayer(void) :
	GenerativeLayer(sf::Color::Yellow, sf::Vector2f(0.5f, 0.5f), sf::Vector2u(512u, 128u), 20.f)
{}

GenerativeLayer::GenerativeLayer(sf::Color const & color, sf::Vector2f const & speed, sf::Vector2u const & mapSize, float transitionDuration) :
	ALayer(speed),
	m_camera(octo::Application::getCamera()),
	m_positions(0u),
	m_positionsPrev(0u),
	m_mapSize(mapSize),
	m_color(color),
	m_widthScreen(octo::Application::getGraphicsManager().getVideoMode().width / Tile::TileSize + 4u),
	m_verticesCount(0u),
	m_tileSize(Tile::TileSize),
	m_depth(0.f),
	m_transitionTimer(0.f),
	m_transitionTimerDuration(transitionDuration)
{
	// Initialize mapSurface pointer
	setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
	{
		return noise.perlinNoise(x, y, 3, 2.f, 5.0f);
	});
}

void GenerativeLayer::init(void)
{
	m_verticesCount = m_widthScreen * 4u;
	m_vertices.reset(new sf::Vertex[m_verticesCount]);
	setColor(m_color);
	m_positions.resize(getMapSize().x * 4);
	m_positionsPrev.resize(getMapSize().x * 4);
	m_noise.setSeed(42);

	computeVertices(m_positions);
	swap();
}

void GenerativeLayer::computeVertices(std::vector<sf::Vector2f> & positions)
{
	// Compute value to manage start/end transition
	float start = getHeightValue((getMapSize().x - 5) * 4);
	float end = getHeightValue(5 * 4);
	for (std::size_t i = 0u; i < getMapSize().x; i++)
	{
		// If we are in start/end transition
		if (i < 5u || i >= getMapSize().x - 5u)
		{
			float transition = i < 5u ? static_cast<float>(i + 5) : static_cast<float>(5 - static_cast<int>(getMapSize().x) + static_cast<float>(i));
			transition = octo::cosinusInterpolation(start, end, transition / 10.f);
			transition = static_cast<int>(transition) - static_cast<int>(transition) % 16;
			positions[(i * 4u) + 0u] = std::move(sf::Vector2f(i * m_tileSize, transition));
			positions[(i * 4u) + 1u] = std::move(sf::Vector2f((i + 1u) * m_tileSize, transition));
		}
		else
		{
			positions[(i * 4u) + 0u] = std::move(sf::Vector2f(i * m_tileSize, getHeightValue(i * 4u)));
			positions[(i * 4u) + 1u] = std::move(sf::Vector2f((i + 1u) * m_tileSize, getHeightValue(i * 4u)));
		}
		positions[(i * 4u) + 2u] = std::move(sf::Vector2f((i + 1u) * m_tileSize, static_cast<float>(getMapSize().y) * m_tileSize));
		positions[(i * 4u) + 3u] = std::move(sf::Vector2f(i * m_tileSize, static_cast<float>(getMapSize().y) * m_tileSize));
	}
	// Set triangles
	for (std::size_t i = 0u; i < getMapSize().x; i++)
	{
		int prev = i == 0 ? getMapSize().x - 1 : i - 1;
		int next = i == getMapSize().x - 1 ? 0 : i + 1;
		if (positions[(prev * 4u + 1u)].y > positions[(i * 4u) + 0u].y)
			positions[(i * 4u) + 0u].y += m_tileSize;
		if (positions[(next * 4u + 0u)].y > positions[(i * 4u) + 1u].y)
			positions[(i * 4u) + 1u].y += m_tileSize;
	}
}

void GenerativeLayer::swap(void)
{
	m_positions.swap(m_positionsPrev);
	m_depth += 0.03f;
	computeVertices(m_positions);
}

float GenerativeLayer::getHeightValue(int x)
{
	int f = (m_backgroundSurface(static_cast<float>(x) / static_cast<float>(getMapSize().x), m_depth) + 1.f) / 2.f * static_cast<float>(getMapSize().y) - 20;
	return (f * static_cast<int>(m_tileSize));
}

void GenerativeLayer::setColor(sf::Color const & color)
{
	for (std::size_t i = 0u; i < m_verticesCount; i++)
		m_vertices[i].color = color;
}

void GenerativeLayer::setBackgroundSurfaceGenerator(BackgroundSurfaceGenerator mapSurface)
{
	m_backgroundSurface = std::bind(mapSurface, std::ref(m_noise), std::placeholders::_1, std::placeholders::_2);
}

void GenerativeLayer::update(float deltatime)
{
	m_transitionTimer += deltatime;
	if (m_transitionTimerDuration > 0.f && m_transitionTimer > m_transitionTimerDuration)
	{
		m_transitionTimer = 0.f;
		swap();
	}
	float transition = m_transitionTimer / m_transitionTimerDuration;
	sf::FloatRect const & rect = m_camera.getRectangle();
	float offsetX = (rect.left * getSpeed().x) / m_tileSize;
	int offsetBackground = static_cast<int>(offsetX);
	offsetX -= static_cast<float>(offsetBackground);
	offsetX *= m_tileSize;
	float offsetY = rect.top * getSpeed().y;
	for (std::size_t i = 0u; i < m_widthScreen; i++)
	{
		if (m_transitionTimerDuration < 0.f)
		{
			m_vertices[(i * 4u) + 0u].position.y = m_positions[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 0u].y + offsetY;
			m_vertices[(i * 4u) + 1u].position.y = m_positions[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 1u].y + offsetY;
		}
		else
		{
			m_vertices[(i * 4u) + 0u].position.y = octo::cosinusInterpolation(m_positionsPrev[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 0u].y, m_positions[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 0u].y, transition) + offsetY;
			m_vertices[(i * 4u) + 1u].position.y = octo::cosinusInterpolation(m_positionsPrev[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 1u].y, m_positions[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 1u].y, transition) + offsetY;
		}
		m_vertices[(i * 4u) + 2u].position.y = m_positions[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 2u].y + offsetY;
		m_vertices[(i * 4u) + 3u].position.y = m_positions[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 3u].y + offsetY;
		m_vertices[(i * 4u) + 0u].position.x = i * m_tileSize + rect.left - offsetX - Tile::DoubleTileSize;
		m_vertices[(i * 4u) + 1u].position.x = (i + 1) * m_tileSize + rect.left - offsetX - Tile::DoubleTileSize;
		m_vertices[(i * 4u) + 2u].position.x = (i + 1) * m_tileSize + rect.left - offsetX - Tile::DoubleTileSize;
		m_vertices[(i * 4u) + 3u].position.x = i * m_tileSize + rect.left - offsetX - Tile::DoubleTileSize;
	}
}

void GenerativeLayer::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), m_verticesCount, sf::Quads, states);
}
