#include "LevelRocketEndScreen.hpp"
#include "RocketEnd.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>
#include <PostEffectManager.hpp>

LevelRocketEndScreen::LevelRocketEndScreen(void) :
	m_starsCount(15u),
	m_stars(new StarSystem[m_starsCount]),
	m_rocket(new RocketEnd(sf::Vector2f(600.f, octo::Application::getCamera().getRectangle().height + 100.f))),
	m_credit(new Credit(sf::Vector2f(1100.f, octo::Application::getCamera().getRectangle().height + 100.f))),
	m_speedCredit(1.f),
	m_timerBeforeEnd(sf::seconds(3.f))
{
	m_generator.setSeed("random");
}

void	LevelRocketEndScreen::start()
{
	InputListener::addInputListener();

	for (std::size_t i = 0; i < m_starsCount; i++)
	{
		float speed = m_generator.randomFloat(1000.f, 2000.f);
		m_stars[i].setup(sf::Vector2f(2.f, 2.f));
		m_stars[i].setEmitTimeRange(0.04f, 0.1f);
		m_stars[i].canEmit(true);
		m_stars[i].setSpeed(sf::Vector2f(0.f, speed));
		m_stars[i].setOrigin(sf::Vector2f(-20.f, octo::Application::getCamera().getRectangle().top));
		m_stars[i].isFromTop(true);
	}

	octo::Camera & camera = octo::Application::getCamera();
	camera.setCenter(camera.getRectangle().width / 2.f, camera.getRectangle().height / 2.f);

	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resource = octo::Application::getResourceManager();
	audio.startMusic(resource.getSound(MUSIC_MENU_OPUS_II_REVERSE_OGG), sf::seconds(1.f));
}

void	LevelRocketEndScreen::pause()
{
}

void	LevelRocketEndScreen::resume()
{
}

void	LevelRocketEndScreen::stop()
{
	InputListener::removeInputListener();
}

void	LevelRocketEndScreen::update(sf::Time frameTime)
{
	for (std::size_t i = 0; i < m_starsCount; i++)
		m_stars[i].update(frameTime);
	m_rocket->update(frameTime);
	if (m_rocket->getPosition().y >= 0.f)
		m_rocket->setOriginPosition(m_rocket->getPosition() + sf::Vector2f(0.f, -150.f) * frameTime.asSeconds() * m_speedCredit);
	m_credit->update(frameTime * m_speedCredit);
	if (m_credit->isFinished())
	{
		m_timerBeforeEnd -= frameTime;
		if (m_timerBeforeEnd <= sf::Time::Zero)
			octo::Application::getStateManager().change("menu");
	}
}

void	LevelRocketEndScreen::draw(sf::RenderTarget & render) const
{
	sf::RenderStates states;
	render.clear(sf::Color::Black);
	for (std::size_t i = 0; i < m_starsCount; i++)
		m_stars[i].draw(render);
	m_rocket->drawFront(render, states);
	m_credit->draw(render, states);
}

bool	LevelRocketEndScreen::onInputPressed(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Capacity:
		case OctoKeys::Elevator:
			m_speedCredit = 2.f;
			break;
		default:
			break;
	}
	return true;
}

bool	LevelRocketEndScreen::onInputReleased(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Capacity:
		case OctoKeys::Elevator:
			m_speedCredit = 1.f;
			break;
		default:
			break;
	}
	return true;
}
