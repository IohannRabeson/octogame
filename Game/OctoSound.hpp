#ifndef OCTOSOUND_HPP
# define OCTOSOUND_HPP
# include "CharacterOcto.hpp"
# include <AudioManager.hpp>

class CharacterOcto::OctoSound
{
public:
	OctoSound();
	~OctoSound();
	void	update(sf::Time frameTime, Events event, bool inWater, bool onGround);
private:
	void	resetTimeEvent();
	void	startEvent(Events event);
	void	duringEvent(sf::Time frameTime, Events event);
	void	walkSound();
	std::shared_ptr<sf::Sound>	m_sound;
	Events						m_prevEvent;

	sf::Time										m_timeEventFall;
	sf::Time										m_timeEventIdle;
	sf::Time										m_timeEventElevator;
	bool											m_inWater;
	bool											m_onGround;
	float											m_volumeEffect;
	float											m_volumeVoice;
	std::mt19937									m_engine;
	std::uniform_real_distribution<float>			m_pitchDistribution;
};

#endif
