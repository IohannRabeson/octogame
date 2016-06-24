#include "GenerativeLayer.hpp"
#include "SkyCycle.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>
#include <limits>

GenerativeLayer::GenerativeLayer(void) :
	GenerativeLayer(sf::Color::Yellow, sf::Vector2f(0.5f, 0.5f), sf::Vector2u(512u, 128u), 16.f, 20, 0.f, 1.f, 20.f)
{}

GenerativeLayer::GenerativeLayer(sf::Color const & color, sf::Vector2f const & speed, sf::Vector2u const & mapSize, float tileSize, int heightOffset, float topOpacity, float botOpacity, float transitionDuration, float deltaOffset) :
	ALayer(speed),
	m_camera(octo::Application::getCamera()),
	m_positions(0u),
	m_positionsPrev(0u),
	m_mapSize(mapSize),
	m_color(color),
	m_tileSize(tileSize),
	m_depth(0.f),
	m_transitionTimer(0.f),
	m_transitionTimerDuration(transitionDuration),
	m_topOpacity(topOpacity),
	m_botOpacity(botOpacity),
	m_highestY(0.f),
	m_heightOffset(heightOffset),
	m_deltaOffset(deltaOffset),
	m_widthScreen(octo::Application::getGraphicsManager().getVideoMode().width / m_tileSize + 4u),
	m_verticesCount(0u)
{
	// Initialize mapSurface pointer
	setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
	{
		return noise.perlin(x, y, 3, 2.f, 5.0f);
	});
	m_bottomLeft.setSize(sf::Vector2f(m_mapSize) * 100.f);
	m_bottomRight.setSize(sf::Vector2f(m_mapSize) * 100.f);
}

void GenerativeLayer::setup(void)
{
	m_verticesCount = m_widthScreen * 4u;
	m_vertices.reset(new sf::Vertex[m_verticesCount]);
	setColor(m_color);
	m_positions.resize(getMapSize().x * 4);
	m_positionsPrev.resize(getMapSize().x * 4);
	m_noise.setSeed(42);

	for (std::size_t i = 0u; i < getMapSize().x; i++)
	{
		m_positions[(i * 4u) + 2u].y = static_cast<float>(getMapSize().y) * m_tileSize + m_deltaOffset;
		m_positions[(i * 4u) + 3u].y = static_cast<float>(getMapSize().y) * m_tileSize + m_deltaOffset;
		m_positionsPrev[(i * 4u) + 2u].y = static_cast<float>(getMapSize().y) * m_tileSize + m_deltaOffset;
		m_positionsPrev[(i * 4u) + 3u].y = static_cast<float>(getMapSize().y) * m_tileSize + m_deltaOffset;
	}

	computeVertices(m_positions);
	swap();
}

void GenerativeLayer::computeVertices(std::vector<sf::Vector2f> & positions)
{
	m_highestY = std::numeric_limits<float>::max();
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
			transition = static_cast<int>(transition) - static_cast<int>(transition) % static_cast<int>(m_tileSize);
			positions[(i * 4u) + 0u] = std::move(sf::Vector2f(i * m_tileSize, transition));
			positions[(i * 4u) + 1u] = std::move(sf::Vector2f((i + 1u) * m_tileSize, transition));
		}
		else
		{
			positions[(i * 4u) + 0u] = std::move(sf::Vector2f(i * m_tileSize, getHeightValue(i * 4u)));
			positions[(i * 4u) + 1u] = std::move(sf::Vector2f((i + 1u) * m_tileSize, getHeightValue(i * 4u)));
		}
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
	int f = (m_backgroundSurface(static_cast<float>(x) / static_cast<float>(getMapSize().x), m_depth) + 1.f) / 2.f * static_cast<float>(getMapSize().y) - m_heightOffset;
	float ff = f * m_tileSize;
	if (ff < m_highestY)
		m_highestY = ff;
	return ff;
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

void GenerativeLayer::update(float deltatime, ABiome &)
{
	m_transitionTimer += deltatime;
	if (m_transitionTimerDuration > 0.f && m_transitionTimer > m_transitionTimerDuration)
	{
		m_transitionTimer = 0.f;
		swap();
	}
	float transition = m_transitionTimer / m_transitionTimerDuration;
	sf::FloatRect const & rect = m_camera.getRectangle();
	//TODO : Keep an eyes on that
	float offsetX = (rect.left * getSpeed().x) / m_tileSize + 1000000.f;
	int offsetBackground = static_cast<int>(offsetX);
	offsetX -= static_cast<float>(offsetBackground);
	offsetX *= m_tileSize;
	float offsetY = rect.top * getSpeed().y;
	for (std::size_t i = 0u; i < m_widthScreen; i++)
	{
		if (m_transitionTimerDuration < 0.f)
		{
			m_vertices[(i * 4u) + 0u].position.y = m_positions[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 0u].y;
			m_vertices[(i * 4u) + 1u].position.y = m_positions[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 1u].y;
		}
		else
		{
			m_vertices[(i * 4u) + 0u].position.y = octo::linearInterpolation(m_positionsPrev[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 0u].y, m_positions[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 0u].y, transition);
			m_vertices[(i * 4u) + 1u].position.y = octo::linearInterpolation(m_positionsPrev[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 1u].y, m_positions[((offsetBackground + i) * 4u) % (getMapSize().x * 4) + 1u].y, transition);
		}
		m_vertices[(i * 4u) + 2u].position.y = m_positions[(i * 4u) + 2u].y;
		m_vertices[(i * 4u) + 3u].position.y = m_positions[(i * 4u) + 3u].y;
		//TODO: little optimization possible if we precompute some values, maybe not worth
		m_vertices[(i * 4u) + 0u].position.x = i * m_tileSize + rect.left - offsetX - m_tileSize * 2;
		m_vertices[(i * 4u) + 1u].position.x = (i + 1) * m_tileSize + rect.left - offsetX - m_tileSize * 2;
		m_vertices[(i * 4u) + 2u].position.x = (i + 1) * m_tileSize + rect.left - offsetX - m_tileSize * 2;
		m_vertices[(i * 4u) + 3u].position.x = i * m_tileSize + rect.left - offsetX - m_tileSize * 2;
	}

	sf::Color opacityColor = getOwner().getSkyCycle().getSkyColorDown();

	float max = m_vertices[3u].position.y - m_highestY;
	float t;
	for (std::size_t i = 0u; i < m_widthScreen; i++)
	{
		t = octo::linearInterpolation(m_topOpacity, m_botOpacity, (m_vertices[(i * 4u) + 0u].position.y - m_highestY) / max);
		m_vertices[(i * 4u) + 0u].color = std::move(octo::linearInterpolation(m_color, opacityColor, t));

		t = octo::linearInterpolation(m_topOpacity, m_botOpacity, (m_vertices[(i * 4u) + 1u].position.y - m_highestY) / max);
		m_vertices[(i * 4u) + 1u].color = std::move(octo::linearInterpolation(m_color, opacityColor, t));

		m_vertices[(i * 4u) + 2u].color = std::move(octo::linearInterpolation(m_color, opacityColor, m_botOpacity));
		m_vertices[(i * 4u) + 3u].color = m_vertices[(i * 4u) + 2u].color;

		m_vertices[(i * 4u) + 0u].position.y += offsetY;
		m_vertices[(i * 4u) + 1u].position.y += offsetY;
		m_vertices[(i * 4u) + 2u].position.y += offsetY;
		m_vertices[(i * 4u) + 3u].position.y += offsetY;
	}

	m_bottomLeft.setPosition(sf::Vector2f(m_vertices[0u].position.x, m_vertices[((m_widthScreen - 1) * 4u) + 2u].position.y));
	m_bottomRight.setPosition(m_bottomLeft.getPosition() - sf::Vector2f(m_mapSize.x, 0.f));

	m_bottomLeft.setFillColor(m_vertices[0 * 4u + 0u].color);
	m_bottomRight.setFillColor(m_vertices[0 * 4u + 2u].color);
}

void GenerativeLayer::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), m_verticesCount, sf::Quads, states);
	render.draw(m_bottomLeft, states);
	render.draw(m_bottomRight, states);
}
