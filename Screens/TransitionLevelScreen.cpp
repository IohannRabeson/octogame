#include "TransitionLevelScreen.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>

#include "ResourceDefinitions.hpp"
#include "Progress.hpp"

TransitionLevelScreen::TransitionLevelScreen() :
	m_sound(nullptr),
	m_timeTransition(sf::Time::Zero),
	m_timeTransitionMax(sf::seconds(0.8f))
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::SpriteAnimation::FrameList	frames;

	m_volumeDefault = audio.getSoundVolume();

	frames.emplace_back(sf::seconds(0.1f), 7);
	frames.emplace_back(sf::seconds(0.1f), 8);
	frames.emplace_back(sf::seconds(0.1f), 9);
	frames.emplace_back(sf::seconds(0.1f), 10);
	frames.emplace_back(sf::seconds(0.1f), 11);
	frames.emplace_back(sf::seconds(0.1f), 12);
	frames.emplace_back(sf::seconds(0.1f), 13);
	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::NoLoop);
}

TransitionLevelScreen::~TransitionLevelScreen()
{
}

void	TransitionLevelScreen::start()
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	octo::Camera&				camera = octo::Application::getCamera();
	sf::Vector2f const&			pos = Progress::getInstance().getOctoPosTransition();
	sf::Vector2f const&			cameraPos = sf::Vector2f(camera.getRectangle().left, camera.getRectangle().top);
	sf::Vector2f				scale = sf::Vector2f(0.6f, 0.6f);
	Progress const &			progress = Progress::getInstance();
	Level						current = progress.getCurrentDestination();
	Level						next = progress.getNextDestination();

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

	if (!(current == Level::EndRocket || next == Level::EndRocket) && !(current == Level::Blue || next == Level::Blue) && !(current == Level::Red || next == Level::Red))
		m_sound = audio.playSound(resources.getSound(OCTO_SOUND_DOUBLE_JUMP_OGG), 1.f);
}

void	TransitionLevelScreen::pause()
{

}

void	TransitionLevelScreen::resume()
{

}

void	TransitionLevelScreen::stop()
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	audio.setSoundVolume(m_volumeDefault);
}

void	TransitionLevelScreen::update(sf::Time frameTime)
{
	octo::AudioManager &		audio = octo::Application::getAudioManager();

	m_timeTransition += frameTime;
	if (m_timeTransition > m_timeTransitionMax)
		changeState();
	else
	{
		audio.setSoundVolume(octo::linearInterpolation(m_volumeDefault, 0.f, std::pow(m_timeTransition / m_timeTransitionMax, 2)));
		if (m_sound)
			m_sound->setVolume(0.5f * m_volumeDefault);
		m_sprite.update(frameTime);
	}
}

void	TransitionLevelScreen::changeState(void)
{
	Progress const & progress = Progress::getInstance();
	octo::StateManager &		states = octo::Application::getStateManager();

	Level current = progress.getCurrentDestination();
	Level next = progress.getNextDestination();

	if (current == Level::EndRocket || next == Level::EndRocket)
	{
		states.setTransitionDuration(sf::seconds(0.0f), sf::seconds(2.5f));
		states.change("game", "blue");
	}
	else if (current == Level::Blue || next == Level::Blue)
	{
		if (next != Level::Blue)
			states.setTransitionDuration(sf::seconds(0.0f), sf::seconds(2.5f));
		else
			states.setTransitionDuration(sf::seconds(0.0f), sf::seconds(0.0f));
		states.change("game", "blue");
	}
	else if (current == Level::Red || next == Level::Red)
	{
		if (next != Level::Red)
			states.setTransitionDuration(sf::seconds(0.0f), sf::seconds(2.5f));
		else
			states.setTransitionDuration(sf::seconds(0.0f), sf::seconds(0.0f));
		states.change("game", "red");
	}
	else
	{
		states.setTransitionDuration(sf::seconds(0.5f), sf::seconds(0.5f));
		states.change("game");
	}
}

void	TransitionLevelScreen::draw(sf::RenderTarget& render)const
{
	Progress const & progress = Progress::getInstance();
	Level current = progress.getCurrentDestination();
	Level next = progress.getNextDestination();

	if (current == Level::EndRocket || next == Level::EndRocket)
		render.clear(sf::Color(0, 0, 155));
	else if (current == Level::Blue || next == Level::Blue)
		render.clear(sf::Color(0, 0, 155));
	else if (current == Level::Red || next == Level::Red)
		render.clear(sf::Color(155, 0, 0));
	else
	{
		render.clear();
		if (m_timeTransition <= m_timeTransitionMax)
			render.draw(m_sprite);
	}
}

