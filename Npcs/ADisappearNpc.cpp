#include "ADisappearNpc.hpp"
#include "RectangleShape.hpp"

RandomGenerator ADisappearNpc::m_generator("random");

ADisappearNpc::ADisappearNpc(ResourceKey const & key, float alphaMin, float alphaMax, bool isFlying) :
	ANpc(key, false),
	m_isVisible(true),
	m_isFlying(isFlying),
	m_alphaCurrent(0.f),
	m_alphaTarget(m_generator.randomFloat(alphaMin, alphaMax)),
	m_alphaMin(alphaMin),
	m_alphaMax(alphaMax),
	m_alphaAlmostDisappear(m_generator.randomFloat(0.f, 2.5f)),
	m_randomAppearTimer(sf::seconds(m_generator.randomFloat(1.f, 5.f)))
{
	setSize(sf::Vector2f(25.f, 60.f));
	setOrigin(sf::Vector2f(90.f, m_generator.randomFloat(50.f, 400.f)));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(-20.f, -10.f));
	setTimerMax(sf::seconds(8.0f));

	setupBox(this, static_cast<std::size_t>(GameObjectType::DisappearNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void ADisappearNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);

	setMachine(machine);
	setNextEvent(Idle);
}

void ADisappearNpc::makeDisappear(sf::Time frametime)
{
	m_randomAppearTimer -= frametime;
	m_randomDisappearTimer -= frametime;

	if (getCollideEventOcto())
		m_isVisible = false;
	if (m_randomDisappearTimer <= sf::Time::Zero)
	{
		m_isVisible = false;
		m_randomDisappearTimer = sf::seconds(m_generator.randomFloat(1.f, 15.f));
	}

	if (!m_isVisible)
	{
		if (m_alphaCurrent - frametime.asSeconds() * m_alphaTarget > 2.f)
			m_alphaCurrent -= frametime.asSeconds() * m_alphaTarget;
		else
		{
			m_randomAppearTimer = sf::seconds(m_generator.randomFloat(1.f, 15.f));
			m_alphaTarget = m_generator.randomFloat(m_alphaMin, m_alphaMax);
			m_alphaAlmostDisappear = m_generator.randomFloat(0.f, 2.0f);
			m_isVisible = true;
			if (m_isFlying)
				setOrigin(sf::Vector2f(m_generator.randomFloat(20.f, 120.f), m_generator.randomFloat(50.f, 400.f)));
		}
	}
	else if (m_randomAppearTimer <= sf::Time::Zero && m_alphaCurrent + frametime.asSeconds() * m_alphaTarget < m_alphaTarget)
		m_alphaCurrent += frametime.asSeconds() * m_alphaTarget;
}

void ADisappearNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	updatePhysics();

	makeDisappear(frametime);

	sprite.update(frametime);
	sprite.setPosition(getBox()->getRenderPosition());
	sprite.setColor(sf::Color(255, 255, 255, m_alphaCurrent));

	updateText(frametime);
	resetVariables();
}

RandomGenerator & ADisappearNpc::getGenerator(void)
{
	return m_generator;
}
