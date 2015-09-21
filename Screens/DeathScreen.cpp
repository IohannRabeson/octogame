#include "DeathScreen.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Console.hpp>

#include "ResourceDefinitions.hpp"
#include "Progress.hpp"

DeathScreen::DeathScreen() :
	m_timeDeath(sf::Time::Zero),
	m_timeDeathMax(sf::seconds(3.5f))
{
	octo::SpriteAnimation::FrameList	frames;

	frames.emplace_back(sf::seconds(1.f), 0);
	frames.emplace_back(sf::seconds(0.2f), 1);
	frames.emplace_back(sf::seconds(0.2f), 2);
	frames.emplace_back(sf::seconds(0.2f), 3);
	frames.emplace_back(sf::seconds(0.2f), 4);
	frames.emplace_back(sf::seconds(0.2f), 5);
	frames.emplace_back(sf::seconds(0.2f), 6);
	frames.emplace_back(sf::seconds(0.2f), 7);
	frames.emplace_back(sf::seconds(0.2f), 8);
	frames.emplace_back(sf::seconds(0.2f), 9);
	frames.emplace_back(sf::seconds(0.2f), 10);
	frames.emplace_back(sf::seconds(0.2f), 11);
	frames.emplace_back(sf::seconds(0.2f), 12);
	frames.emplace_back(sf::seconds(0.2f), 13);
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
	sf::Vector2f const&		pos = Progress::getInstance().getOctoPos();
	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_DEATH_OSS));
	m_sprite.setPosition(pos.x, pos.y - m_sprite.getLocalSize().y);
	m_sprite.setAnimation(m_animation);
	m_sprite.setScale(0.6f, 0.6f);
	m_sprite.play();
	camera.setCenter(m_sprite.getGlobalBounds().left + m_sprite.getGlobalBounds().width / 2,
					 m_sprite.getGlobalBounds().top + m_sprite.getGlobalBounds().height / 2);
}

void	DeathScreen::pause()
{

}

void	DeathScreen::resume()
{

}

void	DeathScreen::stop()
{

}

void	DeathScreen::update(sf::Time frameTime)
{
	octo::StateManager & states = octo::Application::getStateManager();
	m_timeDeath += frameTime;
	if (m_timeDeath > m_timeDeathMax)
		states.push("game");
	m_sprite.update(frameTime);
}

void	DeathScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
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

bool	DeathScreen::onPressed(sf::Event::KeyEvent const&)
{
	return (true);
}
