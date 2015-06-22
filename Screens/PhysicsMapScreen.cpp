#include "PhysicsMapScreen.hpp"

PhysicsMapScreen::PhysicsMapScreen(void) :
	m_engine(PhysicsEngine::getInstance())
{}

void	PhysicsMapScreen::start()
{
	m_mapManager.init();
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
	m_mapManager.update(deltatime.asSeconds());
	m_engine.update(deltatime.asSeconds());
}

void	PhysicsMapScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::White);
	sf::RenderStates states;
	m_mapManager.draw(render, states);
	m_engine.debugDraw(render);
}
