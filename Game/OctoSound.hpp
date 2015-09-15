#ifndef OCTOSOUND_HPP
# define OCTOSOUND_HPP
# include "CharacterOcto.hpp"
# include <AudioManager.hpp>

class CharacterOcto::OctoSound
{
public:
	OctoSound();
	void	update(sf::Time frameTime, Events event);
private:
	std::shared_ptr<sf::Sound>	m_sound;
	Events						m_prevEvent;
};

#endif
