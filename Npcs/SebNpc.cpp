#include "SebNpc.hpp"
#include "RectangleShape.hpp"

SebNpc::SebNpc(ABiome & biome) :
	ASwimNpc(SEB_OSS, biome, true)
{
    setType(GameObjectType::SebNpc);
	setSize(sf::Vector2f(70.f, 200.f));
	setOrigin(sf::Vector2f(60.f, 58.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setVelocity(15.f);
	setup();
}

void SebNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.2f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {9u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void SebNpc::update(sf::Time frameTime)
{
	ASwimNpc::update(frameTime);

	octo::CharacterSprite & sprite = getSprite();
	RectangleShape * box = getBox();

	if (sprite.getCurrentEvent() != Special1 && box->getPosition().y > getWaterLevel() + 100.f)
		box->setVelocity(sf::Vector2f(0.f, -100.f));
}
