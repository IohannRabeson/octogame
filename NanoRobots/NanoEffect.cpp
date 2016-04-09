#include "NanoEffect.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <AudioManager.hpp>
#include <ResourceManager.hpp>

NanoEffect::NanoEffect(void) :
	m_vertices(new sf::Vertex[100]),
	m_count(100),
	m_used(0u),
	m_size(150.f, 150.f),
	m_color(255, 255, 255, 200),
	m_state(State::Active),
	m_isTransferHappen(false),
	m_glowingTimerMax(sf::seconds(2.f)),
	m_transferTimerMax(sf::seconds(2.f)),
	m_soundPlayed(false),
	m_lastNanoCount(0u)
{
	m_generator.setSeed("random");
	m_randomTimerMax = sf::seconds(m_generator.randomFloat(30.f, 90.f));
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
	m_lastNanoCount = Progress::getInstance().getNanoRobotCount();
}

void NanoEffect::playSound(void)
{
	if (m_soundPlayed == false)
	{
		std::size_t soundChoose = m_generator.randomInt(0u, 3u);
		octo::AudioManager& audio = octo::Application::getAudioManager();
		octo::ResourceManager& resource = octo::Application::getResourceManager();
	
		if (m_lastNanoCount != Progress::getInstance().getNanoRobotCount())
			audio.playSound(resource.getSound(OCTO_QUESTION_OGG), 0.7f, 1.f);
		switch (soundChoose)
		{
			case 0u:
				audio.playSound(resource.getSound(NANO_1_OGG), 0.4f, 1.f);
				break;
			case 1u:
				audio.playSound(resource.getSound(NANO_2_OGG), 0.4f, 1.f);
				break;
			case 2u:
				audio.playSound(resource.getSound(NANO_3_OGG), 0.4f, 1.f);
				break;
			default:
				break;
		}
		m_soundPlayed = true;
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

void NanoEffect::createEffect(sf::Vector2f const & size, sf::Vector2f const & origin, float glowingCoef, sf::Color color, octo::VertexBuilder& builder)
{
	color.a = color.a * (1 - glowingCoef);
	createLosange(size * glowingCoef, origin, color, builder);
}

void NanoEffect::update(sf::Time frameTime)
{
	m_builder.clear();

	switch (m_state)
	{
		case State::Active:
		{
			m_glowingTimer += frameTime;
			if (m_glowingTimer >= m_glowingTimerMax)
				m_glowingTimer = sf::Time::Zero;
			createEffect(m_size, m_position, m_glowingTimer / m_glowingTimerMax, m_color, m_builder);
			break;
		}
		case State::Transfer:
		{
			playSound();
			m_transferTimer += frameTime;
			if (m_transferTimer >= m_transferTimerMax)
			{
				m_isTransferHappen = true;
				m_transferTimer = sf::Time::Zero;
				m_state = State::Wait;
			}
			float coef = m_transferTimer / m_transferTimerMax;
			createEffect(m_size, m_position, coef, m_color, m_builder);
			createEffect(m_size * 2.f, m_position, coef, m_color, m_builder);
			createEffect(m_size * 3.f, m_position, coef, m_color, m_builder);

			if (m_transferTimer <= m_transferTimerMax / 2.f)
				m_nanoScale = octo::cosinusInterpolation(m_nanoScaleOrigin, m_nanoScaleZoom, m_transferTimer / (m_transferTimerMax / 2.f));
			else if (m_transferTimer <= m_transferTimerMax)
				m_nanoScale = octo::cosinusInterpolation(m_nanoScaleZoom, m_nanoScaleOrigin, (m_transferTimer - m_transferTimerMax / 2.f) / (m_transferTimerMax / 2.f));
			break;
		}
		case State::Random:
		{
			playSound();
			m_glowingTimer += frameTime;
			if (m_glowingTimer >= m_glowingTimerMax)
			{
				m_glowingTimer = sf::Time::Zero;
				m_state = State::Wait;
			}
			createEffect(m_size, m_position, m_glowingTimer / m_glowingTimerMax, m_color, m_builder);
			break;
		}
		case State::Wait:
		{
			m_randomTimer += frameTime;
			if (m_randomTimer >= m_randomTimerMax)
			{
				m_glowingTimer = sf::Time::Zero;
				m_randomTimer = sf::Time::Zero;
				m_randomTimerMax = sf::seconds(m_generator.randomFloat(30.f, 90.f));
				m_state = State::Random;
			}
			break;
		}
		default:
			break;
	}

	Progress const & progress = Progress::getInstance();
	m_lastNanoCount = progress.getNanoRobotCount();
	m_used = m_builder.getUsed();
}

void NanoEffect::onTransfer(void)
{
	if (m_isTransferHappen == false)
		m_state = State::Transfer;
}

void NanoEffect::setState(State state)
{
	m_state = state;
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

sf::Vector2f const & NanoEffect::getNanoScale(void) const
{
	return m_nanoScale;
}

void NanoEffect::setPosition(sf::Vector2f const & position)
{
	m_position = position;
}

void NanoEffect::setColor(sf::Color const & color)
{
	m_color = color;
}

void NanoEffect::setNanoScale(sf::Vector2f const & scale)
{
	m_nanoScaleOrigin = scale;
	m_nanoScaleZoom = scale * 2.f;
	m_nanoScale = scale;
}
