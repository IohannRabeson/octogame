#include "JuNpc.hpp"

JuNpc::JuNpc(void) :
	ASpecialNpc(JU_OSS, true, true)
{
	setType(GameObjectType::JuNpc);
	setSize(sf::Vector2f(31.f, 162.f));
	setOrigin(sf::Vector2f(82.f, 31.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();
}

void JuNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {17u, sf::FloatRect(), sf::Vector2f()})
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {20u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {21u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {22u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {23u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {24u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {25u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {26u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {27u, sf::FloatRect(), sf::Vector2f()})
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}
