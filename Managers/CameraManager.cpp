#include "CameraManager.hpp"
#include "MapManager.hpp"

CameraManager::CameraManager(void) :
	m_mapManager(nullptr),
	m_position(),
	m_camera(octo::Application::getCamera())
{ }

CameraManager::~CameraManager(void) { }

void CameraManager::init(MapManager * mapManager)
{
	m_mapManager = mapManager;
}

void CameraManager::update(float)
{
	m_position.x += m_mapManager->getPlayer().getOldVelocity().x;
	m_position.y = 300.f;
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	//	m_position.x -= speed;
}
