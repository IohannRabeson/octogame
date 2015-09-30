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
	struct soundFade
	{
		soundFade() = default;
		explicit soundFade(std::shared_ptr<sf::Sound> sound) :
			sound(sound),
			time(sf::Time::Zero),
			stop(false)
		{
			m_maxVolume = sound->getVolume();
		}
		std::shared_ptr<sf::Sound>		sound;
		sf::Time						time;
		bool							stop;
		float							m_maxVolume;
	};
	void	resetTimeEvent();
	void	environmentEvent(bool inWater, bool onGround);
	void	startEvent(Events event);
	void	duringEvent(sf::Time frameTime, Events event);
	void	walkSound();
	void	stopSound();
	void	fadeOut(sf::Time frameTime);
	std::shared_ptr<sf::Sound>				m_sound;
	std::shared_ptr<sf::Sound>				m_soundEnvironment;
	std::shared_ptr<sf::Sound>				m_soundTransition;
	std::vector<soundFade>					m_soundFadeOut;
	Events									m_prevEvent;

	sf::Time										m_timeEventFall;
	sf::Time										m_timeEventIdle;
	sf::Time										m_timeEventElevator;
	sf::Time										m_timeSoundIn;
	sf::Time										m_timeSoundTransition;
	sf::Time										m_timeSoundTransitionMax;
	bool											m_inWater;
	bool											m_onGround;
	bool											m_landing;
	bool											m_transitionInWater;
	bool											m_transitionOutWater;
	float											m_volumeEffect;
	float											m_volumeVoice;
	std::mt19937									m_engine;
	std::uniform_real_distribution<float>			m_pitchDistribution;
};

#endif
