#include "NanoEffect.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <AudioManager.hpp>
#include <ResourceManager.hpp>

NanoEffect::NanoEffect(void) :
	m_vertices(new sf::Vertex[100]),
	m_count(100),
	m_used(0u),
	m_size(150.f, 150.f),
	m_color(255, 255, 255, 200),
	m_isActive(true),
	m_isTransfer(false),
	m_isRandom(false),
	m_glowingTimerMax(sf::seconds(2.f))
{
	m_generator.setSeed("random");
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
}

void NanoEffect::playSound(void)
{
	std::size_t soundChoose = m_generator.randomInt(0u, 3u);
	octo::AudioManager& audio = octo::Application::getAudioManager();
	octo::ResourceManager& resource = octo::Application::getResourceManager();

	switch (soundChoose)
	{
		case 0u:
			audio.playSound(resource.getSound(NANO_1_WAV), 0.5f, 1.f);
			break;
		case 1u:
			audio.playSound(resource.getSound(NANO_2_WAV), 0.5f, 1.f);
			break;
		case 2u:
			audio.playSound(resource.getSound(NANO_3_WAV), 0.5f, 1.f);
			break;
		default:
			break;
	}
}

void NanoEffect::createLosange(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f up(0.f, -size.y);
	sf::Vector2f down(0.f, size.y);
	sf::Vector2f left(-size.x, 0.f);
	sf::Vector2f right(size.x, 0.f);

	up += origin;
	down += origin;
	left += origin;
	right += origin;

	builder.createTriangle(left, up, right, color);
	builder.createTriangle(left, down, right, color);
}

void NanoEffect::createEffect(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color color, octo::VertexBuilder& builder)
{
	float glowingCoef = m_glowingTimer / m_glowingTimerMax;

	color.a = color.a * (1 - glowingCoef);
	createLosange(size * glowingCoef, origin, color, builder);
}

void NanoEffect::update(sf::Time frameTime)
{
	m_builder.clear();
	m_glowingTimer += frameTime;
	m_randomGlowing += frameTime;
	if (m_glowingTimer >= m_glowingTimerMax)
	{
		if (m_isTransfer == true)
			m_isActive = false;
		m_glowingTimer = sf::Time::Zero;
	}
	if (m_isActive == true)
	{
		createEffect(m_size, m_position, m_color, m_builder);
		if (m_isTransfer == true && m_isRandom == false)
			createEffect(m_size * 2.f, m_position, m_color, m_builder);
	}
	if (m_randomGlowing >= m_randomGlowingMax && m_isTransfer == true)
	{
		playSound();
		m_isActive = true;
		m_glowingTimer = sf::Time::Zero;
		m_randomGlowing = sf::Time::Zero;
		m_randomGlowingMax = sf::seconds(m_generator.randomFloat(30.f, 90.f));
	}
	m_used = m_builder.getUsed();
}

void NanoEffect::onTransfer(void)
{
	if (m_isActive == true && m_isTransfer == false)
	{
		playSound();
		m_isTransfer = true;
		m_isRandom = true;
		m_glowingTimer = sf::Time::Zero;
	}
}

void NanoEffect::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
}

sf::Vector2f const & NanoEffect::getPosition(void) const
{
	return m_position;
}

sf::Color const & NanoEffect::getColor(void) const
{
	return m_color;
}

bool NanoEffect::isActive(void) const
{
	return m_isActive;
}

void NanoEffect::setPosition(sf::Vector2f const & position)
{
	m_position = position;
}

void NanoEffect::setColor(sf::Color const & color)
{
	m_color = color;
}

void NanoEffect::setActive(bool isActive)
{
	m_isActive = isActive;
}
