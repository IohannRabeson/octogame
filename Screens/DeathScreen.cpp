#include "DeathScreen.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>

#include "ResourceDefinitions.hpp"
#include "Progress.hpp"

DeathScreen::DeathScreen() :
	m_sound(nullptr),
	m_timeTransition(sf::Time::Zero),
	m_timeTransitionMax(sf::seconds(1.25f))
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::SpriteAnimation::FrameList	frames;

	m_volumeDefault = audio.getSoundVolume();

	frames.emplace_back(sf::seconds(0.6f), 0);
	frames.emplace_back(sf::seconds(0.05f), 1);
	frames.emplace_back(sf::seconds(0.05f), 2);
	frames.emplace_back(sf::seconds(0.05f), 3);
	frames.emplace_back(sf::seconds(0.05f), 4);
	frames.emplace_back(sf::seconds(0.05f), 5);
	frames.emplace_back(sf::seconds(0.05f), 6);
	frames.emplace_back(sf::seconds(0.05f), 7);
	frames.emplace_back(sf::seconds(0.05f), 8);
	frames.emplace_back(sf::seconds(0.05f), 9);
	frames.emplace_back(sf::seconds(0.05f), 10);
	frames.emplace_back(sf::seconds(0.05f), 11);
	frames.emplace_back(sf::seconds(0.05f), 12);
	frames.emplace_back(sf::seconds(0.05f), 13);
	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::NoLoop);
}

DeathScreen::~DeathScreen()
{
}

void	DeathScreen::start()
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	octo::Camera&			camera = octo::Application::getCamera();
	sf::Vector2f const&		pos = Progress::getInstance().getOctoPosTransition();
	sf::Vector2f const&		cameraPos = sf::Vector2f(camera.getRectangle().left, camera.getRectangle().top);
	sf::Vector2f			scale = sf::Vector2f(0.6f, 0.6f);

	InputListener::addInputListener();

	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_DEATH_OSS));
	m_sprite.setAnimation(m_animation);
	m_sprite.play();
	if (Progress::getInstance().getReverseSprite())
	{
		scale.x *= -1.f;
		m_sprite.setOrigin(m_sprite.getOrigin().x + m_sprite.getLocalSize().x, 0.f);
	}
	m_sprite.setScale(scale);
	m_sprite.setPosition(pos - m_sprite.getGlobalSize() + cameraPos);
}

void	DeathScreen::pause()
{
	InputListener::removeInputListener();
}

void	DeathScreen::resume()
{
	InputListener::addInputListener();
}

void	DeathScreen::stop()
{
	InputListener::removeInputListener();
	octo::AudioManager &		audio = octo::Application::getAudioManager();

	audio.setSoundVolume(m_volumeDefault);
}

void	DeathScreen::update(sf::Time frameTime)
{
	octo::ResourceManager&		resources = octo::Application::getResourceManager();
	octo::StateManager &		states = octo::Application::getStateManager();
	octo::AudioManager &		audio = octo::Application::getAudioManager();

	m_timeTransition += frameTime;
	if (!m_sound && m_timeTransition > sf::seconds(0.75f))
		m_sound = audio.playSound(resources.getSound(DOUBLE_JUMP_TEST_OGG), 0.5f);

	if (m_timeTransition >= m_timeTransitionMax)
		states.change("game");
	else
	{
		audio.setSoundVolume(octo::linearInterpolation(m_volumeDefault, 0.f, std::pow(m_timeTransition / m_timeTransitionMax, 2)));
		if (m_sound)
			m_sound->setVolume(0.5f * m_volumeDefault);
		m_sprite.update(frameTime);
	}
}

void	DeathScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	if (m_timeTransition < m_timeTransitionMax)
		render.draw(m_sprite);
}

void	DeathScreen::setSpriteSheet(octo::SpriteSheet const& spriteSheet)
{
	m_sprite.setSpriteSheet(spriteSheet);
}

void	DeathScreen::setAnimation(octo::SpriteAnimation const& animation)
{
	m_sprite.setAnimation(animation);
}

bool	DeathScreen::onInputPressed(InputListener::OctoKeys const &)
{
	if (!Progress::getInstance().isFirstTime())
		m_timeTransition = m_timeTransitionMax;
	return true;
}
