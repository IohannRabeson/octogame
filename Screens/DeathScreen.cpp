#include "DeathScreen.hpp"

#include <Application.hpp>
#include <AudioManager.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>

#include "ResourceDefinitions.hpp"
#include "Progress.hpp"

DeathScreen::DeathScreen() :
	m_timeDeath(sf::Time::Zero),
	m_timeDeathMax(sf::seconds(1.25f))
{
	octo::SpriteAnimation::FrameList	frames;

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
	octo::AudioManager &	audio = octo::Application::getAudioManager();
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
	audio.playSound(resources.getSound(PORTAL_START_OGG), 0.7f);
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
}

void	DeathScreen::update(sf::Time frameTime)
{
	octo::StateManager & states = octo::Application::getStateManager();
	m_timeDeath += frameTime;
	if (m_timeDeath >= m_timeDeathMax)
		states.change("game");
	m_sprite.update(frameTime);
}

void	DeathScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	if (m_timeDeath < m_timeDeathMax)
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
		m_timeDeath = m_timeDeathMax;
	return true;
}
