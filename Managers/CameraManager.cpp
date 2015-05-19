#include "CameraManager.hpp"
#include "MapManager.hpp"
#include <GraphicsManager.hpp>

CameraManager::CameraManager(void) :
	m_mapManager(nullptr),
	m_camera(octo::Application::getCamera())
{ }

CameraManager::~CameraManager(void) { }

void CameraManager::init(MapManager * mapManager)
{
	m_mapManager = mapManager;
}

#include <iostream>
void CameraManager::update(float pf_deltatime)
{
	//m_position.x += m_mapManager->getPlayer().getOldVelocity().x;
	//m_position.y = 300.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_camera.move(300.f * pf_deltatime, 0.f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_camera.move(-300.f * pf_deltatime, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_camera.move(0.f, 300.f * pf_deltatime);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_camera.move(0.f, -300.f * pf_deltatime);
	//std::cout << m_camera.getCenter().x << " " << m_camera.getCenter().y  << std::endl;
}

sf::Vector2f CameraManager::getUpLeft(void) const
{
	return sf::Vector2f(m_camera.getCenter().x - octo::Application::getGraphicsManager().getVideoMode().width / 2.f,
				m_camera.getCenter().y - octo::Application::getGraphicsManager().getVideoMode().height/ 2.f);
}
