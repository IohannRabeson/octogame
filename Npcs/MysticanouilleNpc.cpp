#include "MysticanouilleNpc.hpp"

MysticanouilleNpc::MysticanouilleNpc(void) :
	ASpecialNpc(MYSTICANOUILLE_OSS)
{
	setType(GameObjectType::MysticanouilleNpc);
	setSize(sf::Vector2f(62.f, 300.f));
	setOrigin(sf::Vector2f(125.f, 64.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void MysticanouilleNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.3f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {15u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}
