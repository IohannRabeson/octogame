#include "Water.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>
#include <Camera.hpp>

Water::Water(ABiome & biome) :
	m_waveCycle(sf::Time::Zero),
	m_width(biome.getMapSize().x * Tile::TileSize),
	m_limit(1000.f),
	m_shaderIndex(0u)
{
	m_rectLeft.setSize(sf::Vector2f(m_width, 1000.f));
	m_rectLeft.setFillColor(sf::Color(0, 0, 200, 50));
	m_rectLeft.setOutlineColor(sf::Color::Red);

	m_rectRight.setSize(sf::Vector2f(m_width, 1000.f));
	m_rectRight.setFillColor(sf::Color(0, 0, 200, 50));
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

	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();
	octo::PostEffect postEffectShader;
	postEffectShader.resetShader(&m_shader);
	m_shaderIndex = postEffect.addEffect(std::move(postEffectShader));
	postEffect.enableEffect(m_shaderIndex, true);
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

void Water::update(sf::Time frameTime)
{
	sf::FloatRect const & rect = octo::Application::getCamera().getRectangle();
	m_waveCycle += frameTime;
	m_shader.setParameter("time", m_waveCycle.asSeconds());
	m_shader.setParameter("distortionFactor", 0.15f);
	m_shader.setParameter("riseFactor", 0.1f);
	float limit = rect.height - (1000.f - rect.top);
	m_shader.setParameter("limit", limit);
	m_shader.setParameter("offset_limit", limit / rect.height);
	m_shader.setParameter("height", 1.f - (limit / rect.height));
}

void Water::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	//states.shader = &m_shader;
	render.draw(m_rectLeft, states);
	render.draw(m_rectRight, states);
}