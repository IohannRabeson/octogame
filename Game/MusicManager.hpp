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
	enum class MusicNameArea : std::size_t
	{
		CedricChallenge,
		Concert,
		Undefined,
		NoBalle,
	};
	MusicManager();
	virtual ~MusicManager();
	void setup(ABiome & biome);
	void update(sf::Time frameTime, sf::Vector2f const & octoPos);
	void debugDraw(sf::RenderTarget & render);
	void startBalleMusic(sf::Time duration, MusicNameArea name);

private:
	struct AreaMusic
	{
		AreaMusic() = default;
		explicit AreaMusic(Level level, ResourceKey key, sf::FloatRect rect, MusicNameArea areaName = MusicNameArea::Undefined,
				sf::Time transitionTime = sf::seconds(1.f)) :
			level(level),
			name(key),
			areaName(areaName),
			area(rect),
			offset(sf::Time::Zero),
			transitionTime(transitionTime)
		{
			if (std::string("noMusic").compare(key))
				music = octo::Application::getResourceManager().getSound(key);
		}
		Level			level;
		ResourceKey		name;
		MusicNameArea		areaName;
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
	RandomGenerator						m_generator;

	bool								m_newBalle;
	MusicNameArea							m_musicToPlay;
	sf::Time								m_durationBalle;

private:
	void	basePosition(sf::Vector2f const & octoPos);
	void	transition(sf::Time frameTime);
	void	grow(AreaMusic & music, sf::Time frameTime);
	void	fade(AreaMusic & music, sf::Time frameTime);
};

#endif
