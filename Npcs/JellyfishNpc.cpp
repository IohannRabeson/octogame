#include "JellyfishNpc.hpp"
#include "RectangleShape.hpp"
#include <Interpolations.hpp>

JellyfishNpc::JellyfishNpc(ABiome & biome) :
	ASwimNpc(JELLYFISH_OSS, biome, false)
{
	setType(GameObjectType::JellyfishNpc);
	setSize(sf::Vector2f(5.f, 150.f));
	setOrigin(sf::Vector2f(51.f, 81.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -20.f));
	setup();
}

void JellyfishNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void JellyfishNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();
	RectangleShape * box = getBox();
	float angle = octo::rad2Deg(std::atan2(box->getPosition().y - getOctoPosition().y, box->getPosition().x - getOctoPosition().x)) - 90.f;

	if (sprite.getCurrentEvent() == Special1)
	{
		if (angle < 0.f)
			angle += 360.f;
		sprite.setRotation(angle);
	}
	else
	{
		if (sprite.getRotation() > 180.f)
			sprite.setRotation(octo::linearInterpolation(360.f, sprite.getRotation(), 1.f - frametime.asSeconds()));
		else
			sprite.setRotation(octo::linearInterpolation(0.f, sprite.getRotation(), 1.f - frametime.asSeconds()));
	}

	ASwimNpc::update(frametime);
}
