#include "ChamanMonsterNpc.hpp"
#include "RectangleShape.hpp"
#include "Progress.hpp"

ChamanMonsterNpc::ChamanMonsterNpc(void) :
	ASpecialNpc(CHAMAN_MONSTER_OSS, false)
{
	setType(GameObjectType::ChamanMonsterNpc);
	setSize(sf::Vector2f(220.f, 700.f));
	setOrigin(sf::Vector2f(37.f, 12.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, 0.f));
	setup();
}

void ChamanMonsterNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.3f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void ChamanMonsterNpc::update(sf::Time frameTime)
{
	Progress const & progress = Progress::getInstance();
	octo::CharacterSprite & sprite = getSprite();

	ASpecialNpc::update(frameTime);
	sf::Vector2f position = octo::cosinusInterpolation(sprite.getPosition(), progress.getOctoPos() + sf::Vector2f(0.f, -600.f), 0.3f);
	sprite.setPosition(position);
}
