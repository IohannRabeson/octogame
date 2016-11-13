#include "MusicManager.hpp"
#include "Progress.hpp"

std::unique_ptr<MusicManager> MusicManager::m_instance = nullptr;

MusicManager & MusicManager::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new MusicManager());
	}
	return *m_instance;
}

void MusicManager::deleteInstance()
{
	if (m_instance)
		m_instance.reset(nullptr);
}

MusicManager::MusicManager() :
	m_audio(octo::Application::getAudioManager()),
	m_played(false),
	m_timer(sf::Time::Zero),
	m_generator("random"),
	m_startEvent(false),
	m_endEvent(false),
	m_eventTimerMax(sf::seconds(2.f))
{
	ResourceKey			musicKey[9];

	musicKey[0] = MUSIC_SPACE_SHIP_OGG;
	musicKey[1] = MUSIC_MENU_OPUS_II_OGG;
	musicKey[2] = MUSIC_COLONISATION_OGG;
	musicKey[3] = MUSIC_BALLADE_MENTALE_OGG;
	musicKey[4] = MUSIC_MENU_OPUS_III_OGG;
	musicKey[5] = MUSIC_ACTION_SLOW_OGG;
	musicKey[6] = MUSIC_ACTION_FAST_OGG;
	musicKey[7] = MUSIC_SOUTERRAIN_LUGUBRE_OGG;
	musicKey[8] = MUSIC_MENU_OPUS_I_OGG;

	m_musicLevel.resize(21);
	m_musicLevel[0] = AreaMusic(Level::IceA, MUSIC_SPACE_SHIP_OGG, sf::FloatRect());
	m_musicLevel[1] = AreaMusic(Level::IceB, MUSIC_ICE_A_B_OGG, sf::FloatRect());
	m_musicLevel[2] = AreaMusic(Level::IceC, MUSIC_MENU_OPUS_III_OGG, sf::FloatRect());
	m_musicLevel[3] = AreaMusic(Level::IceD, MUSIC_ICE_D_OGG, sf::FloatRect());
	m_musicLevel[4] = AreaMusic(Level::DesertA, MUSIC_MENU_OPUS_II_OGG, sf::FloatRect());
	m_musicLevel[5] = AreaMusic(Level::DesertB, MUSIC_MENU_OPUS_I_OGG, sf::FloatRect());
	m_musicLevel[6] = AreaMusic(Level::DesertC, MUSIC_MENU_OPUS_II_OGG, sf::FloatRect());
	m_musicLevel[7] = AreaMusic(Level::DesertD, MUSIC_MENU_OPUS_III_OGG, sf::FloatRect());
	m_musicLevel[8] = AreaMusic(Level::JungleA, MUSIC_COLONISATION_OGG, sf::FloatRect());
	m_musicLevel[9] = AreaMusic(Level::JungleB, MUSIC_COLONISATION_OGG, sf::FloatRect());
	m_musicLevel[10] = AreaMusic(Level::JungleC, MUSIC_ACTION_SLOW_OGG, sf::FloatRect());
	m_musicLevel[11] = AreaMusic(Level::JungleD, MUSIC_COLONISATION_OGG, sf::FloatRect());
	m_musicLevel[12] = AreaMusic(Level::WaterA, MUSIC_BALLADE_MENTALE_OGG, sf::FloatRect());
	m_musicLevel[13] = AreaMusic(Level::WaterB, MUSIC_BALLADE_MENTALE_OGG, sf::FloatRect());
	m_musicLevel[14] = AreaMusic(Level::WaterC, MUSIC_BALLADE_MENTALE_OGG, sf::FloatRect());
	m_musicLevel[15] = AreaMusic(Level::WaterD, MUSIC_BALLADE_MENTALE_OGG, sf::FloatRect());

	m_musicLevel[16] = AreaMusic(Level::Final, MUSIC_FINAL_OGG, sf::FloatRect());
	m_musicLevel[17] = AreaMusic(Level::Blue, MUSIC_PYRAMID_OGG, sf::FloatRect());
	m_musicLevel[18] = AreaMusic(Level::Red, MUSIC_PYRAMID_OGG, sf::FloatRect());
	m_musicLevel[19] = AreaMusic(Level::Random, MUSIC_SPACE_SHIP_OGG, sf::FloatRect());
	m_musicLevel[20] = AreaMusic(Level::Rewards, MUSIC_MENU_OPUS_III_OGG, sf::FloatRect());


	m_music.resize(8);
	//ice balle
	m_music[0] = AreaMusic(Level::IceC, MUSIC_MENU_OPUS_II_REVERSE_OGG,
			sf::FloatRect(sf::Vector2f(50.f * 16.f, -340.f * 16.f), sf::Vector2f(120.f * 16.f, 275.f * 16.f)));
	//desert balle
	m_music[1] = AreaMusic(Level::DesertB, MUSIC_MENU_OPUS_II_REVERSE_OGG,
			sf::FloatRect(sf::Vector2f(90.f * 16.f, -110.f * 16.f), sf::Vector2f(200.f * 16.f, 700.f * 16.f)));
	// jungle balle
	m_music[2] = AreaMusic(Level::JungleA, MUSIC_MENU_OPUS_II_REVERSE_OGG,
			sf::FloatRect(sf::Vector2f(45.f * 16.f, -2400.f), sf::Vector2f(420.f * 16.f, 2200.f)));
	//jungle b cave
	m_music[3] = AreaMusic(Level::JungleB, MUSIC_SOUTERRAIN_LUGUBRE_OGG,
			sf::FloatRect(sf::Vector2f(30.f * 16.f, 110.f * 16.f), sf::Vector2f(380.f * 16.f, 420.f * 16.f)));
	// village
	m_music[4] = AreaMusic(Level::JungleA, MUSIC_ACTION_SLOW_OGG,
			sf::FloatRect(sf::Vector2f(586.f * 16.f, -1800.f), sf::Vector2f(220.f * 16.f, 2300.f)));
	// water balle
	m_music[5] = AreaMusic(Level::WaterB, MUSIC_MENU_OPUS_II_REVERSE_OGG,
			sf::FloatRect(sf::Vector2f(0.f * 16.f, -12850.f), sf::Vector2f(700.f * 16.f, 10000.f * 16.f)));
	// water c
	m_music[6] = AreaMusic(Level::WaterC, MUSIC_SOUTERRAIN_LUGUBRE_OGG,
			sf::FloatRect(sf::Vector2f(0.f * 16.f, 400.f), sf::Vector2f(750.f * 16.f, 600.f * 16.f)));
	// final pyramid
	m_music[7] = AreaMusic(Level::Final, MUSIC_PYRAMID_OGG,
			sf::FloatRect(sf::Vector2f(650.f * 16.f, -400.f * 16.f), sf::Vector2f(400.f * 16.f, 420.f * 16.f)));
	// desert d factory
	//m_music[8] = AreaMusic(Level::DesertD, OBJECT_FACTORY_OGG,
	//		sf::FloatRect(sf::Vector2f(300.f * 16.f, -250.f * 16.f), sf::Vector2f(70.f * 16.f, 130.f * 16.f)));
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
	m_played = false;
	m_timer = sf::Time::Zero;
}

void	MusicManager::update(sf::Time frameTime, sf::Vector2f const & octoPos)
{
	m_maxVolume = Progress::getInstance().getMusicVolume();
	basePosition(octoPos);
	transition(frameTime);
	updateEvent(frameTime);
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
	std::size_t	inLevel = 0u;
	std::size_t	index = 0u;

	for (auto & music : m_music)
	{
		if (music.level != m_currentLevel)
			continue;
		inLevel++;
		if (music.area.contains(m_position) && !m_played)
		{
			if (!std::string("noMusic").compare(music.name))
				break;
			index = inLevel;
			//START
			m_current = music.name;
			if (music.music.getDuration() <= music.offset)
				music.offset = sf::Time::Zero;
			m_audio.setMusicVolume(0.f);
			m_timer = sf::Time::Zero;
			m_audio.startMusic(music.music, sf::Time::Zero, music.offset, true);
			m_played = true;
			isStart = true;
			break;
		}
		else if (music.area.contains(m_position) && m_current != music.name && m_played)
		{
			//FADE MAIN MUSIC
			for (auto & main : m_musicLevel)
			{
				if (main.level != m_currentLevel)
					continue;
				fade(main, frameTime);
			}
			break;
		}
		else if (music.area.contains(m_position) && m_current == music.name && m_played)
		{
			grow(music, frameTime);
			break;
		}
		else if (!music.area.contains(m_position) && m_current == music.name && m_played)
		{
			fade(music, frameTime);
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
					main.offset = sf::Time::Zero;
				//TODO: Find a better way to do that
				if (m_currentLevel == Level::Blue || m_currentLevel == Level::Red)
					m_audio.startMusic(main.music, sf::Time::Zero, m_music[7].offset, true);
				else
					m_audio.startMusic(main.music, sf::Time::Zero, main.offset, true);
				m_audio.setMusicVolume(0.f);
				m_timer = sf::Time::Zero;
				m_played = true;
				isStart = true;
				break;
			}
			else
			{
				grow(main, frameTime);
				break;
			}
		}
	}
	if (m_timer == sf::Time::Zero && m_played && !isStart)
	{
		//STOP
		m_audio.stopMusic(sf::Time::Zero);
		m_played = false;
	}
}

void	MusicManager::grow(AreaMusic & music, sf::Time frameTime)
{
	float	volume = 0.f;

	m_timer += frameTime;
	music.offset += frameTime;
	if (m_timer > music.transitionTime)
		m_timer = music.transitionTime;
	volume = m_maxVolume * (m_timer / music.transitionTime);
	m_audio.setMusicVolume(volume);
}

void	MusicManager::fade(AreaMusic & music, sf::Time frameTime)
{
	float	volume = 0.f;

	m_timer -= frameTime;
	music.offset += frameTime;
	if (m_timer < sf::Time::Zero)
		m_timer = sf::Time::Zero;
	volume = m_maxVolume * (m_timer / music.transitionTime);
	m_audio.setMusicVolume(volume);
}

void	MusicManager::updateEvent(sf::Time frameTime)
{
	if (m_startEvent)
	{
		octo::AudioManager &	audio = octo::Application::getAudioManager();
		Progress const &		progress = Progress::getInstance();
		float					refSoundVolume = progress.getSoundVolume();
		float					refMusicVolume = progress.getMusicVolume();

		if (!m_endEvent)
			m_eventTimer = std::min(m_eventTimer + frameTime, m_eventTimerMax);
		else
			m_eventTimer = std::max(m_eventTimer - frameTime, sf::Time::Zero);

		audio.setMusicVolume(octo::linearInterpolation(refMusicVolume, refMusicVolume * 2.f, m_eventTimer / m_eventTimerMax));
		audio.setSoundVolume(octo::linearInterpolation(refSoundVolume, refSoundVolume * 0.2f, m_eventTimer / m_eventTimerMax));

		if (m_eventTimer == sf::Time::Zero)
		{
			m_startEvent = false;
			m_endEvent = false;
		}
	}
}

void	MusicManager::startEvent()
{
	m_startEvent = true;
}

void	MusicManager::endEvent()
{
	m_endEvent = true;
}
