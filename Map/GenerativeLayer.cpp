#include "GenerativeLayer.hpp"
# include "Tile.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>

GenerativeLayer::GenerativeLayer(void) :
	GenerativeLayer(sf::Color::Yellow, 0.5f)
{}

GenerativeLayer::GenerativeLayer(sf::Color const & color, float speed) :
	ALayer(speed),
	m_camera(octo::Application::getCamera()),
	m_mapSize(512u, 128u),
	m_widthScreen(octo::Application::getGraphicsManager().getVideoMode().width / Tile::TileSize + 4u),
	m_positions(m_mapSize.x * 4),
	m_tileSize(Tile::TileSize),
	m_color(color)
{
	init();
}

void GenerativeLayer::init(void)
{
	m_vertices.reset(new sf::Vertex[m_mapSize.x * 4]);
	m_noise.setSeed(42);

	for (std::size_t i = 0u; i < m_mapSize.x; i++)
	{
		m_positions[(i * 4u) + 0u] = sf::Vector2f(i * m_tileSize, getHeightValue(i * 4u));
		m_positions[(i * 4u) + 1u] = sf::Vector2f((i + 1u) * m_tileSize, getHeightValue(i * 4u));
		m_positions[(i * 4u) + 2u] = sf::Vector2f((i + 1u) * m_tileSize, static_cast<float>(m_mapSize.y) * 12.f);
		m_positions[(i * 4u) + 3u] = sf::Vector2f(i * m_tileSize, static_cast<float>(m_mapSize.y) * 12.f);

		for (std::size_t j = 0u; j < 4u; j++)
		{
			m_vertices[(i * 4u) + j].position = m_positions[(i * 4u) + j];
			m_vertices[(i * 4u) + j].color = m_color;
		}
	}
	for (std::size_t i = 1u; i < m_mapSize.x - 1u; i++)
	{
		if (m_positions[((i - 1u) * 4u + 1u)].y > m_positions[(i * 4u) + 0u].y)
			m_positions[(i * 4u) + 0u].y += m_tileSize;
		if (m_positions[((i + 1u) * 4u) + 0u].y > m_positions[(i * 4u) + 1u].y)
			m_positions[(i * 4u) + 1u].y += m_tileSize;
	}
}

//TODO: background generator
//TODO: rename
float GenerativeLayer::getHeightValue(int x)
{
	int f;
	if (getSpeed() < 0.5f)
		f = (m_noise.perlinNoise(static_cast<float>(x) / static_cast<float>(m_mapSize.x), 0.008f, 2, 5.f) + 1.f) / 2.f * static_cast<float>(m_mapSize.y);
	else
		f = (m_noise.perlinNoise(static_cast<float>(x) / static_cast<float>(m_mapSize.x), 0.108f, 2, 5.f) + 1.f) / 2.f * static_cast<float>(m_mapSize.y);
	return (f * static_cast<int>(m_tileSize));
}

void GenerativeLayer::setColor(sf::Color const & color)
{
	for (std::size_t i = 0u; i < m_widthScreen * 4u; i++)
		m_vertices[i].color = color;
}

void GenerativeLayer::update(float)
{
	sf::FloatRect const & rect = m_camera.getRectangle();
	float offset = (rect.left * getSpeed()) / m_tileSize;
	int offsetBackground = static_cast<int>(offset);
	offset -= static_cast<float>(offsetBackground);
	offset *= m_tileSize;
	for (std::size_t i = 0u; i < m_widthScreen; i++)
	{
		m_vertices[(i * 4u) + 0u].position.y = m_positions[((offsetBackground + i) * 4u) % (m_mapSize.x * 4) + 0u].y;
		m_vertices[(i * 4u) + 1u].position.y = m_positions[((offsetBackground + i) * 4u) % (m_mapSize.x * 4) + 1u].y;
		m_vertices[(i * 4u) + 2u].position.y = m_positions[((offsetBackground + i) * 4u) % (m_mapSize.x * 4) + 2u].y;
		m_vertices[(i * 4u) + 3u].position.y = m_positions[((offsetBackground + i) * 4u) % (m_mapSize.x * 4) + 3u].y;
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
