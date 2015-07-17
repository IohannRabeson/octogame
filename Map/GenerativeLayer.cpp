#include "GenerativeLayer.hpp"
#include "Tile.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>

GenerativeLayer::GenerativeLayer(void) :
	GenerativeLayer(sf::Color::Yellow, sf::Vector2f(0.5f, 0.5f))
{}

GenerativeLayer::GenerativeLayer(sf::Color const & color, sf::Vector2f const & speed) :
	ALayer(speed),
	m_camera(octo::Application::getCamera()),
	m_mapSize(512u, 128u),
	m_widthScreen(octo::Application::getGraphicsManager().getVideoMode().width / Tile::TileSize + 4u),
	m_positions(m_mapSize.x * 4),
	m_tileSize(Tile::TileSize),
	m_color(color)
{
	// Initialize mapSurface pointer
	setBackgroundSurfaceGenerator([](Noise & noise, float x)
	{
		return noise.perlinNoise(x, 0.028f, 3, 2.f, 5.0f);
	});

	init();
}

void GenerativeLayer::init(void)
{
	m_vertices.reset(new sf::Vertex[m_mapSize.x * 4]);
	m_noise.setSeed(42);

	// Compute value to manage start/end transition
	float start = getHeightValue((m_mapSize.x - 5) * 4);
	float end = getHeightValue(5 * 4);
	for (std::size_t i = 0u; i < m_mapSize.x; i++)
	{
		// If we are in start/end transition
		if (i < 5u || i >= m_mapSize.x - 5u)
		{
			float transition = i < 5u ? static_cast<float>(i + 5) : static_cast<float>(5 - static_cast<int>(m_mapSize.x) + static_cast<float>(i));
			transition = octo::cosinusInterpolation(start, end, transition / 10.f);
			transition = static_cast<int>(transition) - static_cast<int>(transition) % 16;
			m_positions[(i * 4u) + 0u] = sf::Vector2f(i * m_tileSize, transition);
			m_positions[(i * 4u) + 1u] = sf::Vector2f((i + 1u) * m_tileSize, transition);
		}
		else
		{
			m_positions[(i * 4u) + 0u] = sf::Vector2f(i * m_tileSize, getHeightValue(i * 4u));
			m_positions[(i * 4u) + 1u] = sf::Vector2f((i + 1u) * m_tileSize, getHeightValue(i * 4u));
		}
		m_positions[(i * 4u) + 2u] = sf::Vector2f((i + 1u) * m_tileSize, static_cast<float>(m_mapSize.y) * m_tileSize);
		m_positions[(i * 4u) + 3u] = sf::Vector2f(i * m_tileSize, static_cast<float>(m_mapSize.y) * m_tileSize);

		for (std::size_t j = 0u; j < 4u; j++)
		{
			m_vertices[(i * 4u) + j].position = m_positions[(i * 4u) + j];
			m_vertices[(i * 4u) + j].color = m_color;
		}
	}
	// Set triangles
	for (std::size_t i = 0u; i < m_mapSize.x; i++)
	{
		int prev = i == 0 ? m_mapSize.x - 1 : i - 1;
		int next = i == m_mapSize.x - 1 ? 0 : i + 1;
		if (m_positions[(prev * 4u + 1u)].y > m_positions[(i * 4u) + 0u].y)
			m_positions[(i * 4u) + 0u].y += m_tileSize;
		if (m_positions[(next * 4u + 0u)].y > m_positions[(i * 4u) + 1u].y)
			m_positions[(i * 4u) + 1u].y += m_tileSize;
	}
}

float GenerativeLayer::getHeightValue(int x)
{
	int f = (m_backgroundSurface(static_cast<float>(x) / static_cast<float>(m_mapSize.x)) + 1.f) / 2.f * static_cast<float>(m_mapSize.y) - 20;
	return (f * static_cast<int>(m_tileSize));
}

void GenerativeLayer::setColor(sf::Color const & color)
{
	for (std::size_t i = 0u; i < m_widthScreen * 4u; i++)
		m_vertices[i].color = color;
}

void GenerativeLayer::setBackgroundSurfaceGenerator(BackgroundSurfaceGenerator mapSurface)
{
	m_backgroundSurface = std::bind(mapSurface, std::ref(m_noise), std::placeholders::_1);
}

void GenerativeLayer::update(float)
{
	sf::FloatRect const & rect = m_camera.getRectangle();
	float offset = (rect.left * getSpeed().x) / m_tileSize;
	int offsetBackground = static_cast<int>(offset);
	offset -= static_cast<float>(offsetBackground);
	offset *= m_tileSize;
	float offsetY = rect.top * getSpeed().y;
	for (std::size_t i = 0u; i < m_widthScreen; i++)
	{
		m_vertices[(i * 4u) + 0u].position.y = m_positions[((offsetBackground + i) * 4u) % (m_mapSize.x * 4) + 0u].y + offsetY;
		m_vertices[(i * 4u) + 1u].position.y = m_positions[((offsetBackground + i) * 4u) % (m_mapSize.x * 4) + 1u].y + offsetY;
		m_vertices[(i * 4u) + 2u].position.y = m_positions[((offsetBackground + i) * 4u) % (m_mapSize.x * 4) + 2u].y + offsetY;
		m_vertices[(i * 4u) + 3u].position.y = m_positions[((offsetBackground + i) * 4u) % (m_mapSize.x * 4) + 3u].y + offsetY;
		m_vertices[(i * 4u) + 0u].position.x = i * m_tileSize + rect.left - offset - Tile::DoubleTileSize;
		m_vertices[(i * 4u) + 1u].position.x = (i + 1) * m_tileSize + rect.left - offset - Tile::DoubleTileSize;
		m_vertices[(i * 4u) + 2u].position.x = (i + 1) * m_tileSize + rect.left - offset - Tile::DoubleTileSize;
		m_vertices[(i * 4u) + 3u].position.x = i * m_tileSize + rect.left - offset - Tile::DoubleTileSize;
	}
}

void GenerativeLayer::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), m_widthScreen * 4u, sf::Quads, states);
}
