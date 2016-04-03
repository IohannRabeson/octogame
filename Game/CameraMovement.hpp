#ifndef CAMERAMOVEMENT_HPP
# define CAMERAMOVEMENT_HPP

# include <SFML/System/Vector2.hpp>
# include <SFML/System/Time.hpp>

# include <iostream>

class CameraMovement
{
public:
	CameraMovement(void);
	virtual ~CameraMovement(void) = default;

	void follow(sf::Time frameTime, sf::Vector2f const & octoPos);
	void collideZoom(sf::Time frametime);
	void collideZoomEvent(sf::Vector2f const & npcPos);
	void setEventFallTimer(sf::Time const & eventFallTimer);
	void setFall(bool isFall);

private:
	sf::Vector2f								m_initialSize;
	sf::Vector2f								m_currentSize;
	float										m_zoomFactor;
	bool										m_isZoom;
	bool										m_isFall;
	sf::Time									m_eventFallTimer;
	sf::Vector2f								m_npcPos;
	sf::Time									m_timerZoom;
	sf::Time									m_timerZoomMax;
	sf::Time									m_timerUnZoom;
	sf::Time									m_timerUnZoomMax;
	sf::Time									m_timerBeforeChange;
	sf::Time									m_timerBeforeChangeMax;
	sf::Time									m_timerChangeLookAt;
	sf::Time									m_timerChangeLookAtMax;
};

#endif
