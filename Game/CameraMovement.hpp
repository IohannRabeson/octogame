#ifndef CAMERAMOVEMENT_HPP
# define CAMERAMOVEMENT_HPP

# include <SFML/System/Vector2.hpp>
# include <SFML/System/Time.hpp>
# include <SFML/Graphics.hpp>

# include <iostream>

class CharacterOcto;

class CameraMovement
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

private:
	Behavior		m_behavior;
	float			m_speed;
	float			m_maxSpeed;
	float			m_transition;
	sf::CircleShape	m_circle;

};

#endif
