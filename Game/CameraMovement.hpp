#ifndef CAMERAMOVEMENT_HPP
# define CAMERAMOVEMENT_HPP

# include "InputListener.hpp"

# include <SFML/System/Vector2.hpp>
# include <SFML/System/Time.hpp>
# include <SFML/Graphics.hpp>

# include <iostream>

class CharacterOcto;

class CameraMovement : public InputListener
{
public:
	enum Behavior
	{
		FollowOcto,
		OctoFalling,
		OctoRaising,
		ZoomNpc,
		ControlledByPlayer
	};

	CameraMovement(void);
	virtual ~CameraMovement(void);

	void update(sf::Time frameTime, CharacterOcto & octo);
	void debugDraw(sf::RenderTarget & render);

	bool onInputPressed(InputListener::OctoKeys const & key);
	bool onInputReleased(InputListener::OctoKeys const & key);

private:
	Behavior		m_behavior;
	sf::Vector2f	m_baseSize;
	sf::Time		m_zoomTimer;
	sf::Time		m_zoomTimerMax;
	float			m_speed;
	float			m_verticalTransition;
	float			m_horizontalTransition;
	float			m_horizontalAxis;
	float			m_verticalAxis;
	bool			m_zoom;
	sf::CircleShape	m_circle;

};

#endif
