#ifndef MUSICPLAYER_HPP
# define MUSICPLAYER_HPP

#include "ABiome.hpp"
#include <AudioManager.hpp>
#include <ResourceManager.hpp>
#include <vector>
#include <SFML/System/Time.hpp>

#include "ResourceDefinitions.hpp"

class CharacterOcto;

class MusicPlayer
{
public:
	MusicPlayer(void);
	~MusicPlayer(void) = default;

	void setup(ABiome const & biome);
	void update(sf::Time frameTime, sf::Vector2f const & octoPos);

private:
struct MusicValue
{
	ResourceKey		key;
	sf::SoundBuffer	soundBuffer;
	sf::Time		duration;
	sf::Time		offset;
};

	void initMusicValue(MusicValue & music, ResourceKey key);
	ResourceKey const & getOpusMusic(ResourceKey const & key);

	bool getEvent(sf::Vector2f const & octoPos);
	void playMainMusic(sf::Time frameTime,
						std::vector<MusicValue> & musics,
						bool & startStateOn,
						bool & endStateOn,
						ResourceKey & current);

	octo::AudioManager &		m_audio;
	octo::ResourceManager &		m_resource;

	std::vector<MusicValue>	m_mainMusics;
	bool					m_mainMusicOn;
	ResourceKey				m_currentMainKey;
	std::vector<MusicValue>	m_eventMusics;
	bool					m_eventMusicOn;
	ResourceKey				m_currentEventKey;
	std::string				m_biomeName;
};

#endif
