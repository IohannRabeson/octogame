#include "CedricIceANpc.hpp"
#include "RectangleShape.hpp"
#include <Interpolations.hpp>

CedricIceANpc::CedricIceANpc(void) :
	ASpecialNpc(CEDRIC_ICE_A_OSS, false),
	m_timerMoveMax(sf::seconds(8.f)),
	m_timerDisapearMax(sf::seconds(1.f))
{
    setType(GameObjectType::CedricIceANpc);
	setSize(sf::Vector2f(34.f, 164.f));
	setOrigin(sf::Vector2f(84.f, 31.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(-10.f, -50.f));
	setup();
	reverseSprite(true);
}

void CedricIceANpc::setup(void)
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
			Frame(sf::seconds(0.3f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void CedricIceANpc::update(sf::Time frametime)
{
	ANpc::update(frametime);
	octo::CharacterSprite & sprite = getSprite();
	sf::Vector2f const & boxPosition = getBox()->getPosition();

	m_timerMove += frametime;
	if (m_timerMove <= m_timerMoveMax)
	{
		sprite.setPosition(sf::Vector2f(octo::linearInterpolation(boxPosition.x, boxPosition.x - 800.f, m_timerMove / m_timerMoveMax), sprite.getPosition().y));
		if (m_timerMove >= m_timerMoveMax - m_timerDisapearMax)
		{
			m_timerDisapear = std::min(m_timerDisapear + frametime, m_timerDisapearMax);
			sprite.setColor(sf::Color(255, 255, 255, octo::linearInterpolation(255.f, 0.f, m_timerDisapear / m_timerDisapearMax)));
		}
	}
}
