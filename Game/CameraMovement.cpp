#include "CameraMovement.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>

CameraMovement::CameraMovement(void):
	m_zoomFactor(0.85f),
	m_isZoom(false),
	m_isFall(false),
	m_timerZoom(sf::Time::Zero),
	m_timerZoomMax(sf::seconds(2.f)),
	m_timerUnZoom(sf::Time::Zero),
	m_timerUnZoomMax(sf::seconds(2.f)),
	m_timerBeforeChangeMax(sf::seconds(3.f)),
	m_timerChangeLookAtMax(sf::seconds(3.f))
{
	m_initialSize = sf::Vector2f(octo::Application::getGraphicsManager().getVideoMode().width, octo::Application::getGraphicsManager().getVideoMode().height);
	octo::Application::getCamera().setSize(m_initialSize);
}

void	CameraMovement::follow(sf::Time frameTime, sf::Vector2f const & octoPos)
{
	float frameTimeSeconds = frameTime.asSeconds();
	octo::Camera & camera = octo::Application::getCamera();
	sf::Vector2f const & cameraSize = camera.getSize();
	sf::Vector2f cameraPos = camera.getCenter();
	float lookAtUpPosY = octoPos.y;
	float lookAtDownPosY = octoPos.y;

	if (m_isFall)
		m_timerBeforeChange += frameTime;
	else
		m_timerBeforeChange = sf::Time::Zero;

	if (m_isFall && m_timerChangeLookAt <= m_timerChangeLookAtMax && m_timerBeforeChange >= m_timerBeforeChangeMax)
		m_timerChangeLookAt += frameTime;
	else if (m_timerChangeLookAt > sf::Time::Zero)
		m_timerChangeLookAt -= frameTime / 2.f;
	else
		m_timerChangeLookAt = sf::Time::Zero;

	float coefChangeLookAt = m_timerChangeLookAt / m_timerChangeLookAtMax;
	lookAtDownPosY += -cameraSize.y / 4.f + (cameraSize.y / 4.f) * coefChangeLookAt * 3.f;

	cameraPos.x = octo::linearInterpolation(octoPos.x, cameraPos.x, 1.f - frameTimeSeconds);
	if (lookAtDownPosY >= cameraPos.y)
		cameraPos.y = octo::linearInterpolation(lookAtDownPosY, cameraPos.y, 1.f - frameTimeSeconds * 6.f);
	if (lookAtUpPosY <= cameraPos.y)
		cameraPos.y = octo::linearInterpolation(lookAtUpPosY, cameraPos.y, 1.f - frameTimeSeconds * 4.f);

	if (m_isZoom)
	{
		collideZoom(frameTime);
		if (m_timerZoom < m_timerZoomMax * 2.f)
		{
			lookAtUpPosY = m_npcPos.y;
			lookAtDownPosY = m_npcPos.y;
			lookAtDownPosY -= cameraSize.y / 4.f;
			cameraPos.x = octo::cosinusInterpolation(m_npcPos.x, cameraPos.x, 1.f - frameTimeSeconds * 3.f);
			if (lookAtDownPosY >= cameraPos.y)
				cameraPos.y = octo::cosinusInterpolation(lookAtDownPosY, cameraPos.y, 1.f - frameTimeSeconds * 8.f);
//			else if (lookAtUpPosY <= cameraPos.y)
//			 	cameraPos.y = octo::cosinusInterpolation(lookAtUpPosY, cameraPos.y, 1.f - frameTimeSeconds * 6.f);
		}
	}
	camera.setCenter(cameraPos);
}

void CameraMovement::collideZoom(sf::Time frametime)
{
	octo::Camera & camera = octo::Application::getCamera();

	if (m_timerZoom < m_timerZoomMax)
	{
		if (m_timerZoom == sf::Time::Zero)
			m_currentSize = camera.getSize();
		m_timerZoom += frametime;
		camera.setSize(octo::cosinusInterpolation(m_currentSize, m_initialSize * m_zoomFactor, m_timerZoom / m_timerZoomMax));
	}
	else if (m_timerZoom >= m_timerZoomMax && m_timerZoom < m_timerZoomMax * 2.f)
		m_timerZoom += frametime;
	else if (m_timerUnZoom < m_timerUnZoomMax)
	{
		if (m_timerUnZoom == sf::Time::Zero)
			m_currentSize = camera.getSize();
		m_timerUnZoom += frametime;
		camera.setSize(octo::cosinusInterpolation(m_currentSize, m_initialSize, m_timerUnZoom / m_timerUnZoomMax));
	}
	else
		m_isZoom = false;
}

void CameraMovement::collideZoomEvent(sf::Vector2f const & npcPos)
{
	m_isZoom = true;
	if (npcPos != m_npcPos)
	{
		m_timerZoom = sf::Time::Zero;
		m_timerUnZoom = sf::Time::Zero;
		m_npcPos = npcPos;
	}
}

void CameraMovement::setEventFallTimer(sf::Time const & eventFallTimer)
{
	m_eventFallTimer = eventFallTimer;
}

void CameraMovement::setFall(bool isFall)
{
	m_isFall = isFall;
}
