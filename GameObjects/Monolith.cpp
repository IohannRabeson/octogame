#include "Monolith.hpp"
#include "PhysicsEngine.hpp"
#include "CircleShape.hpp"
#include "MonolithStep.hpp"
#include "Progress.hpp"
#include "CharacterOcto.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

Monolith::Monolith(sf::Vector2f const & scale, sf::Vector2f const & position) :
	InstanceDecor(MONOLITH_PEDESTAL_OSS, scale, position, 1u),
	m_vertices(new sf::Vertex[100u]),
	m_size(500.f, 500.f),
	m_box(PhysicsEngine::getShapeBuilder().createCircle(false)),
	m_position(position),
	m_timerMax(sf::seconds(2.f)),
	m_used(0u),
	m_state(None),
	m_offset(0.f)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.1f), 0u);
	m_animationMonolith.setFrames(frames);
	m_animationMonolith.setLoop(octo::LoopMode::Loop);

	m_position += sf::Vector2f(156.f, 112.f);
	m_spriteMonolith.setSpriteSheet(resources.getSpriteSheet(MONOLITH_OSS));
	m_spriteMonolith.setScale(scale);
	m_spriteMonolith.setPosition(m_position);
	m_spriteMonolith.setAnimation(m_animationMonolith);
	m_spriteMonolith.play();

	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_1_OSS, position + sf::Vector2f(6.f, 262.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_2_OSS, position + sf::Vector2f(56.f, 194.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_3_OSS, position + sf::Vector2f(50.f, 106.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_4_OSS, position + sf::Vector2f(112.f, 44.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_5_OSS, position + sf::Vector2f(225.f, 12.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_6_OSS, position + sf::Vector2f(268.f, 31.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_7_OSS, position + sf::Vector2f(369.f, 62.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_8_OSS, position + sf::Vector2f(331.f, 106.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_9_OSS, position + sf::Vector2f(356.f, 162.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_10_OSS, position + sf::Vector2f(419.f, 206.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_11_OSS, position + sf::Vector2f(363.f, 256.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_12_OSS, position + sf::Vector2f(412.f, 275.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_13_OSS, position + sf::Vector2f(387.f, 375.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_14_OSS, position + sf::Vector2f(287.f, 425.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_15_OSS, position + sf::Vector2f(237.f, 462.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_16_OSS, position + sf::Vector2f(194.f, 431.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_17_OSS, position + sf::Vector2f(119.f, 437.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_18_OSS, position + sf::Vector2f(31.f, 369.f), scale));

	for (std::size_t i = 0; i < Progress::getInstance().getActivatedMonolith(); i++)
		m_steps[i]->activate();

	m_box->setGameObject(this);
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::Monolith));
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::Player));
	m_box->setRadius(500.f);
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setPosition(m_spriteMonolith.getPosition() + m_spriteMonolith.getGlobalSize() / 2.f - sf::Vector2f(m_box->getRadius(), m_box->getRadius()));
	m_box->update();

	m_builder = octo::VertexBuilder(m_vertices.get(), 100u);
}

void Monolith::addMapOffset(float x, float y)
{
	InstanceDecor::addMapOffset(x, y);
	m_spriteMonolith.setPosition(m_spriteMonolith.getPosition().x + x, m_spriteMonolith.getPosition().y + y);
	for (auto & step : m_steps)
		step->addMapOffset(x, y);
	m_box->setPosition(m_box->getPosition() + sf::Vector2f(x, y));
	m_box->update();
}

void Monolith::setPosition(sf::Vector2f const & position)
{
	Monolith::setPosition(position);
	m_spriteMonolith.setPosition(position);
	for (auto & step : m_steps)
		step->setPosition(position);
	m_box->setPosition(m_spriteMonolith.getPosition() + m_spriteMonolith.getGlobalSize() / 2.f - sf::Vector2f(m_box->getRadius(), m_box->getRadius()));
	m_box->update();
}

void Monolith::collideOcto(CharacterOcto * octo)
{
	if (m_state == None)
	{
		if (Progress::getInstance().getActivatedMonolith() < Progress::getInstance().countRandomDiscover())
		{
			octo->enableCutscene(true, true);
			m_state = StartEffect;
		}
		else if (Progress::getInstance().getActivatedMonolith() == 18)
		{
			m_state = StartFinalScene;
			m_size = sf::Vector2f(1000.f, 1000.f);
			std::cout << "end" << std::endl;
		}
	}
}

void Monolith::update(sf::Time frameTime)
{
	InstanceDecor::update(frameTime);

	switch (m_state)
	{
		case StartEffect:
		{
			m_timer += frameTime;
			if (m_timer >= m_timerMax)
			{
				m_timer = sf::Time::Zero;
				m_state = Activate;
			}
			break;
		}
		case Activate:
		{
			Progress & progress = Progress::getInstance();

			for (std::size_t i = progress.getActivatedMonolith(); i < progress.countRandomDiscover(); i++)
				m_steps[i]->firstActivate();
			progress.setActivatedMonolith(progress.countRandomDiscover());
			m_state = None;
			break;
		}
		case StartFinalScene:
		{
			m_timer += frameTime;
			if (m_timer > m_timerMax)
				m_timer = sf::Time::Zero;
			m_builder.clear();
			sf::Vector2f pos = m_spriteMonolith.getPosition() + sf::Vector2f(m_spriteMonolith.getGlobalSize().x / 2.f, m_spriteMonolith.getGlobalSize().y / 2.f);
			createEffect(m_size, pos, std::pow(m_timer / m_timerMax, 0.567f), sf::Color(240, 25, 25, 200), m_builder);
			m_used = m_builder.getUsed();
			break;
		}
		default:
			break;
	}
	m_spriteMonolith.update(frameTime);
	for (auto & step : m_steps)
		step->update(frameTime);
	m_offset += frameTime.asSeconds();
	m_spriteMonolith.setPosition(m_position + sf::Vector2f(0.f, std::cos(m_offset) * 5.f));
}

void Monolith::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	InstanceDecor::draw(render, states);
	m_spriteMonolith.draw(render, states);
	for (auto & step : m_steps)
		step->draw(render, states);
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
}

void Monolith::drawFront(sf::RenderTarget& render, sf::RenderStates states) const
{
	InstanceDecor::drawFront(render, states);
}

void Monolith::createLosange(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
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

void Monolith::createEffect(sf::Vector2f const & size, sf::Vector2f const & origin, float glowingCoef, sf::Color color, octo::VertexBuilder& builder)
{
	color.a = color.a * (1 - glowingCoef);
	createLosange(size * glowingCoef, origin, color, builder);
}
