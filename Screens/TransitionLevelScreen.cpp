#include "TransitionLevelScreen.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>

#include "ResourceDefinitions.hpp"
#include "Progress.hpp"

TransitionLevelScreen::TransitionLevelScreen() :
	m_sound(nullptr),
	m_timeTransition(sf::Time::Zero),
	m_timeTransitionMax(sf::seconds(0.8f))
{
	octo::SpriteAnimation::FrameList	frames;

	frames.emplace_back(sf::seconds(0.1f), 6);
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
	m_sound = audio.playSound(resources.getSound(PORTAL_START_OGG), 0.7f);
	std::cout << "Transition level screen start" << std::endl;
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
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	if (m_sound != nullptr)
		m_sound->stop();
	audio.playSound(resources.getSound(PORTAL_END_OGG));
	std::cout << "Transition level screen stop" << std::endl;
}

void	TransitionLevelScreen::update(sf::Time frameTime)
{
	octo::StateManager & states = octo::Application::getStateManager();
	m_timeTransition += frameTime;
	if (m_timeTransition  > m_timeTransitionMax)
		states.change("game");
	m_sprite.update(frameTime);
}

void	TransitionLevelScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	render.draw(m_sprite);
}

