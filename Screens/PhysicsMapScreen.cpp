#include "PhysicsMapScreen.hpp"
#include "ConvexShape.hpp"
#include "CircleShape.hpp"
#include "RectangleShape.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>

PhysicsMapScreen::PhysicsMapScreen(void) :
	m_engine(PhysicsEngine::getInstance()),
	m_camera(octo::Application::getCamera()),
	m_shape(nullptr)
{}

void	PhysicsMapScreen::start()
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);

	m_groundManager.init(m_biome);

	m_engine.setIterationCount(4u);
	m_engine.setTileCollision(true);

	m_shape = m_engine.createConvex();
	m_shape->setPosition(300.f, 600.f);
	m_shape->setApplyGravity(false);
	m_shape->setVertexCount(4u);
	m_shape->setOrigin(sf::Vector2f(10.f, 30.f));
	m_shape->setPosition(sf::Vector2f(460.f, 350.f));
	m_shape->setVertex(0u, sf::Vector2f(0.f, 0.f));
	m_shape->setVertex(1u, sf::Vector2f(40.f, 0.f));
	m_shape->setVertex(2u, sf::Vector2f(40.f, 60.f));
	m_shape->setVertex(3u, sf::Vector2f(0.f, 60.f));

	for (std::size_t i = 0u; i < 10; i++)
	{
		m_shapes[i] = m_engine.createRectangle();
		if (i == 1)
			m_shapes[i]->setApplyGravity(false);
		else
			m_shapes[i]->setApplyGravity(true);
		m_shapes[i]->setPosition(sf::Vector2f(560.f + i * 50.f, 350.f));
		m_shapes[i]->setSize(sf::Vector2f(40.f, 60.f));
	}

	RectangleShape * rect = m_engine.createRectangle();
	rect->setSize(sf::Vector2f(40.f, 60.f));
	rect->setPosition(sf::Vector2f(1500.f, 500.f));
	rect->setApplyGravity(true);

	m_engine.unregisterShape(m_shapes[5]);
}

void	PhysicsMapScreen::pause()
{
}

void	PhysicsMapScreen::resume()
{
}

void	PhysicsMapScreen::stop()
{
}

void	PhysicsMapScreen::update(sf::Time deltatime)
{
	float speed = 200.f * deltatime.asSeconds();
	float cameraSpeed = 500.f * deltatime.asSeconds();

	m_groundManager.update(deltatime.asSeconds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
		m_shape->addVelocity(0.f, -speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		m_shape->addVelocity(0.f, speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		m_shape->addVelocity(speed, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		m_shape->addVelocity(-speed, 0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_camera.move(-cameraSpeed, 0.f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_camera.move(cameraSpeed, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_camera.move(0.f, -cameraSpeed);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_camera.move(0.f, cameraSpeed);

	m_engine.update(deltatime.asSeconds());
}

bool PhysicsMapScreen::onPressed(sf::Event::KeyEvent const &)
{
	return true;
}

void	PhysicsMapScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::Black);
	render.draw(m_groundManager);
	m_engine.debugDraw(render);
}
