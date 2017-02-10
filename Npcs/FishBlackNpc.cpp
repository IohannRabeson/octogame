#include "FishBlackNpc.hpp"

FishBlackNpc::FishBlackNpc(ABiome & biome) :
	AFishNpc(FISH_BLACK_OSS, biome, false)
{
    setType(GameObjectType::FishBlackNpc);
	setSize(sf::Vector2f(93.f, 24.f));
	setOrigin(sf::Vector2f(57.f, 45.f));
	setScale(randomFloat(0.5f, 0.9f));
	setup();
}

void FishBlackNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.2f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}
