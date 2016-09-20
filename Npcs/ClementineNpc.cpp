#include "ClementineNpc.hpp"

ClementineNpc::ClementineNpc(void) :
	ASpecialNpc(CLEMENTINE_OSS)
{
	setSize(sf::Vector2f(22.f, 169.f));
	setOrigin(sf::Vector2f(88.f, 25.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void ClementineNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.6f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.6f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {6u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}
