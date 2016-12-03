#include "CameraMovement.hpp"
#include "CharacterOcto.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>

CameraMovement::CameraMovement(void) :
	m_behavior(Behavior::FollowOcto),
	m_zoomState(ZoomState::ZoomIn),
	m_baseSize(octo::Application::getCamera().getSize()),
	m_zoomTimer(sf::seconds(3.f)),
	m_zoomTimerMax(sf::seconds(3.f)),
	m_shakeTimer(sf::Time::Zero),
	m_shakeDuration(sf::seconds(1.f)),
	m_waveDuration(sf::seconds(1.f)),
	m_shakeIntensity(0.f),
	m_speed(3.f),
	m_verticalTransition(0.f),
	m_horizontalTransition(0.f),
	m_horizontalAxis(0.f),
	m_verticalAxis(0.f),
	m_blockAxisY(false)
{
	m_circle.setRadius(10.f);
	InputListener::addInputListener();
}

CameraMovement::~CameraMovement(void)
{
	octo::Application::getCamera().setSize(m_baseSize);
	InputListener::removeInputListener();
}

void CameraMovement::update(sf::Time frametime, CharacterOcto & octo)
{
	octo::Camera & camera = octo::Application::getCamera();

	std::pair<float, float> const & look = octo.look();
	if (look.first != 0.f || look.second != 0.f)
	{
		m_behavior = Behavior::FollowOcto;
		m_horizontalTransition = look.first;
		m_verticalTransition = look.second;
	}
	else if (octo.isInRocketEnd())
		m_behavior = Behavior::FollowOcto;
	else if (octo.isRaising())
	{
		if (m_behavior == ControlledByPlayer)
			m_blockAxisY = true;
		else
			m_behavior = Behavior::OctoRaising;
	}
	else if (m_behavior != ControlledByPlayer)
	{
		if (octo.isCenteredCamera())
			m_behavior = Behavior::OctoCentered;
		else if (octo.isFalling())
			m_behavior = Behavior::OctoFalling;
		else
			m_behavior = Behavior::FollowOcto;
	}
	if (octo.isZooming())
		m_zoomState = ZoomState::ZoomIn;
	if (octo.isMeetingNpc())
	{
		octo.meetNpc(false);
		m_zoomState = ZoomState::ZoomIn;
	}
	if (octo.isStopFollowCamera())
		m_behavior = StopFollowOcto;

	float goalTop = octo.getPosition().y - camera.getRectangle().height / 4.f;
	float goalBot = octo.getPosition().y + camera.getRectangle().height / 3.f;
	float goalLeft = octo.getPosition().x - camera.getRectangle().width / 3.f;
	float goalRight = octo.getPosition().x + camera.getRectangle().width / 3.f;

	switch (m_behavior)
	{
		case Behavior::FollowOcto:
		{
			if (octo.getSpeedCamera() != 0.f)
				m_speed = octo.getSpeedCamera();
			else
				m_speed = 2.5f;
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
			m_speed = 2.5f;
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
			m_speed = 5.f;
			m_verticalTransition -= frametime.asSeconds();
			if (m_verticalTransition < 0.f)
				m_verticalTransition = 0.f;
			if (m_horizontalTransition + frametime.asSeconds() < 0.f)
				m_horizontalTransition += frametime.asSeconds();
			else if (m_horizontalTransition - frametime.asSeconds() > 0.f)
				m_horizontalTransition -= frametime.asSeconds();
			break;
		}
		case Behavior::OctoCentered:
		{
			if (Progress::getInstance().getCurrentDestination() == Level::IceB)
				m_speed = 2.0f;
			else
				m_speed = 4.f;
			if (m_verticalTransition < 0.5f)
			{
				m_verticalTransition += frametime.asSeconds();
				if (m_verticalTransition > 0.5f)
					m_verticalTransition = 0.5f;
			}
			else if (m_verticalTransition > 0.5f)
			{
				m_verticalTransition -= frametime.asSeconds();
				if (m_verticalTransition < 0.5f)
					m_verticalTransition = 0.5f;
			}
			if (m_horizontalTransition + frametime.asSeconds() < 0.f)
				m_horizontalTransition += frametime.asSeconds();
			else if (m_horizontalTransition - frametime.asSeconds() > 0.f)
				m_horizontalTransition -= frametime.asSeconds();
			break;
		}
		case Behavior::ControlledByPlayer:
		{
			m_speed = 3.f;
			if (m_blockAxisY) // If octo is raising, we don't move the Y axis
			{
				m_verticalTransition -= frametime.asSeconds();
				if (m_verticalTransition < 0.f)
					m_verticalTransition = 0.f;
			}
			else
			{
				m_verticalTransition += m_verticalAxis * frametime.asSeconds();
				if (m_verticalTransition > 1.f)
					m_verticalTransition = 1.f;
				else if (m_verticalTransition < 0.f)
					m_verticalTransition = 0.f;
			}

			m_horizontalTransition += m_horizontalAxis * frametime.asSeconds();
			if (m_horizontalTransition > 1.f)
				m_horizontalTransition = 1.f;
			else if (m_horizontalTransition < -1.f)
				m_horizontalTransition = -1.f;
			break;
		}
		case Behavior::StopFollowOcto:
		{
			m_speed = std::max(m_speed - frametime.asSeconds() * 20.f, 0.f);
			break;
		}
		default:
			m_speed = 3.f;
			break;
	}

	switch (m_zoomState)
	{
		case ZoomState::ZoomIn:
			m_zoomTimer += frametime;
			m_speed *= 2.f;
			if (m_zoomTimer > m_zoomTimerMax)
			{
				m_zoomTimer = m_zoomTimerMax;
				m_zoomState = ZoomState::ZoomOut;
			}
			break;
		case ZoomState::ZoomOut:
			m_zoomTimer -= frametime;
			m_speed *= 2.f;
			if (m_zoomTimer <= sf::Time::Zero)
			{
				m_zoomTimer = sf::Time::Zero;
				m_zoomState = ZoomState::None;
			}
			break;
		default:
			break;
	}

	sf::Vector2f goal = sf::Vector2f(octo::linearInterpolation(goalRight, goalLeft, (m_horizontalTransition + 1.f) / 2.f),
									octo::linearInterpolation(goalTop, goalBot, (m_verticalTransition)));

	if (m_shakeTimer < m_shakeDuration)
	{
		m_shakeTimer += frametime;
		m_waveTimer += frametime;
		if (m_waveTimer >= m_waveDuration)
		{
			m_prevShakeOffset = m_shakeOffset;
			m_shakeOffset.x = m_noise.perlin(m_shakeTimer.asSeconds(), 0.f, 3, 2.f);
			m_shakeOffset.y = m_noise.perlin(0.f, m_shakeTimer.asSeconds(), 3, 2.f);
			octo::normalize(m_shakeOffset);
			m_shakeOffset *= m_shakeIntensity;
			m_waveTimer -= m_waveDuration;
		}
		if (m_shakeTimer >= m_shakeDuration)
		{
			m_prevShakeOffset = octo::linearInterpolation(m_prevShakeOffset, m_shakeOffset, std::min(1.f, m_waveTimer/ m_waveDuration));
			m_shakeOffset = sf::Vector2f(0.f, 0.f);
			m_waveTimer = sf::Time::Zero;
		}
	}
	else
	{
		if (m_waveTimer < m_waveDuration)
			m_waveTimer += frametime;
	}

	sf::Vector2f offset = octo::linearInterpolation(m_prevShakeOffset, m_shakeOffset, std::min(1.f, m_waveTimer / m_waveDuration));
	camera.setSize(octo::cosinusInterpolation(m_baseSize, m_baseSize * 0.85f, m_zoomTimer.asSeconds() / m_zoomTimerMax.asSeconds()));
	camera.setCenter(octo::linearInterpolation(camera.getCenter(), goal, m_speed * frametime.asSeconds()) + offset);

	m_circle.setPosition(goal);
	m_blockAxisY = false;
}

void CameraMovement::shake(float duration, float intensity, float waveDuration)
{
	m_shakeTimer = sf::Time::Zero;
	m_shakeDuration = sf::seconds(duration);
	m_shakeIntensity = intensity;
	m_waveDuration = sf::seconds(waveDuration);
	m_waveTimer = m_waveDuration;
}

void CameraMovement::debugDraw(sf::RenderTarget & render)
{
	m_circle.setFillColor(sf::Color::Red);
	render.draw(m_circle);
	m_circle.setPosition(octo::Application::getCamera().getCenter());
	m_circle.setFillColor(sf::Color::Green);
	render.draw(m_circle);
}

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
