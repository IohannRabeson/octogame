#ifndef OCTOSOUND_HPP
# define OCTOSOUND_HPP
# include "CharacterOcto.hpp"
# include <AudioManager.hpp>

class CharacterOcto::OctoSound
{
public:
	OctoSound();
	~OctoSound();
	void	update(sf::Time frameTime, Events event);
private:
	void	resetTimeEvent();
	void	startEvent(Events event);
	void	duringEvent(sf::Time frameTime, Events event);
	std::shared_ptr<sf::Sound>	m_sound;
	Events						m_prevEvent;

	sf::Time					m_timeEventFall;
	sf::Time					m_timeEventIdle;
};

#endif
