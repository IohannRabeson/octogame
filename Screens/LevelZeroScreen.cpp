#include "LevelZeroScreen.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <AudioManager.hpp>
#include <GraphicsManager.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Interpolations.hpp>

LevelZeroScreen::LevelZeroScreen(void) :
	m_spaceShip(SpaceShip::Flying),
	m_starsCount(15u),
	m_stars(new StarSystem[m_starsCount]),
	m_background(sf::Quads, 4),
	m_upColorBackground(sf::Color::Black),
	m_downColorBackground(sf::Color(8, 20, 26)),
	m_state(Flying),
	m_offsetCamera(0.f),
	m_keyUp(false),
	m_keyDown(false)
{}

void	LevelZeroScreen::start()
{
	octo::GraphicsManager &		graphics = octo::Application::getGraphicsManager();
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resource = octo::Application::getResourceManager();

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

	audio.startMusic(resource.getSound(ACTION_FAST_WAV), sf::milliseconds(1000.f));
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
	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::Camera &				camera = octo::Application::getCamera();
	sf::FloatRect const &		cameraRect = camera.getRectangle();

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
		else
		{
			sf::Color color = octo::linearInterpolation(sf::Color::Black, m_downColorBackground, m_timerEnd / m_timerEndMax / 2.f);
			createBackground(sf::Vector2f(cameraRect.left, cameraRect.top), color);
		}
	}
	else if (m_state == Falling)
	{
		audio.stopMusic(m_timerEndMax);
		if (m_timerEnd >= m_timerEndMax)
		{
			octo::StateManager & states = octo::Application::getStateManager();
			states.pop();
		}
		else
		{
			sf::Color color = octo::linearInterpolation(m_downColorBackground, sf::Color::White, m_timerEnd / m_timerEndMax / 1.5f);
			createBackground(sf::Vector2f(cameraRect.left, cameraRect.top), color);
		}
		m_offsetCamera = 150.f * frameTime.asSeconds();
		translation.y = 300.f * frameTime.asSeconds();
	}

	m_spaceShip.move(translation);
	m_spaceShip.update(frameTime);
	for (std::size_t i = 0; i < m_starsCount; i++)
		m_stars[i].update(frameTime);
	float ratio = m_timer / m_timerMax;
	sf::Vector2f movement(std::cos(ratio * octo::Pi2) * 15.f, std::sin(ratio * octo::Pi2) * 35.f);
	camera.setCenter(movement + m_spaceShip.getPosition() + sf::Vector2f(camera.getRectangle().width / 2.f + 300.f, m_spaceShip.getSize().y / 2.f + m_offsetCamera));
}

void	LevelZeroScreen::createBackground(sf::Vector2f const & position, sf::Color const & color)
{
	sf::Vector2f cameraSize = octo::Application::getCamera().getSize();
	m_background[0] = sf::Vertex(sf::Vector2f(0.f, 0.f) + position, m_upColorBackground);
	m_background[1] = sf::Vertex(sf::Vector2f(cameraSize.x, 0.f) + position, m_upColorBackground);
	m_background[2] = sf::Vertex(sf::Vector2f(cameraSize.x, cameraSize.y * 1.5f) + position, color);
	m_background[3] = sf::Vertex(sf::Vector2f(0.f, cameraSize.y * 1.5f) + position, color);
}

void	LevelZeroScreen::draw(sf::RenderTarget & render) const
{
	sf::RenderStates states;
	render.clear(sf::Color::Black);
	render.draw(m_background, states);
	for (std::size_t i = 0; i < m_starsCount; i++)
		m_stars[i].draw(render);
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
