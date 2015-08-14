#ifndef MUSICPLAYER_HPP
# define MUSICPLAYER_HPP

#include "ABiome.hpp"
#include <AudioManager.hpp>
#include <ResourceManager.hpp>

class CharacterOcto;

class MusicPlayer
{
public:
	MusicPlayer(void);
	~MusicPlayer(void) = default;

	void update(CharacterOcto & octo);
private:
	octo::AudioManager &	m_audio;
	octo::ResourceManager &	m_resource;

	sf::Vector2f			m_mapSize;
	bool					m_isPlayingOpusI;
	bool					m_isPlayingBallade;

};

#endif
