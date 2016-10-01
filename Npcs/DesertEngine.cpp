#include "DesertEngine.hpp"
#include <ResourceManager.hpp>
#include <Application.hpp>

DesertEngine::DesertEngine(void) :
	AIdleNpc(DESERT_ENGINE_1_OSS, false, true),
	m_generator("random")
{
    setType(GameObjectType::DesertEngine);
	setSize(sf::Vector2f(100.f, 452.f));
	setOrigin(sf::Vector2f(103.f, 137.f));
	setSprite();
	setup();
}

void DesertEngine::setSprite()
{
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	octo::CharacterSprite &		sprite = getSprite();
	std::size_t const			index = m_generator.randomInt(0, 2);
	float const					scale = m_generator.randomFloat(0.5f, 1.f);

	switch (index)
	{
		case 0u:
			sprite.setSpriteSheet(resources.getSpriteSheet(DESERT_ENGINE_1_OSS));
			break;
		case 1u:
			sprite.setSpriteSheet(resources.getSpriteSheet(DESERT_ENGINE_2_OSS));
			break;
		case 2u:
			sprite.setSpriteSheet(resources.getSpriteSheet(DESERT_ENGINE_3_OSS));
			break;
		default:
			break;
	}
	setScale(scale);
}

void DesertEngine::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}
