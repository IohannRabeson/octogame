#include "CameraMovement.hpp"
#include "CharacterOcto.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>

CameraMovement::CameraMovement(void) :
	m_behavior(Behavior::FollowOcto),
	m_speed(4.f),
	m_maxSpeed(5.f),
	m_transition(0.f)
{}

void CameraMovement::update(sf::Time frametime, CharacterOcto & octo)
{
	octo::Camera & camera = octo::Application::getCamera();

	if (octo.isFalling())
		m_behavior = Behavior::OctoFalling;

	if (octo.isRaising())
		m_behavior = Behavior::OctoRaising;

	sf::Vector2f goalTop = octo.getPosition();
	goalTop.y -= camera.getRectangle().height / 5.f;
	sf::Vector2f goalBot = octo.getPosition();
	goalBot.y += camera.getRectangle().height / 3.f;

	switch (m_behavior)
	{
		case Behavior::FollowOcto:
		{
			m_transition -= 0.3f * frametime.asSeconds();
			if (m_transition < 0.f)
				m_transition = 0.f;
			break;
		}
		case Behavior::OctoFalling:
		{
			m_transition += 0.5f * frametime.asSeconds();
			if (m_transition > 1.f)
				m_transition = 1.f;
			break;
		}
		case Behavior::OctoRaising:
		{
			m_transition -= frametime.asSeconds();
			if (m_transition < 0.f)
				m_transition = 0.f;
			break;
		}
		default:
			break;
	}
	sf::Vector2f goal = octo::linearInterpolation(goalTop, goalBot, m_transition);
	float t = (std::abs(goal.y - camera.getCenter().y) / (camera.getRectangle().height / 2.f));
	m_speed = octo::linearInterpolation(1.f, m_maxSpeed, std::min(t, 1.f));
	camera.setCenter(octo::linearInterpolation(camera.getCenter(), goal, m_speed * frametime.asSeconds()));

	m_behavior = FollowOcto;
	m_circle.setPosition(goal);
	m_circle.setFillColor(sf::Color::Red);
	m_circle.setRadius(10.f);
}

void CameraMovement::draw(sf::RenderTarget & render)
{
	render.draw(m_circle);
	m_circle.setPosition(octo::Application::getCamera().getCenter());
	m_circle.setFillColor(sf::Color::Green);
	render.draw(m_circle);
}

void CameraMovement::collideZoom(sf::Time)
{}

void CameraMovement::collideZoomEvent(sf::Vector2f const &)
{}

void CameraMovement::setEventFallTimer(sf::Time const &)
{}
