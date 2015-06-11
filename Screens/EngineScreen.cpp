#include "EngineScreen.hpp"
#include "RectangleShape.hpp"
#include "ConvexShape.hpp"
#include "Circle.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>

void	EngineScreen::start()
{
	octo::Application::getGraphicsManager().setFramerateLimit(60);

	m_shape = new RectangleShape();
	m_shape->setApplyGravity(false);
	m_shape->setCollisionType(Type::e_player);
	m_shape->setCollisionMask(Type::e_player | Type::e_npc);
	m_shape->setOrigin(sf::Vector2f(100.f, 50.f));
	m_shape->setPosition(sf::Vector2f(200.f, 300.f));
	m_shape->setSize(sf::Vector2f(200.f, 300.f));

	m_shapeA = new ConvexShape();
	m_shapeA->setApplyGravity(false);
	m_shapeA->setCollisionType(Type::e_player);
	m_shapeA->setCollisionMask(Type::e_player | Type::e_npc);
	m_shapeA->setVertexCount(5u);
	m_shapeA->setOrigin(sf::Vector2f(100.f, 50.f));
	m_shapeA->setPosition(sf::Vector2f(500.f, 300.f));
	m_shapeA->setVertex(0u, sf::Vector2f(0.f, 0.f));
	m_shapeA->setVertex(1u, sf::Vector2f(150.f, 0.f));
	m_shapeA->setVertex(2u, sf::Vector2f(200.f, 120.f));
	m_shapeA->setVertex(3u, sf::Vector2f(150.f, 200.f));
	m_shapeA->setVertex(4u, sf::Vector2f(0.f, 200.f));

	m_shapeB = new ConvexShape();
	m_shapeB->setCollisionType(Type::e_player);
	m_shapeB->setCollisionMask(Type::e_player | Type::e_npc);
	m_shapeB->setVertexCount(4u);
	m_shapeB->setOrigin(sf::Vector2f(100.f, 50.f));
	m_shapeB->setPosition(sf::Vector2f(500.f, 300.f));
	m_shapeB->setVertex(0u, sf::Vector2f(0.f, 0.f));
	m_shapeB->setVertex(1u, sf::Vector2f(50.f, 0.f));
	m_shapeB->setVertex(2u, sf::Vector2f(150.f, 200.f));
	m_shapeB->setVertex(3u, sf::Vector2f(0.f, 200.f));

	m_ground = new ConvexShape();
	m_ground->setType(AShape::Type::e_static);
	m_ground->setApplyGravity(false);
	m_ground->setCollisionType(Type::e_player);
	m_ground->setCollisionMask(Type::e_player | Type::e_npc);
	m_ground->setVertexCount(4u);
	m_ground->setPosition(sf::Vector2f(10.f, 900.f));
	m_ground->setVertex(0u, sf::Vector2f(0.f, 0.f));
	m_ground->setVertex(1u, sf::Vector2f(1910.f, 0.f));
	m_ground->setVertex(2u, sf::Vector2f(1910.f, 50.f));
	m_ground->setVertex(3u, sf::Vector2f(0.f, 50.f));
/*
	m_circle = new ConvexShape();
	m_circle->setCollisionType(Type::e_rock);
	m_circle->setCollisionMask(Type::e_player | Type::e_npc);
	m_circle->setPosition(400.f, 450.f);
	m_circle->setRadius(65.f);
	m_circle->setOrigin(sf::Vector2f(10.f, 16.f));
*/
	m_engine.init();
	//m_engine.registerShape(m_circle);
	m_engine.registerShape(m_shape);
	m_engine.registerShape(m_shapeA);
	m_engine.registerShape(m_shapeB);
	m_engine.registerShape(m_ground);
}

void	EngineScreen::pause()
{
}

void	EngineScreen::resume()
{
}

void	EngineScreen::stop()
{
}

void	EngineScreen::update(sf::Time deltatime)
{
	float speed = 1250.f * deltatime.asSeconds();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
		m_shapeA->addVelocity(0.f, -speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		m_shapeA->addVelocity(0.f, speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		m_shapeA->addVelocity(speed, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		m_shapeA->addVelocity(-speed, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		m_shapeA->rotate(3.14f * deltatime.asSeconds());
	m_engine.update(deltatime.asSeconds());
}

void	EngineScreen::draw(sf::RenderTarget & render) const
{
	render.clear(sf::Color::Black);
	m_engine.debugDraw(render);
	//m_circle->debugDraw(render);
}
