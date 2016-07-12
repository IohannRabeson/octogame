#include "Water.hpp"
#include "ABiome.hpp"
#include "PostEffectLayer.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>

Water::Water(ABiome & biome) :
	m_waterColor(biome.getWaterColor()),
	m_shader(PostEffectLayer::getInstance().getShader(WATER_FRAG)),
	m_waveCycle(sf::Time::Zero),
	m_width(biome.getMapSize().x * Tile::TileSize),
	m_limit(1000.f),
	m_timeChangeColorMax(sf::seconds(2.f))
{
	m_rectLeft.setSize(sf::Vector2f(m_width, 10000.f));
	m_rectLeft.setFillColor(m_waterColor);
	m_rectLeft.setOutlineColor(sf::Color::Red);

	m_rectRight.setSize(sf::Vector2f(m_width, 10000.f));
	m_rectRight.setFillColor(m_waterColor);
	m_rectRight.setOutlineColor(sf::Color::Red);

	m_limit = biome.getWaterLevel();
	setPosition(sf::Vector2f(0.f, m_limit));

	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	m_distorsionTexture = resources.getTexture(DISTORSION_PNG);
	m_shader.loadFromMemory(resources.getText(WATER_FRAG), sf::Shader::Fragment);

	// It is important to set repeated to true to enable scrolling upwards
	m_distorsionTexture.setRepeated(true);
	// Setting smooth to true lets us use small maps even on larger images
	m_distorsionTexture.setSmooth(true);

	m_shader.setParameter("distortionMapTexture", m_distorsionTexture);
	m_shader.setParameter("max_factor", 0.15f);
	m_shader.setParameter("activate_persistence", biome.getWaterPersistence());
	m_shader.setParameter("offset", 1.f / 300.f);
	sf::Transform kernel(
		1.f / 16.f, 2.f / 16.f, 1.f / 16.f,
		2.f / 16.f, 4.f / 16.f, 2.f / 16.f,
		1.f / 16.f, 2.f / 16.f, 1.f / 16.f
		);
	m_shader.setParameter("kernel", kernel);

	PostEffectLayer::getInstance().enableShader(WATER_FRAG, true);
}

Water::~Water(void)
{}

sf::Vector2f const & Water::getPosition(void) const
{
	return m_rectLeft.getPosition();
}

void Water::setPosition(sf::Vector2f const & position)
{
	m_rectLeft.setPosition(position);
	m_rectRight.setPosition(sf::Vector2f(position.x - m_width, position.y));
}

void Water::addMapOffset(float width)
{
	setPosition(sf::Vector2f(m_rectLeft.getPosition() + sf::Vector2f(width, 0.f)));
}

void Water::changeColor(sf::Time frameTime)
{
	sf::Color color = sf::Color::Blue;
	color.a = 100.f;
	Progress & progress = Progress::getInstance();
	if (progress.canUseWaterJump())
	{
		if (m_timeChangeColor < m_timeChangeColorMax)
			m_timeChangeColor += frameTime;
		else
			m_timeChangeColor = m_timeChangeColorMax;
		m_rectLeft.setFillColor(octo::cosinusInterpolation(m_waterColor, color, m_timeChangeColor / m_timeChangeColorMax));
		m_rectRight.setFillColor(octo::cosinusInterpolation(m_waterColor, color, m_timeChangeColor / m_timeChangeColorMax));
	}
}

void Water::update(sf::Time frameTime)
{
	sf::FloatRect const & rect = octo::Application::getCamera().getRectangle();
	m_waveCycle += frameTime;
	m_shader.setParameter("time", m_waveCycle.asSeconds());
	m_shader.setParameter("distortionFactor", 0.15f);
	m_shader.setParameter("riseFactor", 0.1f);
	float limit = rect.height - (m_limit - rect.top);
	m_shader.setParameter("limit", limit);
	float ratio = limit / rect.height;
	if (ratio > 1.f)
		ratio = 1.f;
	m_shader.setParameter("offset_limit", ratio);
	m_shader.setParameter("height", 1.f - ratio);
	changeColor(frameTime);
}

void Water::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	//states.shader = &m_shader;
	render.draw(m_rectLeft, states);
	render.draw(m_rectRight, states);
}
