#include "MusicManager.hpp"
#include "Progress.hpp"

MusicManager::MusicManager() :
	m_audio(octo::Application::getAudioManager()),
	m_played(false),
	m_timer(sf::Time::Zero)
{
	m_musicLevel.resize(4);
	m_musicLevel[0] = AreaMusic(Level::LevelOne, SPACE_SHIP_WAV, sf::FloatRect());
	m_musicLevel[1] = AreaMusic(Level::LevelTwo, MENU_OPUS_II_WAV, sf::FloatRect());
	m_musicLevel[2] = AreaMusic(Level::LevelThree, COLONISATION_WAV, sf::FloatRect());
	m_musicLevel[3] = AreaMusic(Level::LevelFour, BALLADE_MENTALE_WAV, sf::FloatRect());

	m_music.resize(6);

	// Montagne
	m_music[0] = AreaMusic(Level::LevelTwo, MENU_OPUS_III_WAV,
			sf::FloatRect(sf::Vector2f(340.f * 16.f, -3400.f), sf::Vector2f(3300.f, 1900.f)));
	// oasis
	//	m_music[1] = AreaMusic(Level::LevelTwo, MENU_OPUS_III_WAV,
	//			sf::FloatRect(sf::Vector2f(665.f * 16.f, -1700.f), sf::Vector2f(2100.f, 900.f)));
	// village
	m_music[1] = AreaMusic(Level::LevelThree, ACTION_SLOW_WAV,
			sf::FloatRect(sf::Vector2f(750.f * 16.f, -3500.f), sf::Vector2f(235.f * 16.f, 2300.f)));
	// cedric challenge
	m_music[2] = AreaMusic(Level::LevelThree, ACTION_FAST_WAV,
			sf::FloatRect(sf::Vector2f(55.f * 16.f, -3400.f), sf::Vector2f(530.f * 16.f, 2200.f)));
	//water
	m_music[3] = AreaMusic(Level::LevelFour, SOUTERRAIN_LUGUBRE_WAV,
			sf::FloatRect(sf::Vector2f(0.f, 1.f), sf::Vector2f(1200.f * 16.f, 3200.f)));
	//run
	m_music[4] = AreaMusic(Level::LevelFour, MENU_OPUS_I_WAV,
			sf::FloatRect(sf::Vector2f(125.f * 16.f, -6000.f), sf::Vector2f(415.f * 16.f, 5200.f)));
	//concert
	m_music[5] = AreaMusic(Level::LevelFour, MENU_OPUS_II_REVERSE_WAV,
			sf::FloatRect(sf::Vector2f(700.f * 16.f, -3400.f), sf::Vector2f(70.f * 16.f, 1350.f)));
}

MusicManager::~MusicManager()
{
	if (m_played)
		m_audio.stopMusic(sf::Time::Zero);
	m_audio.setMusicVolume(m_maxVolume);
	Progress::getInstance().save();
}

void	MusicManager::setup(ABiome & biome)
{
	m_currentLevel = biome.getId();
	m_mapSize = biome.getMapSizeFloat();
	m_maxVolume = Progress::getInstance().getMusicVolume();
}

void	MusicManager::update(sf::Time frameTime, sf::Vector2f const & octoPos)
{
	m_maxVolume = Progress::getInstance().getMusicVolume();
	basePosition(octoPos);
	transition(frameTime);
}

void	MusicManager::debugDraw(sf::RenderTarget & render)
{
	sf::Rect<float> rect = octo::Application::getCamera().getRectangle();
	bool	mXm = false;
	bool	mXp = false;

	std::size_t indexX = 0;
	if (rect.left < 0)
	{
		while (rect.left < 0){
			rect.left += m_mapSize.x;
			mXm = true;
			indexX++;
		}
	}
	else if (rect.left > m_mapSize.x)
	{
		while (rect.left > m_mapSize.x)
		{
			indexX++;
			mXp = true;
			rect.left -= m_mapSize.x;
		}
	}
	for (auto & music : m_music)
	{
		if (music.level == m_currentLevel)
		{
			sf::RectangleShape	shape(sf::Vector2f(music.area.width, music.area.height));
			if (music.area.intersects(rect))
			{
				float top = music.area.top;
				float left = music.area.left;
				if (mXm)
					left -= (m_mapSize.x * indexX);
				else if (mXp)
					left += (m_mapSize.x * indexX);
				shape.setPosition(left, top);
			}
			else
				shape.setPosition(music.area.left, music.area.top);
			shape.setFillColor(sf::Color(63, 206, 87));
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
	bool		isStart = false;
	float		volume;
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
			{
				music.offset = sf::Time::Zero;
			}
			m_audio.setMusicVolume(0.f);
			m_timer = sf::Time::Zero;
			m_audio.startMusic(music.music, sf::Time::Zero,
					music.offset, true);
			m_played = true;
			isStart = true;
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
					main.offset = sf::Time::Zero;
				}
				m_audio.startMusic(main.music, sf::Time::Zero,
						main.offset, true);
				m_audio.setMusicVolume(0.f);
				m_timer = sf::Time::Zero;
				m_played = true;
				isStart = true;
				break;
			}
			else
			{
				m_timer += frameTime;
				main.offset += frameTime;
				if (m_timer > main.transitionTime)
					m_timer = main.transitionTime;
				volume = m_maxVolume * (m_timer / main.transitionTime);
				m_audio.setMusicVolume(volume);
				break;
			}
		}
	}
	if (m_timer == sf::Time::Zero && m_played && !isStart)
	{
		//STOP
		m_audio.stopMusic(sf::Time::Zero);
		//m_audio.setMusicVolume(m_maxVolume);
		m_played = false;
	}
}
