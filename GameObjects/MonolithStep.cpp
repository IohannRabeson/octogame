#include "MonolithStep.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

MonolithStep::MonolithStep(ResourceKey const & key, sf::Vector2f const & position, sf::Vector2f const & scale) :
	m_vertices(new sf::Vertex[100u]),
	m_size(100.f, 100.f),
	m_timerMax(sf::seconds(1.65f)),
	m_used(0u),
	m_isActivated(false)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.2f), 0u);
	frames.emplace_back(sf::seconds(0.2f), 1u);
	frames.emplace_back(sf::seconds(0.2f), 2u);
	frames.emplace_back(sf::seconds(0.2f), 3u);
	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::NoLoop);

	m_sprite.setSpriteSheet(resources.getSpriteSheet(key));
	m_sprite.setScale(scale);
	m_sprite.setPosition(position.x, position.y);
	m_sprite.setAnimation(m_animation);
	m_sprite.stop();

	m_builder = octo::VertexBuilder(m_vertices.get(), 100u);
}

void MonolithStep::addMapOffset(float x, float y)
{
	m_sprite.setPosition(m_sprite.getPosition().x + x, m_sprite.getPosition().y + y);
}

void MonolithStep::setPosition(sf::Vector2f const & position)
{
	m_sprite.setPosition(position);
}

void MonolithStep::activate(void)
{
	m_isActivated = true;
	m_sprite.play();
}

void MonolithStep::update(sf::Time frametime)
{
	m_sprite.update(frametime);

	if (m_isActivated)
	{
		m_timer += frametime;
		if (m_timer > m_timerMax)
			m_timer = sf::Time::Zero;
		m_builder.clear();
		sf::Vector2f pos = m_sprite.getPosition() + sf::Vector2f(m_sprite.getGlobalSize().x / 2.f, m_sprite.getGlobalSize().y / 2.f);
		createEffect(m_size, pos, std::pow(m_timer / m_timerMax, 0.867f), sf::Color(240, 125, 125, 200), m_builder);
		m_used = m_builder.getUsed();
	}
}

void MonolithStep::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	m_sprite.draw(render, states);
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
}

void MonolithStep::createLosange(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
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

void MonolithStep::createEffect(sf::Vector2f const & size, sf::Vector2f const & origin, float glowingCoef, sf::Color color, octo::VertexBuilder& builder)
{
	color.a = color.a * (1 - glowingCoef);
	createLosange(size * glowingCoef, origin, color, builder);
}
