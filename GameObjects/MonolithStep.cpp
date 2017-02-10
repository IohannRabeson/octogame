#include "MonolithStep.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Math.hpp>
#include <Interpolations.hpp>

MonolithStep::MonolithStep(ResourceKey const & key, sf::Vector2f const & position, sf::Vector2f const & scale) :
	m_generator("random"),
	m_vertices(new sf::Vertex[100u]),
	m_size(500.f, 500.f),
	m_position(position),
	m_timerMax(sf::seconds(m_generator.randomFloat(1.2f, 1.86f))),
	m_state(None),
	m_used(0u),
	m_offset(m_generator.randomFloat(-0.f, 20.f)),
	m_startEndTransition(0.f)
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
	m_position += sf::Vector2f(x, y);
	m_endPosition += sf::Vector2f(x, y);
}

void MonolithStep::setPosition(sf::Vector2f const & position)
{
	m_sprite.setPosition(position);
	m_position = position;
	m_endPosition = position;
	m_sprite.play();
}

void MonolithStep::setEndPosition(sf::Vector2f const & position)
{
	sf::Vector2f dir = m_sprite.getPosition() + sf::Vector2f(m_sprite.getGlobalBounds().width / 2.f, m_sprite.getGlobalBounds().height / 2.f) - position;
	octo::normalize(dir);
	m_endPosition = m_position + dir * 170.f;
}

void MonolithStep::setStartEndTransition(float transition)
{
	m_startEndTransition = transition;
}

void MonolithStep::addPosition(sf::Vector2f const & position)
{
	m_position += position;
	m_endPosition += position;
}

void MonolithStep::activate(void)
{
	m_state = Activate;
	m_size = sf::Vector2f(m_generator.randomFloat(80.f, 100.f), m_generator.randomFloat(80.f, 100.f));
	m_sprite.play();
}

void MonolithStep::firstActivate(void)
{
	m_state = FirstActivate;
	m_sprite.play();
}

void MonolithStep::update(sf::Time frametime)
{
	m_sprite.update(frametime);

	switch (m_state)
	{
		case FirstActivate:
			{
				m_timer += frametime;
				m_builder.clear();
				sf::Vector2f pos = m_sprite.getPosition() + sf::Vector2f(m_sprite.getGlobalSize().x / 2.f, m_sprite.getGlobalSize().y / 2.f);
				createEffect(m_size, pos, std::pow(std::min(1.f, m_timer / m_timerMax), 0.867f), sf::Color(240, 25, 25, 200), m_builder);
				m_used = m_builder.getUsed();
				if (m_timer > m_timerMax)
				{
					m_timer = sf::Time::Zero;
					activate();
				}
				break;
			}
		case Activate:
			{
				m_timer += frametime;
				if (m_timer > m_timerMax)
					m_timer = sf::Time::Zero;
				m_builder.clear();
				sf::Vector2f pos = m_sprite.getPosition() + sf::Vector2f(m_sprite.getGlobalSize().x / 2.f, m_sprite.getGlobalSize().y / 2.f);
				createEffect(m_size, pos, std::pow(std::min(1.f, m_timer / m_timerMax), 0.867f), sf::Color(240, 25, 25, 200), m_builder);
				m_used = m_builder.getUsed();
				break;
			}
		default:
			break;
	}
	m_offset += frametime.asSeconds() * 2.f;
	sf::Vector2f pos = octo::linearInterpolation(m_position, m_endPosition, m_startEndTransition) + sf::Vector2f(0.f, std::cos(m_offset) * 5.f);
	m_sprite.setPosition(pos);
}

void MonolithStep::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
	m_sprite.draw(render, states);
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
