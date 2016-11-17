#include "Rocket.hpp"
#include "PhysicsEngine.hpp"
#include "RectangleShape.hpp"
#include "CircleShape.hpp"
#include "PostEffectLayer.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Interpolations.hpp>
#include <GraphicsManager.hpp>
#include <StateManager.hpp>

Rocket::Rocket(void) :
	ANpc(ROCKET_OSS),
	m_shader(PostEffectLayer::getInstance().getShader(ROCKET_TAKEOFF_FRAG)),
	m_state(Waiting),
	m_smokesCount(3),
	m_smokes(new SmokeSystem[m_smokesCount]),
	m_timerBeforeMax(sf::seconds(3.f)),
	m_timerFirstBlastMax(sf::seconds(5.f)),
	m_timerSecondBlastMax(sf::seconds(10.f)),
	m_timerOctoEnteringMax(sf::seconds(1.f)),
	m_sound(nullptr),
	m_stopCameraMovement(false)
{
	setType(GameObjectType::Rocket);
	setSize(sf::Vector2f(160.f, 850.f));
	setOrigin(sf::Vector2f(74.f, 605.f));
	setScale(1.f);
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::Player));

	m_smokesPosition1.resize(m_smokesCount);
	m_smokesPosition1[0] = sf::Vector2f(-27.f, 775.f);
	m_smokesPosition1[1] = sf::Vector2f(80.f, 775.f);
	m_smokesPosition1[2] = sf::Vector2f(186.f, 775.f);
	m_smokesPosition2.resize(m_smokesCount);
	m_smokesPosition2[0] = sf::Vector2f(2.f, 865.f);
	m_smokesPosition2[1] = sf::Vector2f(80.f, 865.f);
	m_smokesPosition2[2] = sf::Vector2f(153.f, 865.f);
}

void Rocket::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();

	for (std::size_t i = 0u; i < m_smokesCount; i++)
	{
		m_smokes[i].setVelocity(sf::Vector2f(0.f, 100.f));
		m_smokes[i].setup(sf::Vector2f(10.f, 10.f));
		m_smokes[i].setLifeTimeRange(0.2f, 0.4f);
	}
}

void Rocket::setupMachine(void)
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

void Rocket::setPosition(sf::Vector2f const & position)
{
	if (m_state != StartSmoke)
	{
		ANpc::setPosition(position);
		for (std::size_t i = 0u; i < m_smokesCount; i++)
			m_smokes[i].setPosition(position + m_smokesPosition1[i]);
	}
}

void Rocket::addMapOffset(float x, float y)
{
	if (m_state != StartSmoke)
	{
		ANpc::addMapOffset(x, y);
		for (std::size_t i = 0u; i < m_smokesCount; i++)
			m_smokes[i].setPosition(ANpc::getPosition() + m_smokesPosition1[i]);
	}
}

void Rocket::playSound(void)
{
	if (!m_sound)
	{
		octo::AudioManager& audio = octo::Application::getAudioManager();
		octo::ResourceManager& resources = octo::Application::getResourceManager();
		m_sound = audio.playSound(resources.getSound(EVENT_ROCKET_AIR_BLAST_OGG), 1.f, 1.f, sf::Vector3f(getBox()->getBaryCenter().x, getBox()->getBaryCenter().y, -150.f) , 1000.f, 200.f);
	}
}

void Rocket::collideOctoEvent(CharacterOcto * octo)
{
	(void)octo;
}

void Rocket::collideOcto(CharacterOcto * octo)
{
	ANpc::collideOctoEvent(octo);
	m_octo = octo;
	switch (m_state)
	{
		case Waiting:
		{
			m_state = OctoEnteringX;
			break;
		}
		case OctoEnteringX:
		{
			float x = getBox()->getBaryCenter().x - 10.f;
			m_octoPosition = octo->getPhysicsPosition();
			octo->setStartPosition(octo::linearInterpolation(m_octoPosition, sf::Vector2f(x, m_octoPosition.y), m_timerOctoEntering / m_timerOctoEnteringMax));
			octo->setOctoInRocketEnd();
			break;
		}
		case OctoEnteringY:
		{
			float x = getBox()->getBaryCenter().x - 10.f;
			float y = getBox()->getPosition().y;
			octo->setStartPosition(octo::linearInterpolation(sf::Vector2f(x, m_octoPosition.y), sf::Vector2f(x, y), m_timerOctoEntering / m_timerOctoEnteringMax));
			break;
		}
		case OctoEnteringCockpit:
		{
			sf::Vector2f const & target = getBox()->getPosition() + sf::Vector2f(10.f, 0.f);
			float x = getBox()->getBaryCenter().x - 10.f;
			float y = getBox()->getPosition().y;
			octo->setStartPosition(octo::linearInterpolation(sf::Vector2f(x, y), target, m_timerOctoEntering / m_timerOctoEnteringMax));
			break;
		}
		case StartSmoke:
		{
			if (m_stopCameraMovement)
				octo->endInRocket();
			playSound();
			break;
		}
		default:
			break;
	}
}

void Rocket::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	switch (m_state)
	{
		case Waiting:
			break;
		case OctoEnteringX:
			m_timerOctoEntering += frametime;
			if (m_timerOctoEntering >= m_timerOctoEnteringMax)
			{
				m_timerOctoEntering = sf::Time::Zero;
				m_state = OctoEnteringY;
			}
			break;
		case OctoEnteringY:
			m_timerOctoEntering += frametime;
			if (m_timerOctoEntering >= m_timerOctoEnteringMax)
			{
				m_timerOctoEntering = sf::Time::Zero;
				m_state = OctoEnteringCockpit;
			}
			break;
		case OctoEnteringCockpit:
			m_timerOctoEntering += frametime;
			if (m_timerOctoEntering >= m_timerOctoEnteringMax)
			{
				m_timerOctoEntering = sf::Time::Zero;
				m_timerOctoEnteringMax = sf::seconds(15.f);
				PostEffectLayer::getInstance().enableShader(ROCKET_TAKEOFF_FRAG, true);
				m_lastPosition = getPosition();
				m_lastPositionDoor = getBox()->getPosition();
				m_state = StartSmoke;
			}
			break;
		case StartSmoke:
			if (m_sound)
				m_sound->setPosition(sf::Vector3f(getBox()->getBaryCenter().x, getBox()->getBaryCenter().y, -150.f));
			m_octo->setStartPosition(getBox()->getPosition() + sf::Vector2f(10.f, 0.f));

			m_timerOctoEntering += frametime;
			m_shader.setParameter("time", m_timerOctoEntering.asSeconds());
			m_shader.setParameter("intensity", octo::linearInterpolation(0.f, 0.3f, std::min(1.f, m_timerOctoEntering / m_timerOctoEnteringMax)));
			if (m_timerBefore < m_timerBeforeMax)
				m_timerBefore += frametime;
			else
			{
				m_timerBefore = m_timerBeforeMax;
				if (m_timerFirstBlast < m_timerFirstBlastMax)
					m_timerFirstBlast += frametime;
				else
				{
					m_timerFirstBlast = m_timerFirstBlastMax;
					if (m_timerSecondBlast < m_timerSecondBlastMax)
					{
						m_timerSecondBlast += frametime;
						if (m_timerSecondBlast > m_timerSecondBlastMax - m_timerSecondBlastMax / 10.f)
							octo::Application::getStateManager().change("rocket_end");
					}
					else
						m_timerSecondBlast = m_timerSecondBlastMax;
				}
			}

			for (std::size_t i = 0u; i < m_smokesCount; i++)
			{
				if (m_timerBefore < m_timerBeforeMax)
				{
					float coef = m_timerBefore / m_timerBeforeMax;

					m_smokes[i].setVelocity(sf::Vector2f(0.f, 100.f * coef));
					m_smokes[i].setLifeTimeRange(coef * 1.f, coef * 3.f);
					m_smokes[i].setScaleRange(coef * 1.f, coef * 3.f);
				}
				else
				{
					m_smokes[i].setVelocity(sf::Vector2f(0.f, 30.f * (1.f + m_timerFirstBlast.asSeconds() + m_timerSecondBlast.asSeconds())));
					if (m_timerFirstBlast == m_timerFirstBlastMax)
						m_smokes[i].setEmitTimeRange(0.01f, 0.05f);
					else if (m_timerSecondBlast > m_timerSecondBlastMax)
						m_smokes[i].setCanEmit(false);
				}
			}
			if (m_timerBefore >= m_timerBeforeMax)
			{
				RectangleShape * box = getBox();
				float speed = 0.f;

				if (m_timerFirstBlast < m_timerFirstBlastMax)
				{
					speed = octo::cosinusInterpolation(0.f, -400.f, m_timerFirstBlast / m_timerFirstBlastMax);
				}
				else if (m_timerSecondBlast < m_timerSecondBlastMax)
				{
					if (m_timerSecondBlast > m_timerSecondBlastMax / 2.f)
						m_stopCameraMovement = true;

					if (m_timerSecondBlast < m_timerSecondBlastMax / 10.f)
						speed = octo::cosinusInterpolation(-400.f, -1500.f, m_timerSecondBlast / (m_timerSecondBlastMax / 10.f));
					else
						speed = octo::cosinusInterpolation(-1500.f, -3500.f, (m_timerSecondBlast - m_timerSecondBlastMax / 10.f) / m_timerSecondBlastMax);
				}
				box->setVelocity(sf::Vector2f(0.f, speed));
				box->update();
			}
			break;
		default:
			break;
	}

	sprite.update(frametime);
	sf::Vector2f const & center = getBox()->getRenderPosition();
	sprite.setPosition(center);

	if (m_timerFirstBlast < m_timerFirstBlastMax * 0.8f)
	{
		for (std::size_t i = 0u; i < m_smokesCount; i++)
			m_smokes[i].setPosition(ANpc::getPosition() + m_smokesPosition1[i]);
	}
	else
	{
		for (std::size_t i = 0u; i < m_smokesCount; i++)
			m_smokes[i].setPosition(ANpc::getPosition() + m_smokesPosition2[i]);
	}
	for (std::size_t i = 0u; i < m_smokesCount; i++)
		m_smokes[i].update(frametime);

	resetVariables();
}

void Rocket::onTheFloor(void)
{
	ANpc::onTheFloor();
}

void Rocket::drawFront(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
	for (std::size_t i = 0u; i < m_smokesCount; i++)
		m_smokes[i].draw(render);
}
