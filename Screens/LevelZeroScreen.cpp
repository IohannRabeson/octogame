#include "LevelZeroScreen.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>

LevelZeroScreen::LevelZeroScreen(void) :
	m_spaceShip(SpaceShip::Flying),
	m_state(Flying),
	m_offsetCamera(0.f),
	m_keyUp(false),
	m_keyDown(false)
{}

void	LevelZeroScreen::start()
{
	octo::GraphicsManager &		graphics = octo::Application::getGraphicsManager();

	m_timer = sf::Time::Zero;
	m_timerMax = sf::seconds(4.f);
	m_timerEnd = sf::Time::Zero;
	m_timerEndMax = sf::seconds(6.f);
	m_spaceShip.setPosition(sf::Vector2f(0.f, 0.f));
	m_stars.canEmit(true);

	graphics.addKeyboardListener(this);
}

void	LevelZeroScreen::pause()
{
}

void	LevelZeroScreen::resume()
{
}

void	LevelZeroScreen::stop()
{
	octo::GraphicsManager &	graphics = octo::Application::getGraphicsManager();
	graphics.removeKeyboardListener(this);
}

void	LevelZeroScreen::update(sf::Time frameTime)
{
	m_timer += frameTime;
	m_timerEnd += frameTime;

	if (m_timer >= m_timerMax)
		m_timer -= m_timerMax;

	sf::Vector2f translation(0.f, 0.f);
	if (m_keyUp)
		translation.y = -300.f * frameTime.asSeconds();
	else if (m_keyDown)
		translation.y = 300.f * frameTime.asSeconds();

	if (m_state == Flying)
	{
		if (m_timerEnd >= m_timerEndMax * 2.f)
		{
			m_timerEnd = sf::Time::Zero;
			m_state = Falling;
		}
	}
	else if (m_state == Falling)
	{
		if (m_timerEnd >= m_timerEndMax)
		{
			octo::StateManager & states = octo::Application::getStateManager();
			states.push("transitionLevelZero");
		}
		m_offsetCamera = 150.f * frameTime.asSeconds();
		translation.y = 300.f * frameTime.asSeconds();
	}

	m_spaceShip.move(translation);
	m_spaceShip.update(frameTime);
	m_stars.update(frameTime);
	octo::Camera & camera = octo::Application::getCamera();
	float ratio = m_timer / m_timerMax;
	sf::Vector2f movement(std::cos(ratio * octo::Pi2) * 15.f, std::sin(ratio * octo::Pi2) * 35.f);
	camera.setCenter(movement + m_spaceShip.getPosition() + sf::Vector2f(camera.getRectangle().width / 2.f + 300.f, m_spaceShip.getSize().y / 2.f + m_offsetCamera));
}

void	LevelZeroScreen::draw(sf::RenderTarget & render) const
{
	sf::RenderStates states;
	render.clear(sf::Color::Black);
	m_stars.draw(render);
	m_spaceShip.drawFront(render, states);
}

bool	LevelZeroScreen::onPressed(sf::Event::KeyEvent const & event)
{
	switch (event.code)
	{
		case sf::Keyboard::Up:
			m_keyUp = true;
			break;
		case sf::Keyboard::Down:
			m_keyDown = true;
			break;
		default:
			break;
	}
	return true;
}

bool	LevelZeroScreen::onReleased(sf::Event::KeyEvent const& event)
{
	switch (event.code)
	{
		case sf::Keyboard::Up:
			m_keyUp = false;
			break;
		case sf::Keyboard::Down:
			m_keyDown = false;
			break;
		default:
			break;
	}
	return true;
}
