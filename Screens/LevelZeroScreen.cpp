#include "LevelZeroScreen.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>
#include <PostEffectManager.hpp>

LevelZeroScreen::LevelZeroScreen(void) :
	m_spaceShip(SpaceShip::Flying),
	m_starsCount(15u),
	m_stars(new StarSystem[m_starsCount]),
	m_background(sf::Quads, 4),
	m_upColorBackground(sf::Color::Black),
	m_downColorBackground(sf::Color(8, 20, 26)),
	m_timerStartRedAlarm(sf::Time::Zero),
	m_timerStartRedAlarmMax(sf::seconds(6.f)),
	m_state(Flying),
	m_offsetCamera(0.f),
	m_keyUp(false),
	m_keyDown(false),
	m_isSoundPlayed(false),
	m_isSoundExplodePlayed(false),
	m_blinkShaderState(false),
	m_credit(new Credit(sf::Vector2f(1500.f, octo::Application::getCamera().getRectangle().height - 500.f)))
{
	m_generator.setSeed("random");
}

void	LevelZeroScreen::start()
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resource = octo::Application::getResourceManager();
	
	InputListener::addInputListener();

	m_timer = sf::Time::Zero;
	m_timerMax = sf::seconds(4.f);
	m_timerEnd = sf::Time::Zero;
	m_timerEndMax = sf::seconds(6.f);
	m_spaceShip.setPosition(sf::Vector2f(0.f, 0.f));
	for (std::size_t i = 0; i < m_starsCount; i++)
	{
		float speed = m_generator.randomFloat(1000.f, 2000.f);
		m_stars[i].setup(sf::Vector2f(2.f, 2.f));
		m_stars[i].setEmitTimeRange(0.04f, 0.1f);
		m_stars[i].canEmit(true);
		m_stars[i].setSpeed(sf::Vector2f(-speed, 0.f));
	}

	audio.startMusic(resource.getSound(MUSIC_ACTION_FAST_OGG), sf::seconds(1.f));

	if (Progress::getInstance().spaceShipIsRepair())
	{
		Progress & progress = Progress::getInstance();
		progress.setNextDestination(Level::IceA, false);
		m_state = Rising;
	}

	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();
	postEffect.removeEffects();
	PostEffectLayer::getInstance().clear();
	PostEffectLayer::getInstance().registerShader(RED_ALARM_FRAG, RED_ALARM_FRAG);
	PostEffectLayer::getInstance().getShader(RED_ALARM_FRAG).setUniform("transition", 0.f);
	PostEffectLayer::getInstance().enableShader(RED_ALARM_FRAG, true);
}

void	LevelZeroScreen::pause()
{
}

void	LevelZeroScreen::resume()
{
}

void	LevelZeroScreen::stop()
{
	Progress::getInstance().save();
	InputListener::removeInputListener();
}

void	LevelZeroScreen::update(sf::Time frameTime)
{
	octo::Camera &				camera = octo::Application::getCamera();
	sf::FloatRect const &		cameraRect = camera.getRectangle();

	m_timer += frameTime;
	m_timerEnd += frameTime;
	m_timerStartRedAlarm += frameTime;

	if (m_timerStartRedAlarm > m_timerStartRedAlarmMax && m_state == Flying)
	{
		if (m_blinkShaderState)
		{
			m_timerBlinkShader += frameTime;
			if (m_timerBlinkShader >= sf::seconds(0.55f))
			{
				m_timerBlinkShader = sf::seconds(0.55f);
				m_blinkShaderState = false;
			}
		}
		else if (!m_blinkShaderState)
		{
			m_timerBlinkShader -= frameTime;
			if (m_timerBlinkShader <= sf::Time::Zero)
			{
				octo::AudioManager &		audio = octo::Application::getAudioManager();
				octo::ResourceManager &		resource = octo::Application::getResourceManager();
				audio.playSound(resource.getSound(EVENT_SPACESHIP_ALARM_OGG), 1.f);
				m_timerBlinkShader = sf::Time::Zero;
				m_blinkShaderState = true;
			}
		}
		PostEffectLayer::getInstance().getShader(RED_ALARM_FRAG).setUniform("transition", (m_timerBlinkShader.asSeconds() / 1.0f) * 0.45f);
	}

	if (m_timer >= m_timerMax)
		m_timer -= m_timerMax;

	sf::Vector2f translation(0.f, 0.f);
	if (m_state == Flying)
	{
		if (m_keyUp)
			translation.y = -300.f * frameTime.asSeconds();
		else if (m_keyDown)
			translation.y = 300.f * frameTime.asSeconds();
	}

	switch (m_state)
	{
		case Flying:
		{
			if (m_timerEnd >= m_timerEndMax * 2.f)
			{
				m_timerEnd = sf::Time::Zero;
				m_state = Falling;
			}
			float interpolateValue = m_timerEnd / m_timerEndMax / 2.f;
			sf::Color const & color = octo::linearInterpolation(sf::Color::Black, m_downColorBackground, interpolateValue);
			m_spaceShip.setSmokeVelocity(sf::Vector2f(octo::linearInterpolation(-1400.f, -200.f, interpolateValue), 0.f));
			createBackground(sf::Vector2f(cameraRect.left, cameraRect.top), color);
			break;
		}
		case Falling:
		{
			if (m_isSoundPlayed == false)
			{
				octo::AudioManager &		audio = octo::Application::getAudioManager();
				octo::ResourceManager &		resource = octo::Application::getResourceManager();

				audio.playSound(resource.getSound(OCTO_VOICE_FALL_OGG), 1.0f);
				m_ground = audio.playSound(resource.getSound(OCTO_SOUND_GROUND_OGG), 1.f, 1.8f);
				m_isSoundPlayed = true;
			}
			if (m_timerEnd >= m_timerEndMax - sf::seconds(2.f) && !m_isSoundExplodePlayed)
			{
				octo::AudioManager &		audio = octo::Application::getAudioManager();
				octo::ResourceManager &		resource = octo::Application::getResourceManager();

				audio.stopMusic(sf::seconds(0.1f));
				audio.playSound(resource.getSound(EVENT_CRASH_OGG), 1.0f, 0.5f);
				audio.playSound(resource.getSound(DECOR_TREE_OGG), 1.0f, 0.5f);
				m_ground->setVolume(0.f);
				m_isSoundExplodePlayed = true;
			}
			if (m_timerEnd >= m_timerEndMax)
			{
				octo::StateManager & states = octo::Application::getStateManager();
				states.change("transitionLevelZero");
			}
			float interpolateValue = std::min(m_timerEnd / m_timerEndMax, 1.f);
			sf::Color const & color = octo::linearInterpolation(m_downColorBackground, sf::Color::White, interpolateValue);
			m_spaceShip.setSmokeVelocity(sf::Vector2f(-200.f, octo::linearInterpolation(-100.f, -1700.f, interpolateValue)));
			createBackground(sf::Vector2f(cameraRect.left, cameraRect.top), color);
			m_offsetCamera = -camera.getSize().x * 1.5 * interpolateValue;
			break;
		}
		case Rising:
		{
			float interpolateValue = std::min(m_timerEnd / m_timerEndMax, 1.f);
			sf::Color const & color = octo::linearInterpolation(sf::Color::White, m_downColorBackground, interpolateValue);
			m_spaceShip.setSmokeVelocity(sf::Vector2f(-1400.f, 0.f));
			createBackground(sf::Vector2f(cameraRect.left, cameraRect.top), color);
			m_offsetCamera = -camera.getSize().x * 1.5f * (1.f - interpolateValue);
			if (m_timerEnd >= m_timerEndMax)
			{
				m_timerEnd = sf::Time::Zero;
				m_state = CreditEnd;
			}
			break;
		}
		case CreditEnd:
		{
			m_credit->update(frameTime);
			if (m_credit->isFinished())
			{
				Progress & progress = Progress::getInstance();

				progress.setCurrentDestination(Level::Portal);
				progress.setNextDestination(Level::Portal);
				progress.setLastDestination(Level::Portal);
				progress.save();
				octo::Application::getStateManager().change("game");
			}
			break;
		}
		default:
		break;
	}

	m_spaceShip.move(translation);
	m_spaceShip.update(frameTime);
	for (std::size_t i = 0; i < m_starsCount; i++)
		m_stars[i].update(frameTime);
	float ratio = m_timer / m_timerMax;
	sf::Vector2f movement(std::cos(ratio * octo::Pi2) * 25.f + m_offsetCamera * 0.5f, std::sin(ratio * octo::Pi2) * 55.f);
	camera.setCenter(movement + m_spaceShip.getPosition() + sf::Vector2f(camera.getRectangle().width / 2.f + 300.f, m_spaceShip.getSize().y / 2.f + m_offsetCamera));
}

void	LevelZeroScreen::createBackground(sf::Vector2f const & position, sf::Color const & color)
{
	sf::Vector2f const & cameraSize = octo::Application::getCamera().getSize();
	m_background[0] = sf::Vertex(sf::Vector2f(-500.f, -500.f) + position, m_upColorBackground);
	m_background[1] = sf::Vertex(sf::Vector2f(cameraSize.x + 500.f, -500.f) + position, m_upColorBackground);
	m_background[2] = sf::Vertex(sf::Vector2f(cameraSize.x + 500.f, cameraSize.y + 500.f) + position, color);
	m_background[3] = sf::Vertex(sf::Vector2f(-500.f, cameraSize.y + 500.f) + position, color);
}

void	LevelZeroScreen::draw(sf::RenderTarget & render) const
{
	sf::RenderStates states;
	render.clear(sf::Color::Black);
	render.draw(m_background, states);
	for (std::size_t i = 0; i < m_starsCount; i++)
		m_stars[i].draw(render);
	m_spaceShip.drawFront(render, states);
	if (m_state == CreditEnd)
		m_credit->draw(render, states);
}

bool	LevelZeroScreen::onInputPressed(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Up:
			m_keyUp = true;
			break;
		case OctoKeys::Down:
			m_keyDown = true;
			break;
		case OctoKeys::SelectMenu:
		case OctoKeys::Jump:
		case OctoKeys::Menu:
		{
			octo::StateManager &	states = octo::Application::getStateManager();
			octo::AudioManager &	audio = octo::Application::getAudioManager();
			Progress &				progress = Progress::getInstance();

			audio.stopMusic(sf::seconds(0.1f));
			progress.setNextDestination(Level::Portal);
			progress.save();
			states.change("game");
			break;
		}
		default:
			break;
	}
	return true;
}

bool	LevelZeroScreen::onInputReleased(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Up:
			m_keyUp = false;
			break;
		case OctoKeys::Down:
			m_keyDown = false;
			break;
		default:
			break;
	}
	return true;
}
