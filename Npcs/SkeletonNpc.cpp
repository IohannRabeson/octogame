#include "SkeletonNpc.hpp"
#include <ResourceManager.hpp>
#include <Application.hpp>

SkeletonNpc::SkeletonNpc(void) :
	AIdleNpc(SKELETON_1_OSS, false, true),
	m_generator("random")
{
    setType(GameObjectType::SkeletonNpc);
	setSize(sf::Vector2f(112.f, 200.f));
	setOrigin(sf::Vector2f(44.f, 295.f));
	setSprite();
	setup();
	setIsZoomable(false);
}

void SkeletonNpc::setSprite()
{
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	octo::CharacterSprite &		sprite = getSprite();
	std::size_t const			index = m_generator.randomInt(0, 4);
	float const					scale = m_generator.randomFloat(0.5f, 1.f);

	switch (index)
	{
		case 0u:
			sprite.setSpriteSheet(resources.getSpriteSheet(COLUMN_1_OSS));
			break;
		case 1u:
			sprite.setSpriteSheet(resources.getSpriteSheet(COLUMN_2_OSS));
			break;
		case 2u:
			sprite.setSpriteSheet(resources.getSpriteSheet(COLUMN_3_OSS));
			break;
		case 3u:
			sprite.setSpriteSheet(resources.getSpriteSheet(COLUMN_4_OSS));
			break;
		case 4u:
			sprite.setSpriteSheet(resources.getSpriteSheet(COLUMN_5_OSS));
			break;
		default:
			break;
	}
	setScale(scale);
}

void SkeletonNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}
