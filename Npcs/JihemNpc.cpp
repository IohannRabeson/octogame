#include "JihemNpc.hpp"

JihemNpc::JihemNpc(void) :
	ASpecialNpc(JIHEM_OSS)
{
    setType(GameObjectType::JihemNpc);
	setSize(sf::Vector2f(40.f, 194.f));
	setOrigin(sf::Vector2f(85.f, 57.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void JihemNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.5f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.5f), {5u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}
