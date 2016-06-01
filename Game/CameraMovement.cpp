#include "CameraMovement.hpp"
#include "CharacterOcto.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>

CameraMovement::CameraMovement(void) :
	m_behavior(Behavior::FollowOcto),
	m_speed(3.f),
	m_verticalTransition(0.f),
	m_horizontalTransition(0.f),
	m_horizontalAxis(0.f),
	m_verticalAxis(0.f)
{
	InputListener::addInputListener();
}

CameraMovement::~CameraMovement(void)
{
	InputListener::removeInputListener();
}

void CameraMovement::update(sf::Time frametime, CharacterOcto & octo)
{
	octo::Camera & camera = octo::Application::getCamera();

	if (octo.isRaising())
		m_behavior = Behavior::OctoRaising;
	else if (m_behavior != ControlledByPlayer)
	{
		if (octo.isFalling())
			m_behavior = Behavior::OctoFalling;
		else
			m_behavior = Behavior::FollowOcto;
	}

	float goalTop = octo.getPosition().y;
	goalTop -= camera.getRectangle().height / 5.f;
	float goalBot = octo.getPosition().y;
	goalBot += camera.getRectangle().height / 3.f;
	float goalLeft = octo.getPosition().x;
	goalLeft -= camera.getRectangle().width / 3.f;
	float goalRight = octo.getPosition().x;
	goalRight += camera.getRectangle().width / 3.f;

	switch (m_behavior)
	{
		case Behavior::FollowOcto:
		{
			m_speed = 3.f;
			m_verticalTransition -= 0.3f * frametime.asSeconds();
			if (m_verticalTransition < 0.f)
				m_verticalTransition = 0.f;
			if (m_horizontalTransition + frametime.asSeconds() < 0.f)
				m_horizontalTransition += frametime.asSeconds();
			else if (m_horizontalTransition - frametime.asSeconds() > 0.f)
				m_horizontalTransition -= frametime.asSeconds();
			break;
		}
		case Behavior::OctoFalling:
		{
			m_speed = 3.f;
			m_verticalTransition += 0.5f * frametime.asSeconds();
			if (m_verticalTransition > 1.f)
				m_verticalTransition = 1.f;
			if (m_horizontalTransition + frametime.asSeconds() < 0.f)
				m_horizontalTransition += frametime.asSeconds();
			else if (m_horizontalTransition - frametime.asSeconds() > 0.f)
				m_horizontalTransition -= frametime.asSeconds();
			break;
		}
		case Behavior::OctoRaising:
		{
			m_speed = 4.f;
			m_verticalTransition -= frametime.asSeconds();
			if (m_verticalTransition < 0.f)
				m_verticalTransition = 0.f;
			if (m_horizontalTransition + frametime.asSeconds() < 0.f)
				m_horizontalTransition += frametime.asSeconds();
			else if (m_horizontalTransition - frametime.asSeconds() > 0.f)
				m_horizontalTransition -= frametime.asSeconds();
			break;
		}
		case Behavior::ControlledByPlayer:
		{
			m_verticalTransition += m_verticalAxis * frametime.asSeconds();
			m_horizontalTransition += m_horizontalAxis * frametime.asSeconds();
			if (m_verticalTransition > 1.f)
				m_verticalTransition = 1.f;
			if (m_verticalTransition < 0.f)
				m_verticalTransition = 0.f;
			if (m_horizontalTransition > 1.f)
				m_horizontalTransition = 1.f;
			if (m_horizontalTransition < -1.f)
				m_horizontalTransition = -1.f;
			break;
		}
		default:
			break;
	}
	sf::Vector2f goal = sf::Vector2f(octo::linearInterpolation(goalRight, goalLeft, (m_horizontalTransition + 1.f) / 2.f),
						octo::linearInterpolation(goalTop, goalBot, m_verticalTransition));

	camera.setCenter(octo::linearInterpolation(camera.getCenter(), goal, m_speed * frametime.asSeconds()));

	m_circle.setPosition(goal);
	m_circle.setFillColor(sf::Color::Red);
	m_circle.setRadius(10.f);
}

void CameraMovement::debugDraw(sf::RenderTarget & render)
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

bool CameraMovement::onInputPressed(InputListener::OctoKeys const & key)
{
	if (key == InputListener::OctoKeys::ViewLeft)
	{
		m_behavior = Behavior::ControlledByPlayer;
		m_horizontalAxis = 1.f;
	}
	else if (key == InputListener::OctoKeys::ViewRight)
	{
		m_behavior = Behavior::ControlledByPlayer;
		m_horizontalAxis = -1.f;
	}

	if (key == InputListener::OctoKeys::ViewDown)
	{
		m_behavior = Behavior::ControlledByPlayer;
		m_verticalAxis = 1.f;
	}
	else if (key == InputListener::OctoKeys::ViewUp)
	{
		m_behavior = Behavior::ControlledByPlayer;
		m_verticalAxis = -1.f;
	}
	return true;
}

bool CameraMovement::onInputReleased(InputListener::OctoKeys const & key)
{
	if (key == InputListener::OctoKeys::ViewLeft || key == InputListener::OctoKeys::ViewRight)
	{
		m_horizontalAxis = 0.f;
		if (m_verticalAxis == 0.f)
			m_behavior = Behavior::FollowOcto;
	}
	if (key == InputListener::OctoKeys::ViewUp || key == InputListener::OctoKeys::ViewDown)
	{
		m_verticalAxis = 0.f;
		if (m_horizontalAxis == 0.f)
			m_behavior = Behavior::FollowOcto;
	}
	return true;
}
