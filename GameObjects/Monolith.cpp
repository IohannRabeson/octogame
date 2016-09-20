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
	m_box(PhysicsEngine::getShapeBuilder().createCircle(false))
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.1f), 0u);
	m_animationMonolith.setFrames(frames);
	m_animationMonolith.setLoop(octo::LoopMode::Loop);

	m_spriteMonolith.setSpriteSheet(resources.getSpriteSheet(MONOLITH_OSS));
	m_spriteMonolith.setScale(scale);
	m_spriteMonolith.setPosition(position.x + 150.f, position.y + 100.f);
	m_spriteMonolith.setAnimation(m_animationMonolith);
	m_spriteMonolith.play();

	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_1_OSS, position + sf::Vector2f(0.f, 0.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_2_OSS, position + sf::Vector2f(50.f, 20.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_3_OSS, position + sf::Vector2f(100.f, 50.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_4_OSS, position + sf::Vector2f(200.f, 50.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_5_OSS, position + sf::Vector2f(200.f, 150.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_6_OSS, position + sf::Vector2f(300.f, -50.f), scale));
	m_steps.emplace_back(new MonolithStep(MONOLITH_STEP_7_OSS, position + sf::Vector2f(400.f, 50.f), scale));

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

void Monolith::collideOcto(CharacterOcto *)
{
	if (Progress::getInstance().getActivatedMonolith() < Progress::getInstance().getRandomDiscoverCount())
	{
		//octo->enableCutscene(true, true);
		std::cout << "do cutscene" << std::endl;
		for (std::size_t i = Progress::getInstance().getActivatedMonolith(); i < Progress::getInstance().getRandomDiscoverCount(); i++)
			m_steps[i]->activate();
		Progress::getInstance().setActivatedMonolith(Progress::getInstance().getRandomDiscoverCount());
	}
}

void Monolith::update(sf::Time frameTime)
{
	InstanceDecor::update(frameTime);
	m_spriteMonolith.update(frameTime);
	for (auto & step : m_steps)
		step->update(frameTime);
}

void Monolith::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	InstanceDecor::draw(render, states);
	m_spriteMonolith.draw(render, states);
	for (auto & step : m_steps)
		step->draw(render, states);
}

void Monolith::drawFront(sf::RenderTarget& render, sf::RenderStates states) const
{
	InstanceDecor::drawFront(render, states);
}
