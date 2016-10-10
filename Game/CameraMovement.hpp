#ifndef CAMERAMOVEMENT_HPP
# define CAMERAMOVEMENT_HPP

# include "InputListener.hpp"
# include "Noise.hpp"
# include <SFML/System/Vector2.hpp>
# include <SFML/System/Time.hpp>
# include <SFML/Graphics.hpp>

class CharacterOcto;

class CameraMovement : public InputListener
{
public:
	enum Behavior
	{
		FollowOcto,
		OctoFalling,
		OctoRaising,
		OctoCentered,
		ControlledByPlayer,
		StopFollowOcto
	};

	enum ZoomState
	{
		ZoomIn,
		ZoomOut,
		None
	};

	CameraMovement(void);
	virtual ~CameraMovement(void);

	void shake(float duration, float intensity, float waveDuration);

	void update(sf::Time frameTime, CharacterOcto & octo);
	void debugDraw(sf::RenderTarget & render);

	bool onInputPressed(InputListener::OctoKeys const & key);
	bool onInputReleased(InputListener::OctoKeys const & key);

private:
	Behavior		m_behavior;
	Noise			m_noise;
	ZoomState		m_zoomState;
	sf::Vector2f	m_baseSize;
	sf::Vector2f	m_prevShakeOffset;
	sf::Vector2f	m_shakeOffset;
	sf::Time		m_zoomTimer;
	sf::Time		m_zoomTimerMax;
	sf::Time		m_waveTimer;
	sf::Time		m_shakeTimer;
	sf::Time		m_shakeDuration;
	sf::Time		m_waveDuration;
	float			m_shakeIntensity;
	float			m_speed;
	float			m_verticalTransition;
	float			m_horizontalTransition;
	float			m_horizontalAxis;
	float			m_verticalAxis;
	sf::CircleShape	m_circle;

};

#endif
