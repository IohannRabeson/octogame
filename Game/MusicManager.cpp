#include "MusicManager.hpp"

MusicManager::MusicManager() :
	m_audio(octo::Application::getAudioManager()),
	m_played(false),
	m_timer(sf::Time::Zero)
{
	m_musicLevel.resize(3);
	m_musicLevel[0] = AreaMusic(Level::LevelOne, SPACE_SHIP_WAV, sf::FloatRect());
	m_musicLevel[1] = AreaMusic(Level::LevelTwo, MENU_OPUS_II_WAV, sf::FloatRect());
	m_musicLevel[2] = AreaMusic(Level::LevelThree, COLONISATION_WAV, sf::FloatRect());

	m_music.resize(3);

	// Montagne
	m_music[0] = AreaMusic(Level::LevelTwo, "noMusic",
				sf::FloatRect(sf::Vector2f(340.f * 16.f, -3000.f), sf::Vector2f(3300.f, 1500.f)));
	// oasis
	m_music[1] = AreaMusic(Level::LevelTwo, MENU_OPUS_III_WAV,
				sf::FloatRect(sf::Vector2f(665.f * 16.f, -1400.f), sf::Vector2f(2000.f, 500.f)));

	// village todo pos
	m_music[2] = AreaMusic(Level::LevelThree, ACTION_FAST_WAV,
				sf::FloatRect(sf::Vector2f(760.f * 16.f, -2000.f), sf::Vector2f(225 * 16, 500.f)));

	//TODO TODO TODO
	m_maxVolume = 50.f;
}

MusicManager::~MusicManager()
{
	if (m_played)
		m_audio.stopMusic(sf::seconds(0.f));
}

void	MusicManager::setup(ABiome & biome)
{
	m_currentLevel = biome.getId();
	m_mapSize = biome.getMapSizeFloat();
}

void	MusicManager::update(sf::Time frameTime, sf::Vector2f const & octoPos)
{
	basePosition(octoPos);
	transition(frameTime);
}

void	MusicManager::debugDraw(sf::RenderTarget & render)
{
	for (auto & music : m_music)
	{
		if (music.level == m_currentLevel)
		{
			sf::RectangleShape	shape(sf::Vector2f(music.area.width, music.area.height));
			shape.setPosition(music.area.left, music.area.top);
			shape.setFillColor(sf::Color::White);
			render.draw(shape);
		}
	}
}

void	MusicManager::basePosition(sf::Vector2f const & octoPos)
{
	m_position = octoPos;
	if (m_position.x < 0.f)
	{
		while (m_position.x < 0.f)
			m_position.x += m_mapSize.x;
	}
	else if (m_position.x > m_mapSize.x)
	{
		while (m_position.x > m_mapSize.x)
			m_position.x -= m_mapSize.x;
	}
}

void	MusicManager::transition(sf::Time frameTime)
{
	bool	isStart = false;
	float	volume;
	std::size_t	inLevel = 0u;
	std::size_t	index = 0u;

	for (auto & music : m_music)
	{
		if (music.level != m_currentLevel)
			continue;
		inLevel++;
		volume = 0.f;
		if (music.area.contains(m_position) && !m_played)
		{
			if (!std::string("noMusic").compare(music.name))
				break;
			//START
			m_current = music.name;
			if (music.music.getDuration() <= music.offset)
			{std::cout << "reset" << std::endl;
				music.offset = sf::Time::Zero;
			}
			m_audio.setMusicVolume(0.f);
			m_audio.startMusic(music.music, sf::seconds(0.f),
					music.offset, true);
			m_played = true;
			isStart = true;
			std::cout << "start" << music.offset.asSeconds() << std::endl;
			break;
		}
		else if (music.area.contains(m_position)
				&& m_current != music.name && m_played)
		{
			//FADE MAIN MUSIC
			for (auto & main : m_musicLevel)
			{
				if (main.level != m_currentLevel)
					continue;
				m_timer -= frameTime;
				main.offset += frameTime;
				if (m_timer < sf::Time::Zero)
					m_timer = sf::Time::Zero;
				volume = m_maxVolume * (m_timer / main.transitionTime);
				m_audio.setMusicVolume(volume);
				std::cout << "FadeM" << volume << std::endl;
			}
			break;
		}
		else if (music.area.contains(m_position)
				&& m_current == music.name && m_played)
		{
			//GROW
			m_timer += frameTime;
			music.offset += frameTime;
			if (m_timer > music.transitionTime)
				m_timer = music.transitionTime;
			volume = m_maxVolume * (m_timer / music.transitionTime);
			std::cout << "grow" << volume << std::endl;
			m_audio.setMusicVolume(volume);
			break;
		}
		else if (!music.area.contains(m_position)
				&& m_current == music.name && m_played)
		{
			//FADE
			m_timer -= frameTime;
			music.offset += frameTime;
			if (m_timer < sf::Time::Zero)
				m_timer = sf::Time::Zero;
			volume = m_maxVolume * (m_timer / music.transitionTime);
			m_audio.setMusicVolume(volume);
			std::cout << "Fade" << volume << std::endl;
			break;
		}
		else
		{
			index++;
		}
	}
	if (index == inLevel)
	{
		for (auto & main : m_musicLevel)
		{
			if (main.level != m_currentLevel)
				continue;
			if (!m_played)
			{
				m_current = main.name;
				if (main.music.getDuration() <= main.offset)
			{
					std::cout << "reset" << std::endl;
					main.offset = sf::Time::Zero;
			}
				m_audio.startMusic(main.music, sf::seconds(0.f),
						main.offset, true);
				m_audio.setMusicVolume(0.f);
				m_played = true;
				isStart = true;
				std::cout << "startM" << main.offset.asSeconds() << std::endl;
				break;
			}
			else
			{
				m_timer += frameTime;
				main.offset += frameTime;
				if (m_timer > main.transitionTime)
					m_timer = main.transitionTime;
				volume = m_maxVolume * (m_timer / main.transitionTime);
				std::cout << "growM" << volume << std::endl;
				m_audio.setMusicVolume(volume);
				break;
			}
		}
	}
	if (m_timer == sf::Time::Zero && m_played && !isStart)
	{
		std::cout << "STOP" << volume << std::endl;
		//STOP
		m_audio.stopMusic(sf::seconds(0.f));
		m_audio.setMusicVolume(m_maxVolume);
		m_played = false;
	}
}
