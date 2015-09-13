#include "NanoRobot.hpp"
#include "FireflyPositionBehaviors.hpp"
#include "ResourceDefinitions.hpp"
#include "Tile.hpp"
#include "PhysicsEngine.hpp"
#include "CircleShape.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <sstream>

NanoRobot::NanoRobot(sf::Vector2f const & position, std::string id, std::size_t nbFrames) :
	m_swarm(1),
	m_uniformPopulation(1234u, &octo::Application::getResourceManager().getPalette(FROM_SEA1_OPA),
						1.2f, 2.f, 6.f, 10.f, 32.f, 50.f,
						sf::Time::Zero, sf::Time::Zero),
	m_spawnMode(FireflySwarm::SpawnMode::Normal),
	m_box(PhysicsEngine::getShapeBuilder().createCircle(false))
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_box->setRadius(100.f);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setCollisionMask(static_cast<std::uint32_t>(GameObjectType::Player));
	m_box->setApplyGravity(false);

	m_swarm.setPositionBehavior(new FireflySwarm::CirclePositionBehavior(2345, 50.f));
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
}

NanoRobot::~NanoRobot(void)
{}

void NanoRobot::setup(AGameObjectBase * gameObject)
{
	m_box->setGameObject(gameObject);
	m_box->setCollisionType(static_cast<std::uint32_t>(gameObject->getObjectType()));
}

void NanoRobot::setPosition(sf::Vector2f const & position)
{
	sf::Vector2f	pos = position;
	pos.y -= Tile::TripleTileSize * 2.f;
	m_swarm.setTarget(pos);
}

void NanoRobot::update(sf::Time frametime)
{
	m_swarm.update(frametime);
	m_sprite.update(frametime);

	sf::Vector2f pos = m_swarm.getFirefly(0u).position;
	m_sprite.setPosition(pos - sf::Vector2f(32.f, 32.f));
	m_box->setPosition(pos.x - m_box->getRadius(), pos.y - m_box->getRadius());

	m_text->setPosition(m_sprite.getPosition() - sf::Vector2f(0.f, 0.f));
	m_text->update(frametime);
	m_text->setActive(true);
}

void NanoRobot::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	render.draw(m_sprite);
	m_text->draw(render);
}
