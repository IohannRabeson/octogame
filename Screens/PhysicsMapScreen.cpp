#include "PhysicsMapScreen.hpp"
#include "ConvexShape.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>

PhysicsMapScreen::PhysicsMapScreen(void) :
	m_engine(PhysicsEngine::getInstance())
{}

void	PhysicsMapScreen::start()
{
	octo::Application::getGraphicsManager().setFramerateLimit(60);
	m_mapManager.init();
	m_shape = m_engine.createConvex();
	m_shape->setPosition(300.f, 300.f);
	m_shape->setApplyGravity(true);
	m_shape->setVertexCount(4u);
	m_shape->setOrigin(sf::Vector2f(100.f, 50.f));
	m_shape->setPosition(sf::Vector2f(500.f, 300.f));
	m_shape->setVertex(0u, sf::Vector2f(0.f, 0.f));
	m_shape->setVertex(1u, sf::Vector2f(40.f, 0.f));
	m_shape->setVertex(2u, sf::Vector2f(40.f, 60.f));
	m_shape->setVertex(3u, sf::Vector2f(0.f, 60.f));
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
	float speed = 350.f * deltatime.asSeconds();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
		m_shape->addVelocity(0.f, -speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		m_shape->addVelocity(0.f, speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		m_shape->addVelocity(speed, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		m_shape->addVelocity(-speed, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		m_shape->rotate(3.14f * deltatime.asSeconds());

	m_mapManager.update(deltatime.asSeconds());
	m_engine.update(deltatime.asSeconds());
}

void	PhysicsMapScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::Black);
	sf::RenderStates states;
	m_mapManager.draw(render, states);
	m_engine.debugDraw(render);
}
