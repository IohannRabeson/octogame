#include "AnthemJungle.hpp"

AnthemJungle::AnthemJungle(void) :
	ASpecialNpc(ANTHEM_JUNGLE_OSS, false)
{
	setSize(sf::Vector2f(158.f, 3150.f));
	setOrigin(sf::Vector2f(170.f, 39.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void AnthemJungle::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(3.0f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(2.0f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.2f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(1.5f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {2u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}
