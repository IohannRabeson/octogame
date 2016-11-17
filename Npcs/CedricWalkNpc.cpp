#include "CedricWalkNpc.hpp"

CedricWalkNpc::CedricWalkNpc(void) :
	AWalkNpc(CEDRIC_WALK_OSS),
	m_state(WalkToOcto)
{
	setType(GameObjectType::CedricWalkNpc);
	setSize(sf::Vector2f(34.f, 164.f));
	setOrigin(sf::Vector2f(84.f, 31.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setVelocity(200.f);
	setup();
}

void CedricWalkNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::NoLoop);

	getWalkAnimation().setFrames({
			Frame(sf::seconds(0.25f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.25f), {2u, sf::FloatRect(), sf::Vector2f()}),
			});
	getWalkAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.2f), {19u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {45u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {46u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {47u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {16u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void CedricWalkNpc::collideOctoEvent(CharacterOcto * octo)
{
	ANpc::collideOctoEvent(octo);
	if (m_state == WalkToOcto)
		m_state = Speak;
}

void CedricWalkNpc::update(sf::Time frametime)
{
	ANpc::update(frametime);
}

void CedricWalkNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	switch (m_state)
	{
		case WalkToOcto:
		{
			if (sprite.getCurrentEvent() != Right && sprite.isTerminated())
			{
				reverseSprite(false);
				sprite.setNextEvent(Right);
			}
			break;
		}
		case Speak:
		{
			if (sprite.getCurrentEvent() != Idle || sprite.isTerminated())
				sprite.setNextEvent(Idle);
			break;
		}
		case WalkToRocket:
		{
			if (sprite.getCurrentEvent() != Left && sprite.isTerminated())
			{
				reverseSprite(true);
				sprite.setNextEvent(Left);
			}
			break;
		}
		case Stop:
		{
			if (sprite.getCurrentEvent() != Idle)
				sprite.setNextEvent(Idle);
			break;
		}
		default:
			break;
	}
}
