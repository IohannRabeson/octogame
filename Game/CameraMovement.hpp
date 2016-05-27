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
	virtual ~CameraMovement(void) = default;

	void update(sf::Time frameTime, CharacterOcto & octo);
	void collideZoom(sf::Time frametime);
	void collideZoomEvent(sf::Vector2f const & npcPos);
	void setEventFallTimer(sf::Time const & eventFallTimer);
	void draw(sf::RenderTarget & render);

	bool onInputPressed(InputListener::OctoKeys const & key);
	bool onInputReleased(InputListener::OctoKeys const & key);

private:
	Behavior		m_behavior;
	float			m_speed;
	float			m_maxSpeed;
	float			m_verticalTransition;
	float			m_horizontalTransition;
	float			m_horizontalAxis;
	float			m_verticalAxis;
	sf::CircleShape	m_circle;

};

#endif
