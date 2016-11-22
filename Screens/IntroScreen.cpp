#include "IntroScreen.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"

#include "IceABiome.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <StateManager.hpp>
#include <Options.hpp>
#include <PostEffectManager.hpp>

IntroScreen::IntroScreen(void) :
	m_state(Fly),
	m_keyJump(false),
	m_keyUp(false),
	m_keyDown(false),
	m_keyRight(false),
	m_keyLeft(false),
	m_ticJump(false),
	m_firstCloudCollided(false),
	m_timerBeforeCrashMax(sf::seconds(20.f)),
	m_timerJumpMax(sf::seconds(1.f)),
	m_timerFallMax(sf::seconds(0.5f)),
	m_doSave(false),
	m_musicPlayer(MusicManager::getInstance()),
	m_skyCycle(nullptr),
	m_skyManager(nullptr),
	m_parallaxScrolling(nullptr),
	m_spaceShip(SpaceShip::Flying),
	m_speed(2100.f, 0.f),
	m_timerCameraMax(sf::seconds(4.f)),
	m_timerMax(sf::seconds(4.f)),
	m_timerStartRedAlarmMax(sf::seconds(0.1f)),
	m_timerEndMax(sf::seconds(6.f)),
	m_blinkShaderState(false)
{
    octo::GraphicsManager &	graphics = octo::Application::getGraphicsManager();
	octo::Application::getPostEffectManager().createRender(graphics.getVideoMode());

	m_biomeManager.registerBiome<IceABiome>(Level::IceA);
	m_biomeManager.changeBiome(Level::IceA, 0x12345);
	m_spaceShip.setPosition(sf::Vector2f(0.f, 0.f));
}

void	IntroScreen::start()
{
	Progress &				progress = Progress::getInstance();
	
	InputListener::addInputListener();
	progress.load("save.osv");
	progress.setMenu(false);
	progress.setIntro(true);
	m_menu.setup();
	setupScene();
}

void	IntroScreen::pause()
{
}

void	IntroScreen::resume()
{
}

void	IntroScreen::stop()
{
	octo::Application::getAudioManager().stopMusic(sf::Time::Zero);
	octo::Application::getPostEffectManager().removeEffects();
	InputListener::removeInputListener();
	Progress::getInstance().setIntro(false);
	m_menu.setKeyboard(false);
}

void	IntroScreen::update(sf::Time frameTime)
{
	AMenu::State				state = m_menu.getState();
	//octo::StateManager &		states = octo::Application::getStateManager();
	Progress &					progress = Progress::getInstance();

	m_menu.update(frameTime, m_spaceShip.getPosition() + sf::Vector2f(1025.f, 150.f));
	if (state == AMenu::State::Active || state == AMenu::State::Draw)
	{
		progress.setBubbleNpc(false);
		m_doSave = true;
	}
	else if (m_doSave)
	{
		progress.setBubbleNpc(true);
		progress.save();
		m_doSave = false;
	}
	else
	{
		m_menu.setKeyboard(false);
		updateScene(frameTime);
	}
}

void	IntroScreen::setupScene(void)
{
	octo::AudioManager&			audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resource = octo::Application::getResourceManager();
	octo::PostEffectManager&	postEffect = octo::Application::getPostEffectManager();
	sf::Vector2f				startPosition;

	audio.reset();
	audio.startMusic(resource.getSound(MUSIC_ACTION_FAST_OGG), sf::seconds(1.f));
	// Reset last values
	postEffect.removeEffects();
	PostEffectLayer::getInstance().clear();
	PostEffectLayer::getInstance().registerShader(CIRCLE_RAINBOW_FRAG, CIRCLE_RAINBOW_FRAG);
	PostEffectLayer::getInstance().registerShader(VISION_TROUBLE_FRAG, VISION_TROUBLE_FRAG);
	PostEffectLayer::getInstance().registerShader(PERSISTENCE_FRAG, PERSISTENCE_FRAG);
	PostEffectLayer::getInstance().registerShader(PIXELATE_FRAG, PIXELATE_FRAG);
	PostEffectLayer::getInstance().registerShader(DISPLACEMENT_FRAG, DISPLACEMENT_FRAG);
	PostEffectLayer::getInstance().registerShader(KERNEL_POST_EFFECT_FRAG, KERNEL_POST_EFFECT_FRAG);
	PostEffectLayer::getInstance().registerShader("render_black_kernel", KERNEL_POST_EFFECT_FRAG);
	PostEffectLayer::getInstance().registerShader("render_white_kernel", KERNEL_POST_EFFECT_FRAG);
	PostEffectLayer::getInstance().registerShader(WATER_FRAG, WATER_FRAG);
	PostEffectLayer::getInstance().registerShader(VORTEX_FRAG, VORTEX_FRAG);
	PostEffectLayer::getInstance().registerShader("vortex_red", VORTEX_FRAG);
	PostEffectLayer::getInstance().registerShader("vortex_blue", VORTEX_FRAG);
	PostEffectLayer::getInstance().registerShader("vortex_white", VORTEX_FRAG);
	PostEffectLayer::getInstance().registerShader(RED_ALARM_FRAG, RED_ALARM_FRAG);
	PostEffectLayer::getInstance().registerShader(DUPLICATE_SCREEN_FRAG, DUPLICATE_SCREEN_FRAG);
	PostEffectLayer::getInstance().registerShader(ROCKET_TAKEOFF_FRAG, ROCKET_TAKEOFF_FRAG);
	PostEffectLayer::getInstance().registerShader(CUTSCENE_FRAG, CUTSCENE_FRAG);
	PostEffectLayer::getInstance().registerShader(CIRCLE_WAVE_FRAG, CIRCLE_WAVE_FRAG);
	PostEffectLayer::getInstance().registerShader(COLOR_SATURATION_FRAG, COLOR_SATURATION_FRAG);

	PostEffectLayer::getInstance().enableShader(CUTSCENE_FRAG, true);
	PostEffectLayer::getInstance().getShader(CUTSCENE_FRAG).setParameter("height", 0.15f);
	PostEffectLayer::getInstance().getShader(CUTSCENE_FRAG).setParameter("time", 1.f);
	PostEffectLayer::getInstance().registerShader(RED_ALARM_FRAG, RED_ALARM_FRAG);
	PostEffectLayer::getInstance().getShader(RED_ALARM_FRAG).setParameter("transition", 0.f);
	PostEffectLayer::getInstance().enableShader(RED_ALARM_FRAG, true);

	octo::Application::getCamera().setCenter(startPosition - sf::Vector2f(0.f, 200.f));

	m_skyCycle.reset(new SkyCycle());
	m_skyManager.reset(new SkyManager());
	m_parallaxScrolling.reset(new ParallaxScrolling());

	m_skyCycle->setup(m_biomeManager.getCurrentBiome());
	m_skyManager->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_parallaxScrolling->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);

	m_spaceShip.setPosition(sf::Vector2f(0.f, -7000.f));
}

void	IntroScreen::updateCamera(sf::Time frameTime)
{
	octo::Camera &	camera = octo::Application::getCamera();
	float			ratio = m_timerCamera / m_timerCameraMax;
	sf::Vector2f	movement(std::cos(ratio * octo::Pi2) * 25.f, std::sin(ratio * octo::Pi2) * 55.f);

	m_timerCamera += frameTime;
	camera.setCenter(movement + m_spaceShip.getPosition() + sf::Vector2f(camera.getRectangle().width / 2.f + 300.f, m_spaceShip.getSize().y / 2.f));
}

void	IntroScreen::updateSpaceShip(sf::Time frameTime)
{
	Progress &					progress = Progress::getInstance();
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resource = octo::Application::getResourceManager();
	sf::Vector2f				translation(m_speed.x * frameTime.asSeconds(), 0.f);

	switch (m_state)
	{
		case Fly:
		{
			m_timerBeforeCrash = std::min(m_timerBeforeCrash + frameTime, m_timerBeforeCrashMax);
		
			if (m_keyJump)
			{
				if (!m_ticJump && m_firstCloudCollided)
				{
					m_timerJump = m_timerJumpMax;
					m_ticJump = true;
				}
			}
			else if (m_timerJump <= sf::Time::Zero)
				m_ticJump = false;
			m_timerJump = std::max(m_timerJump - frameTime, sf::Time::Zero);
			translation.y = octo::cosinusInterpolation(300.f * frameTime.asSeconds(), -300.f * frameTime.asSeconds(), m_timerJump / m_timerJumpMax);
		
			if (m_timerFall <= m_timerFallMax)
			{
				m_timerFall += frameTime;
				translation.y += 1000.f * (m_timerFall / m_timerFallMax) * frameTime.asSeconds();
			}
			else if (Progress::getInstance().isInCloud())
			{
				m_firstCloudCollided = true;
				audio.playSound(resource.getSound(DECOR_THUNDER1_STEREO_OGG), 1.f, 3.f);
				m_timerFall = sf::Time::Zero;
				m_speed.x -= 250.f;
				if (m_speed.x < 1000.f)
					m_state = Crash;
			}
			break;
		}
		case Crash:
		{
			m_timerFall += frameTime;
			if (m_timerFall <= m_timerFallMax)
				translation.y += 600.f * (m_timerFall / m_timerFallMax) * frameTime.asSeconds();
			else
				translation.y += 600.f * frameTime.asSeconds();

			if (m_spaceShip.getPosition().y > 0.f)
			{
				audio.playSound(resource.getSound(EVENT_CRASH_OGG), 1.f);
				m_state = End;
			}
			break;
		}
		default:
			break;
	}

	if (m_keyUp)
		translation.y -= 300.f * frameTime.asSeconds();
	if (m_keyDown)
		translation.y += 150.f * frameTime.asSeconds();
	if (m_keyRight)
		translation.x += 300.f * frameTime.asSeconds();
	if (m_keyLeft)
		translation.x -= 300.f * frameTime.asSeconds();

	if (m_firstCloudCollided)
		updateAlarm(frameTime);

	m_spaceShip.move(translation);
	m_spaceShip.update(frameTime);

	progress.setOctoPos(m_spaceShip.getPosition() + sf::Vector2f(1045.f, 90.f));
}

void	IntroScreen::updateAlarm(sf::Time frameTime)
{
	m_timer += frameTime;
	m_timerEnd += frameTime;
	m_timerStartRedAlarm += frameTime;

	if (m_timerStartRedAlarm > m_timerStartRedAlarmMax)
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
				audio.stopMusic(sf::seconds(0.4f));
				m_timerBlinkShader = sf::Time::Zero;
				m_blinkShaderState = true;
			}
		}
		PostEffectLayer::getInstance().getShader(RED_ALARM_FRAG).setParameter("transition", (m_timerBlinkShader.asSeconds() / 1.0f) * 0.75f);
	}

}

void	IntroScreen::updateScene(sf::Time frameTime)
{
	updateSpaceShip(frameTime);
	updateCamera(frameTime);
	m_musicPlayer.update(frameTime, m_spaceShip.getPosition());
	m_skyCycle->update(frameTime, m_biomeManager.getCurrentBiome());
	m_parallaxScrolling->update(frameTime.asSeconds());
	m_skyManager->update(frameTime);
}

void	IntroScreen::drawScene(sf::RenderTarget& render) const
{
	sf::RenderStates states;
	render.clear();
	render.draw(m_skyManager->getDecorsBack(), states);
	render.draw(*m_parallaxScrolling, states);
	m_spaceShip.drawFront(render, states);
	render.draw(m_skyManager->getDecorsFront(), states);
	render.draw(m_skyManager->getFilter(), states);
}

bool IntroScreen::onInputPressed(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Menu:
		{
			AMenu::State state = m_menu.getState();
			if (state == AMenu::State::Hide)
				m_menu.setState(AMenu::State::Active);
			break;
		}
		case OctoKeys::Jump:
			m_keyJump = true;
			break;
		case OctoKeys::Up:
			m_keyUp = true;
			break;
		case OctoKeys::Down:
			m_keyDown = true;
			break;
		case OctoKeys::Right:
			m_keyRight = true;
			break;
		case OctoKeys::Left:
			m_keyLeft = true;
			break;
		default:
			break;
	}
	return (true);
}

bool IntroScreen::onInputReleased(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Jump:
			m_keyJump = false;
			break;
		case OctoKeys::Up:
			m_keyUp = false;
			break;
		case OctoKeys::Down:
			m_keyDown = false;
			break;
		case OctoKeys::Right:
			m_keyRight = false;
			break;
		case OctoKeys::Left:
			m_keyLeft = false;
			break;
		default:
			break;
	}
	return (true);
}
void	IntroScreen::draw(sf::RenderTarget& render)const
{
	drawScene(render);
	if (m_menu.getState() == AMenu::State::Active || m_menu.getState() == AMenu::State::Draw)
		render.draw(m_menu);
}
