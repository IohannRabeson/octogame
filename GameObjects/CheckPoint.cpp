#include "CheckPoint.hpp"
#include "CharacterOcto.hpp"
#include "Progress.hpp"
#include "RectangleShape.hpp"
#include "PhysicsEngine.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <AudioManager.hpp>

CheckPoint::CheckPoint(sf::Vector2f const & scale, sf::Vector2f const & position) :
	InstanceDecor(CHECKPOINT_OSS),
	m_box(PhysicsEngine::getShapeBuilder().createRectangle()),
	m_count(100),
	m_used(0u),
	m_timer(sf::Time::Zero),
	m_timerMax(sf::seconds(1.65f)),
	m_vertices(new sf::Vertex[m_count]),
	m_size(150.f, 150.f),
	m_color(255, 255, 255, 200),
	m_firstFrame(true),
	m_isValidated(false)
{
	m_box->setSize(sf::Vector2f(30.f, 285.f));
	m_sprite.setOrigin(sf::Vector2f(0.f, -10.f));
	m_sprite.setScale(scale);
	m_sprite.setPosition(position);
	m_box->setPosition(getPosition() + sf::Vector2f(30.f, 100.f));
	m_box->update();

	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
	m_box->setGameObject(this);
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::CheckPoint));
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::Player));

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.4f), 0u);
	frames.emplace_back(sf::seconds(0.4f), 1u);
	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::Loop);

	frames.clear();
	frames.emplace_back(sf::seconds(0.20f), 2u);
	frames.emplace_back(sf::seconds(0.15f), 3u);
	frames.emplace_back(sf::seconds(0.10f), 4u);
	frames.emplace_back(sf::seconds(0.15f), 5u);
	frames.emplace_back(sf::seconds(0.20f), 6u);
	frames.emplace_back(sf::seconds(0.25f), 7u);
	frames.emplace_back(sf::seconds(0.30f), 8u);
	frames.emplace_back(sf::seconds(0.30f), 9u);
	m_animationValidated.setFrames(frames);
	m_animationValidated.setLoop(octo::LoopMode::Loop);

	m_sprite.setAnimation(m_animation);
	m_sprite.play();
}

void CheckPoint::createLosange(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
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

void CheckPoint::createEffect(sf::Vector2f const & size, sf::Vector2f const & origin, float glowingCoef, sf::Color color, octo::VertexBuilder& builder)
{
	color.a = color.a * (1 - glowingCoef);
	createLosange(size * glowingCoef, origin, color, builder);
}

void CheckPoint::update(sf::Time frametime)
{
	InstanceDecor::update(frametime);
	if (m_firstFrame)
	{
		m_firstFrame = false;
		m_startPosition = getPosition();
	}
	if (m_isValidated)
	{
		m_timer += frametime;
		if (m_timer > m_timerMax)
			m_timer = sf::Time::Zero;
		m_builder.clear();
		createEffect(m_size, getPosition() + sf::Vector2f(47.f, 20.f), std::pow(m_timer / m_timerMax, 0.867f), m_color, m_builder);
		createEffect(m_size, getPosition() + sf::Vector2f(47.f, 20.f), std::pow(m_timer / m_timerMax, 0.12f), m_color, m_builder);
		m_used = m_builder.getUsed();
	}
}

void CheckPoint::addMapOffset(float x, float y)
{
	InstanceDecor::addMapOffset(x, y);
	m_box->setPosition(m_box->getPosition() + sf::Vector2f(x, y));
	m_box->update();
}

void CheckPoint::setPosition(sf::Vector2f const & position)
{
	InstanceDecor::setPosition(position);
	m_box->setPosition(position - sf::Vector2f(70.f, 288.f));
	m_box->update();
}

void CheckPoint::collideOctoEvent(CharacterOcto *)
{
	if (!m_isValidated)
	{
		octo::AudioManager& audio = octo::Application::getAudioManager();
		octo::ResourceManager& resources = octo::Application::getResourceManager();
		audio.playSound(resources.getSound(OCTO_ANGRY_OGG), 0.7f);
		m_sprite.setAnimation(m_animationValidated);
		Progress::getInstance().setCheckPointPosition(m_startPosition);
	}
	m_isValidated = true;
}

void CheckPoint::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
	InstanceDecor::draw(render, states);
}
