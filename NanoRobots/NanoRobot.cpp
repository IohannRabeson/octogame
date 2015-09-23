#include "NanoRobot.hpp"
#include "ResourceDefinitions.hpp"
#include "Tile.hpp"
#include "PhysicsEngine.hpp"
#include "CircleShape.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <sstream>

NanoRobot::NanoRobot(sf::Vector2f const & position, std::string id, std::size_t nbFrames, int seed) :
	m_swarm(1),
	m_uniformPopulation(1234u, &octo::Application::getResourceManager().getPalette(FROM_SEA1_OPA),
						1.2f, 2.f, 6.f, 10.f, 32.f, 50.f,
						sf::Time::Zero, sf::Time::Zero),
	m_spawnMode(FireflySwarm::SpawnMode::Normal),
	m_positionBehavior(new FireflySwarm::CirclePositionBehavior(seed, 50.f)),
	m_box(PhysicsEngine::getShapeBuilder().createCircle(false)),
	m_state(Idle),
	m_timer(sf::Time::Zero),
	m_timerMax(sf::seconds(7.f)),
	m_isTravelling(false)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_box->setRadius(100.f);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::Player));
	m_box->setApplyGravity(false);

	m_swarm.setPositionBehavior(m_positionBehavior);
	m_swarm.setTexture(resources.getTexture(FIREFLY01_PNG));
	m_swarm.create(m_spawnMode, position, sf::Color::Magenta, 8.f, 32.f, 2.f);

	m_sprite.setSpriteSheet(resources.getSpriteSheet(id));
	m_sprite.setScale(0.6f, 0.6f);

	octo::SpriteAnimation::FrameList	frames;
	for (std::size_t i = 0u; i < nbFrames; i++)
		frames.emplace_back(sf::seconds(0.2f), i);

	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::Loop);
	m_sprite.setAnimation(m_animation);
	m_sprite.play();

	std::map<std::string, std::vector<std::string>>	npcTexts;
	std::istringstream f(resources.getText(DIALOGS_TXT).toAnsiString());
	std::string key;
	std::string line;
	while (std::getline(f, key, '='))
	{
		std::getline(f, line);
		npcTexts[key].push_back(line);
	}
	m_text.reset(new BubbleText());
	m_text->setup(npcTexts[id][0u], sf::Color::White);
	m_text->setType(ABubble::Type::Speak);
	m_text->setActive(false);
}

NanoRobot::~NanoRobot(void)
{}

void NanoRobot::setup(AGameObjectBase * gameObject)
{
	m_box->setGameObject(gameObject);
	m_box->setCollisionType(static_cast<std::size_t>(gameObject->getObjectType()));
}

void NanoRobot::addMapOffset(float x, float y)
{
	setPosition(sf::Vector2f(getPosition().x + x, getPosition().y + y));
}

void NanoRobot::transfertToOcto(void)
{
	PhysicsEngine::getInstance().unregisterShape(m_box);
	m_box = nullptr;
	m_positionBehavior->setRadius(300.f);
	m_swarm.getFirefly(0u).speed = 1.f;
	m_text->setActive(true);
	m_state = Speak;
	m_glowingEffect.onTransfer();
	Progress::getInstance().addNanoRobot();
}

void NanoRobot::setPosition(sf::Vector2f const & position)
{
	sf::Vector2f	pos = position;
	pos.y -= Tile::TripleTileSize * 2.f;
	if (std::abs(pos.x - m_swarm.getFirefly(0u).position.x) > 400.f)
		m_isTravelling = true;
	else
		m_isTravelling = false;
	m_swarm.setTarget(pos);
}

bool NanoRobot::isTravelling(void) const
{
	return m_isTravelling;
}

void NanoRobot::setState(NanoRobot::State state)
{
	m_state = state;
}

sf::Vector2f const & NanoRobot::getPosition(void) const
{
	return m_swarm.getFirefly(0u).position;
}

sf::Vector2f const & NanoRobot::getTargetPosition(void)
{
	if (std::abs(m_swarm.getTarget().x - m_swarm.getFirefly(0u).position.x) > 400.f)
		m_isTravelling = true;
	else
		m_isTravelling = false;
	return m_swarm.getTarget();
}

NanoRobot::State NanoRobot::getState(void) const
{
	return m_state;
}

void NanoRobot::update(sf::Time frametime)
{
	m_swarm.update(frametime);
	m_sprite.update(frametime);

	sf::Vector2f const & pos = m_swarm.getFirefly(0u).position;
	m_sprite.setPosition(pos - m_sprite.getGlobalSize() / 2.f);
	if (m_box)
	{
		m_box->setPosition(pos.x - m_box->getRadius(), pos.y - m_box->getRadius());
		m_box->update();
	}

	if (m_state == Idle || m_state == Speak)
	{
		m_text->setPosition(m_sprite.getPosition() - sf::Vector2f(0.f, 0.f));
		m_text->update(frametime);
	}
	if (m_state == Speak)
	{
		m_timer += frametime;
		if (m_timer > m_timerMax)
		{
			m_state = FollowOcto;
		}
	}
	m_glowingEffect.setPosition(pos);
	m_glowingEffect.update(frametime);
}

void NanoRobot::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	if (!m_isTravelling || m_state == FollowOcto || m_state == Speak)
	{
		render.draw(m_glowingEffect);
		render.draw(m_sprite);
		if (m_state == Idle || m_state == Speak)
			m_text->draw(render);
	}
}
