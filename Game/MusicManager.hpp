#ifndef MUSICMANAGER_HPP
# define MUSICMANAGER_HPP
# include "ResourceDefinitions.hpp"
# include "ABiome.hpp"
# include <AudioManager.hpp>
# include <ResourceManager.hpp>
# include <Application.hpp>

class MusicManager
{
public:
	MusicManager();
	~MusicManager();
	void setup(ABiome & biome);
	void update(sf::Time frameTime, sf::Vector2f const & octoPos);
	void debugDraw(sf::RenderTarget & render);
private:
	struct AreaMusic
	{
		AreaMusic() = default;
		explicit AreaMusic(Level level, ResourceKey key, sf::FloatRect rect,
				sf::Time transitionTime = sf::seconds(1.f)) :
			level(level),
			name(key),
			area(rect),
			offset(sf::Time::Zero),
			transitionTime(transitionTime)
		{
			if (std::string("noMusic").compare(key))
				music = octo::Application::getResourceManager().getSound(key);
		}
		Level			level;
		ResourceKey		name;
		sf::FloatRect	area;
		sf::SoundBuffer	music;
		sf::Time		offset;
		sf::Time		transitionTime;
	};
	Level								m_currentLevel;
	octo::AudioManager &				m_audio;
	std::vector<AreaMusic>				m_music;
	std::vector<AreaMusic>				m_musicLevel;
	sf::Vector2f						m_position;
	sf::Vector2f						m_mapSize;
	bool								m_played;
	ResourceKey							m_current;
	sf::Time							m_timer;
	float								m_maxVolume;

private:
	void	basePosition(sf::Vector2f const & octoPos);
	void	transition(sf::Time frameTime);
};

#endif