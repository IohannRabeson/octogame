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
	m_cloudCollidedCount(0u),
	m_cloudCollidedCountMax(4u),
	m_timerBeforeCrashMax(sf::seconds(20.f)),
	m_timerJumpMax(sf::seconds(1.f)),
	m_timerFallMax(sf::seconds(0.5f)),
	m_cloudMinY(0.f),
	m_cloudMaxY(0.f),
	m_speedFall(600.f),
	m_speedYUp(500.f),
	m_doSave(false),
	m_musicPlayer(MusicManager::getInstance()),
	m_skyCycle(nullptr),
	m_skyManager(nullptr),
	m_parallaxScrolling(nullptr),
	m_starsCount(15u),
	m_stars(new StarSystem[m_starsCount]),
	m_spaceShip(SpaceShip::Flying),
	m_speedX(2100.f),
	m_timerCameraMax(sf::seconds(4.f)),
	m_timerMax(sf::seconds(4.f)),
	m_timerStartRedAlarmMax(sf::seconds(0.1f)),
	m_timerEndMax(sf::seconds(6.f)),
	m_blinkShaderState(false),
	m_timerShaderRocketAppearMax(sf::seconds(1.f)),
	m_shaderRocketState(false),
	m_shakeTimer(sf::Time::Zero),
	m_shakeDuration(sf::seconds(1.f)),
	m_waveDuration(sf::seconds(1.f)),
	m_shakeIntensity(0.f)
{
	octo::StateManager &	states = octo::Application::getStateManager();
    octo::GraphicsManager &	graphics = octo::Application::getGraphicsManager();
	octo::Application::getPostEffectManager().createRender(graphics.getVideoMode());

	m_biomeManager.registerBiome<IceABiome>(Level::IceA);
	m_biomeManager.changeBiome(Level::IceA, 0x12345);
	m_spaceShip.setPosition(sf::Vector2f(0.f, 0.f));

	states.setTransitionDuration(sf::seconds(2.f), sf::seconds(2.f));
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
	//octo::Application::getPostEffectManager().removeEffects();
	InputListener::removeInputListener();
	Progress::getInstance().setIntro(false);
	m_menu.setKeyboard(false);
	Progress::getInstance().save();
}

void	IntroScreen::update(sf::Time frameTime)
{
	AMenu::State				state = m_menu.getState();
	Progress &					progress = Progress::getInstance();

	m_globalTime += frameTime;
	m_menu.update(frameTime, m_spaceShip.getPosition() + sf::Vector2f(1000.f, 100.f));
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
	PostEffectLayer::getInstance().getShader(RED_ALARM_FRAG).setParameter("transition", 0.f);
	PostEffectLayer::getInstance().enableShader(RED_ALARM_FRAG, true);
	PostEffectLayer::getInstance().enableShader(ROCKET_TAKEOFF_FRAG, true);

	octo::Application::getCamera().setCenter(startPosition - sf::Vector2f(0.f, 200.f));

	m_skyCycle.reset(new SkyCycle());
	m_skyManager.reset(new SkyManager());
	m_parallaxScrolling.reset(new ParallaxScrolling());

	m_skyCycle->setup(m_biomeManager.getCurrentBiome());
	m_skyManager->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);
	m_parallaxScrolling->setup(m_biomeManager.getCurrentBiome(), *m_skyCycle);

	m_cloudMinY = m_biomeManager.getCurrentBiome().getCloudMinY();
	m_cloudMaxY = m_biomeManager.getCurrentBiome().getCloudMaxY();
	m_spaceShip.setPosition(sf::Vector2f(0.f, m_cloudMinY - 1500.f));

	for (std::size_t i = 0; i < m_starsCount; i++)
	{
		float speed = m_biomeManager.getCurrentBiome().randomFloat(1000.f, 2000.f);
		m_stars[i].setup(sf::Vector2f(2.f, 2.f));
		m_stars[i].setEmitTimeRange(0.04f, 0.1f);
		m_stars[i].canEmit(true);
		m_stars[i].setSpeed(sf::Vector2f(-speed, 0.f));
	}
}

void	IntroScreen::updateSpaceShip(sf::Time frameTime)
{
	Progress &					progress = Progress::getInstance();
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resource = octo::Application::getResourceManager();
	sf::Vector2f				translation(m_speedX * frameTime.asSeconds(), 0.f);
	sf::Vector2f const &		positionSpaceShip = m_spaceShip.getPosition();

	switch (m_state)
	{
		case Fly:
		{
			m_timerBeforeCrash = std::min(m_timerBeforeCrash + frameTime, m_timerBeforeCrashMax);
		
			if (m_keyJump)
			{
				if (!m_ticJump && positionSpaceShip.y >= m_cloudMinY + 1000.f && positionSpaceShip.y < m_cloudMaxY)
				{
					m_timerJump = m_timerJumpMax;
					m_ticJump = true;
				}
			}
			else if (m_timerJump <= sf::Time::Zero)
				m_ticJump = false;
			m_timerJump = std::max(m_timerJump - frameTime, sf::Time::Zero);
			translation.y = octo::cosinusInterpolation(300.f * frameTime.asSeconds(), -m_speedYUp * frameTime.asSeconds(), m_timerJump / m_timerJumpMax);
		
			if (m_timerFall <= m_timerFallMax)
			{
				m_timerFall += frameTime;
				translation.y += octo::cosinusInterpolation(0.f, 1500.f * frameTime.asSeconds(), m_timerFall / m_timerFallMax);
			}
			else if (Progress::getInstance().isInCloud())
			{
				m_cloudCollidedCount = std::min(m_cloudCollidedCount + 1u, m_cloudCollidedCountMax);
				m_speedYUp = 500.f;
				m_shaderRocketState = true;
				shake(2.f + m_cloudCollidedCount, m_cloudCollidedCount, 0.1f);
				for (std::size_t i = 0; i < m_starsCount; i++)
					m_stars[i].canEmit(false);
				if (m_cloudCollidedCount >= 3u)
					audio.stopMusic(sf::seconds(0.4f));
				else
					audio.setMusicVolume((3u - m_cloudCollidedCount) * 0.2f);
				audio.playSound(resource.getSound(OCTO_SOUND_SLOW_FALL_OGG), 1.f, 3.f);
				m_timerFall = sf::Time::Zero;
				m_speedX -= 300.f;
				if (m_cloudCollidedCount >= m_cloudCollidedCountMax)
					m_state = Crash;
				if (m_globalTime >= sf::seconds(25.f))
					Progress::getInstance().setLongIntro(true);
			}
			if (m_cloudCollidedCount >= 1u)
				m_speedYUp -= frameTime.asSeconds() * 10.f;
			if (positionSpaceShip.y > m_cloudMaxY)
				m_state = Crash;
			break;
		}
		case Crash:
		{
			m_timerFall += frameTime;
			m_speedFall += frameTime.asSeconds() * 600.f;
			if (m_timerFall <= m_timerFallMax)
				translation.y += m_speedFall * (m_timerFall / m_timerFallMax) * frameTime.asSeconds();
			else
				translation.y += m_speedFall * frameTime.asSeconds();

			if (positionSpaceShip.y > m_cloudMaxY)
			{
				m_state = End;
			}
			break;
		}
		case End:
		{
			m_speedFall += frameTime.asSeconds() * 600.f;
			translation.y += m_speedFall * frameTime.asSeconds();
			if (positionSpaceShip.y > m_cloudMaxY + octo::Application::getCamera().getSize().y)
			{
				octo::StateManager &		states = octo::Application::getStateManager();

				states.setTransitionDuration(sf::seconds(0.2f), sf::seconds(2.f));
				states.change("transitionLevelZero", "white");
				audio.playSound(resource.getSound(EVENT_CRASH_OGG), 1.f);
				m_state = None;
			}
			break;
		}
		default:
			break;
	}

	if (m_state != End && m_state != Crash && positionSpaceShip.y >= m_cloudMinY + 1000.f && positionSpaceShip.y < m_cloudMaxY)
	{
		if (m_keyUp)
			translation.y -= m_speedYUp * frameTime.asSeconds();
		if (m_keyDown)
			translation.y += 350.f * frameTime.asSeconds();
		if (m_keyRight)
			translation.x += 200.f * frameTime.asSeconds();
		if (m_keyLeft)
			translation.x -= 200.f * frameTime.asSeconds();
	}

	if (m_cloudCollidedCount >= 3u)
		updateAlarm(frameTime);


	m_spaceShip.move(translation);
	m_spaceShip.update(frameTime);

	progress.setOctoPos(positionSpaceShip + sf::Vector2f(1045.f, 90.f));
}

void	IntroScreen::updateShaders(sf::Time frameTime)
{
	m_timerShaderRocket += frameTime;
	if (m_state != End && m_state != Crash)
	{
		if (m_shaderRocketState && m_timerShaderRocketAppear <= m_timerShaderRocketAppearMax)
			m_timerShaderRocketAppear += frameTime * 5.f;
		else
		{
			m_shaderRocketState = false;
			m_timerShaderRocketAppear = std::max(m_timerShaderRocketAppear - frameTime, sf::Time::Zero);
		}
	}
	else if (m_timerShaderRocketAppear <= m_timerShaderRocketAppearMax)
		m_timerShaderRocketAppear += frameTime * 5.f;

	PostEffectLayer::getInstance().getShader(ROCKET_TAKEOFF_FRAG).setParameter("intensity", octo::linearInterpolation(0.f, 0.05f * m_cloudCollidedCount, std::min(1.f, m_timerShaderRocketAppear / m_timerShaderRocketAppearMax)));
	PostEffectLayer::getInstance().getShader(ROCKET_TAKEOFF_FRAG).setParameter("time", m_timerShaderRocket.asSeconds());

	if (m_cloudCollidedCount == 1u)
		PostEffectLayer::getInstance().getShader(CUTSCENE_FRAG).setParameter("time", 1.f - m_timerFall / m_timerFallMax);
	else if (m_cloudCollidedCount == m_cloudCollidedCountMax)
		PostEffectLayer::getInstance().getShader(CUTSCENE_FRAG).setParameter("time", std::min(1.f, m_timerFall / (m_timerFallMax * 3.f)));

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
				m_timerBlinkShader = sf::Time::Zero;
				m_blinkShaderState = true;
			}
		}
		PostEffectLayer::getInstance().getShader(RED_ALARM_FRAG).setParameter("transition", (m_timerBlinkShader.asSeconds() / 1.0f) * 0.75f);
	}
}

void	IntroScreen::updateCamera(sf::Time frameTime)
{
	octo::Camera &	camera = octo::Application::getCamera();
	float			ratio = m_timerCamera / m_timerCameraMax;
	sf::Vector2f	movement(std::cos(ratio * octo::Pi2) * 25.f, std::sin(ratio * octo::Pi2) * 55.f);
	float			offsetXSpeed =  100.f + m_cloudCollidedCount * 60.f;
	if (m_cloudCollidedCount < m_cloudCollidedCountMax)
		offsetXSpeed = octo::linearInterpolation(100.f + (m_cloudCollidedCount - 1.f) * 60.f, 100.f + m_cloudCollidedCount * 60.f, std::max(1.f, m_timerFall / (m_timerFallMax / 2.f)));
	sf::Vector2f	cameraOffset = sf::Vector2f(camera.getRectangle().width / 2.f + offsetXSpeed, m_spaceShip.getSize().y / 2.f);
	sf::Vector2f	shakeOffset = computeShake(frameTime);
	sf::Vector2f	cameraCenter = shakeOffset + movement;

	m_timerCamera += frameTime;
	if (m_state == Fly || m_state == Crash)
		cameraCenter += m_spaceShip.getPosition() + cameraOffset;
	else
	{
		sf::Vector2f const & cameraShipPosition = m_spaceShip.getPosition() + cameraOffset;
		sf::Vector2f const & cameraCrashPosition = sf::Vector2f(m_spaceShip.getPosition().x, m_cloudMaxY) + cameraOffset;

		cameraCenter += octo::linearInterpolation(cameraShipPosition, cameraCrashPosition, 0.7f);
	}

	camera.setCenter(cameraCenter);
}

sf::Vector2f	IntroScreen::computeShake(sf::Time frameTime)
{
	if (m_shakeTimer < m_shakeDuration)
	{
		m_shakeTimer += frameTime;
		m_waveTimer += frameTime;
		if (m_waveTimer >= m_waveDuration)
		{
			m_prevShakeOffset = m_shakeOffset;
			m_shakeOffset.x = m_noise.perlin(m_shakeTimer.asSeconds(), 0.f, 3, 2.f);
			m_shakeOffset.y = m_noise.perlin(0.f, m_shakeTimer.asSeconds(), 3, 2.f);
			octo::normalize(m_shakeOffset);
			m_shakeOffset *= m_shakeIntensity;
			m_waveTimer -= m_waveDuration;
		}
		if (m_shakeTimer >= m_shakeDuration)
		{
			m_prevShakeOffset = octo::linearInterpolation(m_prevShakeOffset, m_shakeOffset, std::min(1.f, m_waveTimer/ m_waveDuration));
			m_shakeOffset = sf::Vector2f(0.f, 0.f);
			m_waveTimer = sf::Time::Zero;
		}
	}
	else
	{
		if (m_waveTimer < m_waveDuration)
			m_waveTimer += frameTime;
	}

	return octo::linearInterpolation(m_prevShakeOffset, m_shakeOffset, std::min(1.f, m_waveTimer / m_waveDuration));
}

void IntroScreen::shake(float duration, float intensity, float waveDuration)
{
	m_shakeTimer = sf::Time::Zero;
	m_shakeDuration = sf::seconds(duration);
	m_shakeIntensity = intensity;
	m_waveDuration = sf::seconds(waveDuration);
	m_waveTimer = m_waveDuration;
}

void	IntroScreen::updateScene(sf::Time frameTime)
{
	updateSpaceShip(frameTime);
	updateShaders(frameTime);
	updateCamera(frameTime);
	m_musicPlayer.update(frameTime, m_spaceShip.getPosition());
	m_skyCycle->update(frameTime, m_biomeManager.getCurrentBiome());
	m_parallaxScrolling->update(frameTime.asSeconds());
	m_skyManager->update(frameTime);
	for (std::size_t i = 0; i < m_starsCount; i++)
		m_stars[i].update(frameTime);
}

void	IntroScreen::drawScene(sf::RenderTarget& render) const
{
	sf::RenderStates states;
	render.clear();
	render.draw(m_skyManager->getDecorsBack(), states);
	render.draw(*m_parallaxScrolling, states);
	for (std::size_t i = 0; i < m_starsCount; i++)
		m_stars[i].draw(render);
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
