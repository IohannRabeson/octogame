#include "Progress.hpp"
#include "CharacterOcto.hpp"
#include "Tile.hpp"
#include "ABiome.hpp"
#include "SteamAPI.hpp"
#include <SFML/Audio/Listener.hpp>
#include <Application.hpp>
#include <Options.hpp>
#include <AudioManager.hpp>
#include <GraphicsManager.hpp>
#include <SFML/System/Clock.hpp>

#include <fstream>

std::unique_ptr<Progress> Progress::m_instance = nullptr;
std::unique_ptr<SteamAPI> Progress::m_steam = nullptr;

Progress::Progress() :
	m_isMenu(true),
	m_isIntro(false),
	m_isBubbleNpc(true),
	m_filename(octo::Application::getOptions().getPath() + "save.osv"),
	m_newSave(false),
	m_changeLevel(false),
	m_reverseSprite(false),
	m_killOcto(false),
	m_isDoubleJump(false),
	m_isInCloud(false),
	m_cloudId(0u),
	m_npcMax(0u),
	m_countRandomDiscover(0u),
	m_checkpointCountMax(0u),
	m_isOctoOnInstance(false),
	m_isHighLight(false),
	m_isMapMoving(false),
	m_canOctoMoveMap(true),
	m_forceMapToMove(false),
	m_balleMultiplier(5.f),
	m_isResourceLoading(false)
{
}

Progress & Progress::getInstance()
{
	if (m_instance == nullptr)
		m_instance.reset(new Progress());
	return *m_instance;
}

SteamAPI & Progress::getSteamInstance()
{
	if (m_steam == nullptr)
		m_steam.reset(new SteamAPI());
	return *m_steam;
}

bool	Progress::isMenu() const
{
	return m_isMenu;
}

void	Progress::setMenu(bool isMenu)
{
	setBubbleNpc(!isMenu);
	m_isMenu = isMenu;
}

bool	Progress::isIntro() const
{
	return m_isIntro;
}

void	Progress::setIntro(bool isIntro)
{
	m_isIntro = isIntro;
}

bool	Progress::isBubbleNpc() const
{
	return m_isBubbleNpc;
}

void	Progress::setBubbleNpc(bool isBubbleNpc)
{
	m_isBubbleNpc = isBubbleNpc;
}

Progress::MenuType	Progress::getMenuType(void)
{
	return m_data.menuType;
}

void	Progress::setMenuType(MenuType type)
{
	m_data.menuType = type;
}

bool	Progress::isGameFinished() const
{
	return m_data.isGameFinished;
}

void	Progress::setGameFinished(bool finish)
{
	m_data.isGameFinished = finish;
	if (m_data.difficulty == Difficulty::Hard)
		m_data.isGameFinishedHard = finish;
	if (m_data.deathCount == 0u)
		m_data.isGameFinishedZeroDeath = finish;
}

void	Progress::increaseLaunchCount(void)
{
	m_data.launchCount++;
}

void	Progress::setBlueEnd(bool finish)
{
	m_data.isBlueEnd = finish;
}

void	Progress::setRedEnd(bool finish)
{
	m_data.isRedEnd = finish;
}

void	Progress::increaseJumpCount(void)
{
	m_data.jumpCount++;
}

void	Progress::setLongIntro(bool longIntro)
{
	m_data.longIntro = longIntro;
}

void	Progress::setTryToEscape(bool tryToEscape)
{
	m_data.tryToEscape = tryToEscape;
}

void	Progress::setDoorFound(bool doorFound)
{
	m_data.doorFound = doorFound;
}

void	Progress::setup()
{
	m_newSave = false;
	m_filename = octo::Application::getOptions().getPath() + "save.osv";
	m_data = data();
}

void	Progress::load(std::string const &filename)
{
	m_filename = octo::Application::getOptions().getPath() + filename;
	std::ifstream filestream(m_filename, std::ios::in | std::ios::binary);
	if(!filestream)
	{
		m_newSave = true;
		return;
	}
	filestream.read(reinterpret_cast<char*>(&m_data), sizeof(struct data));
	filestream.close();
	init();
}

void	Progress::init()
{
	octo::AudioManager & audio = octo::Application::getAudioManager();
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();

	audio.setMusicVolume(m_data.musicVol);
	audio.setSoundVolume(m_data.soundVol);
	sf::Listener::setGlobalVolume(m_data.globalVol);
	graphics.setFullscreen(m_data.fullscreen);
	graphics.setVerticalSyncEnabled(m_data.vsync);
	m_isOctoOnInstance = false;
	m_killOcto = false;
	loadNpc();
	loadPortals();
	loadDeaths();

}

void	Progress::save(float timePlayed)
{
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();

	m_data.fullscreen = graphics.isFullscreen();
	m_data.vsync = graphics.isVerticalSyncEnabled();
	m_data.timePlayed += timePlayed / 60.f;

	saveNpc();
	savePortals();
	saveToFile();
	saveDeaths();
}

void	Progress::updateSteam(sf::Time frameTime)
{
	m_timerSteamUpdate -= frameTime;
	if (m_timerSteamUpdate < sf::Time::Zero)
	{
		m_timerSteamUpdate = sf::seconds(1.f);
		m_steam->update(m_data);
	}
}

void	Progress::saveToFile()
{
	std::ofstream filestream(m_filename, std::ios::out | std::ios::binary);
	if (!filestream)
		return;
	filestream.write(reinterpret_cast<char*>(&m_data), sizeof(struct data));
	filestream.close();
}

void	Progress::reset()
{
	float globalVolume = m_data.globalVol;
	float musicVolume = m_data.musicVol;
	float soundVolume = m_data.soundVol;
	Keyboard keyboard = m_data.keyboard;
	Language language = m_data.language;

	m_changeLevel = false;
	m_reverseSprite = false;
	m_data.spaceShipRepair = false;
	m_npc.clear();
	m_portals.clear();
	m_deaths.clear();
	setup();
	m_data.globalVol = globalVolume;
	m_data.musicVol = musicVolume;
	m_data.soundVol = soundVolume;
	m_data.keyboard = keyboard;
	m_data.language = language;
	m_data.npc[0] = '0';
	m_data.npc[1] = '\0';
	m_data.deaths[0] = '0';
	m_data.deaths[1] = '\0';
	m_data.portals[0] = '0';
	m_data.portals[1] = '\0';
	save();
}

void	Progress::setLanguage(Language language)
{
	m_data.language = language;
}

Progress::Language Progress::getLanguage(void) const
{
	return m_data.language;
}

void	Progress::setDifficulty(Difficulty difficulty)
{
	m_data.difficulty = difficulty;
}

Progress::Difficulty Progress::getDifficulty(void) const
{
	return m_data.difficulty;
}

void	Progress::setKeyboard(Keyboard keyboard)
{
	m_data.keyboard = keyboard;
}

Progress::Keyboard Progress::getKeyboard(void) const
{
	return m_data.keyboard;
}

ResourceKey Progress::getTextFile(void) const
{
	if (m_data.language == Language::en)
		return DIALOGS_EN_TXT;
	return DIALOGS_FR_TXT;
}

bool	Progress::isJoystick(void) const
{
	sf::Joystick::update();
	return sf::Joystick::isConnected(0);
}

void	Progress::addNanoRobot()
{
	m_data.nanoRobotCount++;
	assert(m_data.nanoRobotCount <= 8);
	save();
}

void	Progress::setNanoRobotCount(std::size_t count)
{
	m_data.nanoRobotCount = count;
	assert(m_data.nanoRobotCount <= 8);
	save();
}

void	Progress::setSpiritCount(std::size_t count)
{
	m_data.spiritCount = count;
	save();
}

void	Progress::addSpirit()
{
	m_data.spiritCount++;
	save();
}

void	Progress::setNextDestination(Level const & destination, bool hasTransition)
{
	if (!m_isMenu || destination != Level::Rewards)
	{
		if (isGameFinished())
		{
			if (m_data.currentDestination != Level::Random)
				m_data.lastDestination = m_data.currentDestination;
		}
		else if (destination != m_data.currentDestination)
			m_data.lastDestination = m_data.currentDestination;
		m_data.nextDestination = destination;
	}
	m_changeLevel = hasTransition;
	m_forceMapToMove = false;

	if (destination == Level::EndRocket)
		m_data.isBlueEnd = true;
	if (destination == Level::Red)
		m_data.isRedEnd = true;
}

Level	Progress::getNextDestination(void) const
{
	return m_data.nextDestination;
}

void	Progress::setCurrentDestination(Level destination)
{
	if (destination != Level::Rewards)
		m_data.currentDestination = destination;
}

Level	Progress::getCurrentDestination(void) const
{
	return m_data.currentDestination;
}

void	Progress::setLastDestination(Level destination)
{
	if (destination != Level::Rewards)
		m_data.lastDestination = destination;
}

Level	Progress::getLastDestination(void) const
{
	return m_data.lastDestination;
}

void	Progress::validateChallenge(ChallengeManager::Effect effect)
{
	m_data.validateChallenge |= static_cast<int>(effect);
}

bool	Progress::isValidateChallenge(ChallengeManager::Effect effect)
{
	return m_data.validateChallenge & static_cast<int>(effect);
}

bool	Progress::canMoveMap()
{
	return (m_data.nanoRobotCount > 0);
}

bool	Progress::canRepair()
{
	return (m_data.nanoRobotCount > 1);
}

bool	Progress::canRepairShip()
{
	return (m_data.nanoRobotCount > 6);
}

bool	Progress::canWalk()
{
	return true;
}

bool	Progress::canJump()
{
	return (m_data.nanoRobotCount > 2);
}

bool	Progress::canDoubleJump()
{
	return (m_data.nanoRobotCount > 3);
}

bool	Progress::canSlowFall()
{
	return (m_data.nanoRobotCount > 5);
}

bool	Progress::canUseElevator()
{
	return (m_data.nanoRobotCount > 1);
}

bool	Progress::canUseWaterJump()
{
	return (m_data.nanoRobotCount > 4);
}

bool	Progress::canUseBalle()
{
	return (m_data.nanoRobotCount > 7);
}

bool	Progress::changeLevel() const
{
	return m_changeLevel;
}

void	Progress::levelChanged()
{
	m_changeLevel = false;
}

void	Progress::setCheckpointCountMax(std::size_t count)
{
	m_checkpointCountMax = count;
}

std::size_t Progress::getCheckpointCountMax(void)
{
	return m_checkpointCountMax;
}

std::size_t	Progress::getCheckpointCount(void)
{
	std::size_t count = 0u;

	for (std::size_t i = 0u; i < m_checkpointCountMax; i++)
	{
		if (isCheckpointValidated(i))
			count++;
	}
	return count;
}

void	Progress::resetCheckpoint(std::size_t id)
{
	if (m_data.respawnType == Progress::RespawnType::Portal && id == 0u)
		m_data.checkpoints = 0u;
}

bool	Progress::isCheckpointValidated(std::size_t id)
{
	std::size_t mask = 1 << id;
	std::size_t value = m_data.checkpoints & mask;
	return value > 0;
}

void	Progress::validateCheckpoint(std::size_t id)
{
	m_data.checkpoints = m_data.checkpoints | (1 << id);
}

void	Progress::registerDeath(sf::Vector2f const & position)
{
	sf::Vector2i const & pos = sf::Vector2i(static_cast<int>(position.x), static_cast<int>(position.y + 1u));
	m_deaths[m_data.currentDestination].insert(m_deaths[m_data.currentDestination].begin(), pos);
	if (m_deaths[m_data.currentDestination].size() > Progress::DeathMax)
		m_deaths[m_data.currentDestination].pop_back();
	m_data.deathCount += 1u;
	m_deathsLevelCount = m_deaths[m_data.currentDestination].size();
	save();
}

std::vector<sf::Vector2i> & Progress::getDeathPos()
{
	return m_deaths[m_data.currentDestination];
}

std::size_t	Progress::getDeathCount()
{
	return m_data.deathCount;
}

std::size_t Progress::getDeathLevelCount()
{
	return m_deathsLevelCount;
}

void Progress::resetDeathLevel(void)
{
	m_deathsLevelCount = 0u;
	return m_deaths[m_data.currentDestination].clear();
}

void	Progress::saveDeaths()
{
	std::string saveDeaths;
	for (auto itLevel = m_deaths.begin(); itLevel != m_deaths.end(); itLevel++)
	{
		saveDeaths += std::to_string(static_cast<int>(itLevel->first)) + " ";
		for (std::size_t i = 0u; i < itLevel->second.size(); i++)
		{
			saveDeaths += std::to_string(itLevel->second[i].x);
			saveDeaths += " " + std::to_string(itLevel->second[i].y) + " ";
		}
		saveDeaths += "\n";
	}
	assert(saveDeaths.size() < 20000);
	std::strcpy(m_data.deaths, saveDeaths.c_str());
}

void	Progress::loadDeaths()
{
	std::istringstream savedDeaths(m_data.deaths);
	std::string line;
	while (std::getline(savedDeaths, line))
	{
		std::vector<std::string> splitLine;
		split(line, ' ', splitLine);

        if (splitLine.size() && std::all_of(splitLine[0].begin(), splitLine[0].end(), ::isdigit))
        {
            Level level = static_cast<Level>(std::stoi(splitLine[0]));
            m_deaths[level].clear();
            for (std::size_t i = 1; i < splitLine.size(); i += 2)
            {
                m_deaths[level].push_back(sf::Vector2i(std::stoi(splitLine[i]), std::stoi(splitLine[i + 1])));
            }
        }
	}
}

void	Progress::registerPortal(Level destination)
{
	m_portals[m_data.currentDestination].insert(std::make_pair(destination, false));
}

bool	Progress::meetPortal(Level destination)
{
	if (m_changeLevel == false && !m_portals[m_data.currentDestination][destination])
	{
		m_portals[m_data.currentDestination][destination] = true;
		return true;
	}
	return false;
}

bool	Progress::meetPortal(Level source, Level destination)
{
	if (m_changeLevel == false && !m_portals[source][destination])
	{
		m_portals[source][destination] = true;
		return true;
	}
	return false;
}

std::size_t Progress::countRandomDiscover(void)
{
	if (m_countRandomDiscover > RandomPortalMax)
		m_countRandomDiscover = RandomPortalMax;
	if (m_countRandomDiscover != 0u)
		return m_countRandomDiscover;
	std::size_t count = 0u;
	for (auto level : m_portals)
	{
		for (auto portal : level.second)
		{
			if (portal.first == Level::Random && portal.second == true)
				count++;
		}
	}
	if (count > RandomPortalMax)
		count = RandomPortalMax;
	return count;
}

bool	Progress::isMetPortal(Level destination)
{
	if (m_changeLevel == false && m_portals[m_data.currentDestination][destination])
		return true;
	return false;
}

bool	Progress::isMetRandom(Level destination)
{
	if (m_changeLevel == false && m_portals[destination][Level::Random])
		return true;
	return false;
}

void	Progress::setPortalPosition(Level destination, sf::Vector2f const & position)
{
	if (!isMetPortal(destination) && destination != m_data.lastDestination)
		m_portalsToDiscover[destination] = position;
	else
		m_portalsToDiscover.erase(destination);
}

void	Progress::removePortalPosition(Level destination)
{
	m_portalsToDiscover.erase(destination);
}

sf::Vector2f Progress::getInterestPoint(void)
{
	if (m_data.currentDestination == Level::Final)
		return m_portalsToDiscover[Level::Blue];
	for (auto portal : m_portalsToDiscover)
	{
		if (portal.first == Level::Random)
			return (portal.second);
	}
	return sf::Vector2f(0.f, 0.f);
}

void	Progress::savePortals()
{
	std::string savePortals;
	for (auto itLevel = m_portals.begin(); itLevel != m_portals.end(); itLevel++)
	{
		savePortals += std::to_string(static_cast<int>(itLevel->first)) + " ";
		for (auto it = itLevel->second.begin(); it != itLevel->second.end(); it++)
		{
			savePortals += std::to_string(static_cast<std::size_t>(it->first));
			savePortals += " " + std::to_string(it->second) + " ";
		}
		savePortals += "\n";
	}
	assert(savePortals.size() < 10000);
	std::strcpy(m_data.portals, savePortals.c_str());
}

void	Progress::loadPortals()
{
	std::istringstream savedPortals(m_data.portals);
	std::string line;
	while (std::getline(savedPortals, line))
	{
		std::vector<std::string> splitLine;
		split(line, ' ', splitLine);

        if (splitLine.size() && std::all_of(splitLine[0].begin(), splitLine[0].end(), ::isdigit))
        {
            Level level = static_cast<Level>(std::stoi(splitLine[0]));
            m_portals[level].clear();

            for (std::size_t i = 1; i < splitLine.size(); i += 2)
            {
                if (splitLine[i + 1] == "1")
                    m_portals[level].insert(std::make_pair(static_cast<Level>(std::stoi(splitLine[i])), true));
                else
                    m_portals[level].insert(std::make_pair(static_cast<Level>(std::stoi(splitLine[i])), false));
            }
        }
	}
	m_portalsToDiscover.clear();
}


void	Progress::registerNpc(GameObjectType key)
{
	if (!m_isMenu && !m_npc[m_data.currentDestination].insert(std::make_pair(key, false)).second)
		m_npcMax++;
}

bool	Progress::meetNpc(GameObjectType key)
{
	if (!m_isMenu && m_changeLevel == false && !m_npc[m_data.currentDestination][key])
	{
		m_npc[m_data.currentDestination][key] = true;
		return true;
	}
	return false;
}

void	Progress::saveNpc()
{
	std::string saveNpc;
	for (auto itLevel = m_npc.begin(); itLevel != m_npc.end(); itLevel++)
	{
		saveNpc += std::to_string(static_cast<int>(itLevel->first)) + " ";
		for (auto it = itLevel->second.begin(); it != itLevel->second.end(); it++)
		{
			saveNpc += std::to_string(static_cast<std::size_t>(it->first));
			saveNpc += " " + std::to_string(it->second) + " ";
		}
		saveNpc += "\n";
	}
	assert(saveNpc.size() < 10000);
	std::strcpy(m_data.npc, saveNpc.c_str());
	m_data.npcCount = getNpcMet().size();
}

void	Progress::loadNpc()
{
	std::istringstream savedNpc(m_data.npc);
	std::string line;
	while (std::getline(savedNpc, line))
	{
		std::vector<std::string> splitLine;
		split(line, ' ', splitLine);

        if (splitLine.size() && std::all_of(splitLine[0].begin(), splitLine[0].end(), ::isdigit))
        {
            Level level = static_cast<Level>(std::stoi(splitLine[0]));
            m_npc[level].clear();

            for (std::size_t i = 1; i < splitLine.size(); i += 2)
            {
                if (splitLine[i + 1] == "1")
                    m_npc[level].insert(std::make_pair(static_cast<GameObjectType>(std::stoul(splitLine[i])), true));
                else
                    m_npc[level].insert(std::make_pair(static_cast<GameObjectType>(std::stoul(splitLine[i])), false));
            }
        }
	}
}

std::size_t	Progress::getNpcCount()
{
	return getNpcMet().size();
}

std::size_t	Progress::getNpcMax()
{
	m_npcMax = m_npc[m_data.lastDestination].size();
	return m_npcMax;
}

std::list<GameObjectType>	Progress::getNpcMet()
{
	std::list<GameObjectType> npcList;

	for (auto level = m_npc.begin(); level != m_npc.end(); level++)
	{
		for (auto npc = level->second.begin(); npc != level->second.end(); npc++)
		{
			if (npc->second == true)
				npcList.push_back(npc->first);
		}
	}
	npcList.sort();
	npcList.unique();

	return std::move(npcList);
}

void		Progress::setIsOctoOnInstance(bool isInstance)
{
	m_isOctoOnInstance = isInstance;
}

bool		Progress::isOctoOnInstance(void)
{
	return m_isOctoOnInstance;
}

void		Progress::setRespawnType(Progress::RespawnType type)
{
	m_data.respawnType = type;
}

Progress::RespawnType	Progress::getRespawnType(void) const
{
	return m_data.respawnType;
}

std::size_t	Progress::getActivatedMonolith(void) const
{
	return m_data.activatedMonolith;
}

void		Progress::setActivatedMonolith(std::size_t count)
{
	m_data.activatedMonolith = count;
}

void		Progress::setLevelOfDetails(int levelOfDetails)
{
	m_data.levelOfDetails = levelOfDetails;
}

int			Progress::getLevelOfDetails(void) const
{
	return m_data.levelOfDetails;
}

void		Progress::setCheckPointPosition(sf::Vector2f const & position)
{
	m_data.checkPointPosition = position;
}

sf::Vector2f const & Progress::getCheckPointPosition(void) const
{
	return m_data.checkPointPosition;
}

void		Progress::setInCloud(bool inCloud, std::size_t cloudId)
{
	if (inCloud && m_cloudId == 0u)
	{
		m_isInCloud = true;
		m_cloudId = cloudId;
	}
	if (!inCloud && cloudId == m_cloudId)
	{
		m_isInCloud = false;
		m_cloudId = 0u;
	}
}

void		Progress::setMapHighlight(bool isHighlight)
{
	m_isHighLight = isHighlight;
}

bool		Progress::isMapHighlight(void) const
{
	return m_isHighLight;
}

std::map<std::string, octo::Array3D<Tile>> & Progress::getMapsTile(void)
{
	return m_mapsTile;
}

std::map<std::string, octo::Array3D<float>> & Progress::getMapsHighlight(void)
{
	return m_mapsHighlight;
}

void	Progress::split(std::string const & s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		elems.push_back(item);
}

void	Progress::registerLevel(Level const & level)
{
	for (auto it = m_levels.begin(); it != m_levels.end(); it++)
	{
		if (*it == level)
			return;
	}
	m_levels.push_back(level);
}

std::vector<Level> const & Progress::getRegisteredLevels(void) const
{
	return (m_levels);
}

void Progress::setBalleMultiplier(float multiplier)
{
	m_balleMultiplier = multiplier;
}

float Progress::getBalleMultiplier(void)
{
	return m_balleMultiplier;
}

std::size_t Progress::getProgression(void)
{
	std::size_t progression = 0u;
	if (m_data.currentDestination == Level::Random)
		progression = static_cast<std::size_t>(m_data.lastDestination);
	else
		progression = static_cast<std::size_t>(m_data.currentDestination);

	if (progression >= static_cast<std::size_t>(Level::Portal))
		progression = static_cast<std::size_t>(Level::Portal);

	return progression;
}
