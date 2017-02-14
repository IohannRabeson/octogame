#ifndef MUSICMANAGER_HPP
# define MUSICMANAGER_HPP
# include "ResourceDefinitions.hpp"
# include "ABiome.hpp"
# include "RandomGenerator.hpp"
# include <AudioManager.hpp>
# include <ResourceManager.hpp>
# include <Application.hpp>

class MusicManager
{
public:
	static MusicManager &	getInstance(void);
	static void				deleteInstance(void);
	MusicManager();
	virtual ~MusicManager();
	void setup(ABiome & biome);
	void update(sf::Time frameTime, sf::Vector2f const & octoPos);
	void debugDraw(sf::RenderTarget & render);
	void startEvent(void);
	void endEvent(void);

private:
	static std::unique_ptr<MusicManager> m_instance;
	struct AreaMusic
	{
		AreaMusic() = default;
		explicit AreaMusic(Level level, ResourceKey key, sf::FloatRect rect, sf::Time transitionTime = sf::seconds(2.f)) :
			level(level),
			name(key),
			area(rect),
			offset(sf::Time::Zero),
			transitionTime(transitionTime)
		{
		}
		Level			level;
		ResourceKey		name;
		sf::FloatRect	area;
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
	RandomGenerator						m_generator;

	bool								m_startEvent;
	bool								m_endEvent;
	sf::Time							m_eventTimer;
	sf::Time							m_eventTimerMax;

private:
	void	basePosition(sf::Vector2f const & octoPos);
	void	transition(sf::Time frameTime);
	void	grow(AreaMusic & music, sf::Time frameTime);
	void	fade(AreaMusic & music, sf::Time frameTime);
	void	updateEvent(sf::Time frameTime);
};

#endif
