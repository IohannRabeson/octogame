#include "Progress.hpp"
#include "CharacterOcto.hpp"
#include "Tile.hpp"
#include "ABiome.hpp"
#include <Application.hpp>
#include <Options.hpp>
#include <AudioManager.hpp>
#include <GraphicsManager.hpp>

#include <fstream>
#include <stdlib.h>

std::unique_ptr<Progress> Progress::m_instance = nullptr;

Progress::Progress() :
	m_filename(octo::Application::getOptions().getValue<std::string>("path") + "save.osv"),
	m_newSave(false),
	m_changeLevel(false),
	m_reverseSprite(false),
	m_validChallenge(false),
	m_spaceShipRepair(false),
	m_npcCount(0u),
	m_npcMax(0u)
{
/*
#ifndef NDEBUG
	m_data.nanoRobotCount = octo::Application::getOptions().getValue<std::size_t>("nb_nano"); // TODO : remove from defaultsetup();
	m_data.nextDestination = static_cast<Level>(octo::Application::getOptions().getValue<std::size_t>("level")); // TODO : remove from defaultsetup();
#endif
*/
}

Progress & Progress::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new Progress());
	}
	return *m_instance;
}

void	Progress::setup()
{
	m_newSave = false;
	m_filename = octo::Application::getOptions().getValue<std::string>("path") + "save.osv";
	m_data = data();
}

void	Progress::load(std::string const &filename)
{
	m_filename = octo::Application::getOptions().getValue<std::string>("path") + filename;
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
	graphics.setFullscreen(m_data.fullscreen);
	graphics.setVerticalSyncEnabled(m_data.vsync);
	m_validChallenge = false;
	loadNpc();
}

void	Progress::save()
{
	//octo::AudioManager & audio = octo::Application::getAudioManager();
	octo::GraphicsManager & graphics = octo::Application::getGraphicsManager();

	//m_data.musicVol = audio.getMusicVolume();
	//m_data.soundVol = audio.getSoundVolume();
	m_data.fullscreen = graphics.isFullscreen();
	m_data.vsync = graphics.isVerticalSyncEnabled();

	saveNpc();
	saveToFile();
}

void	Progress::saveToFile()
{
	std::ofstream filestream(m_filename, std::ios::out | std::ios::binary);
	if(!filestream)
		return;
	filestream.write(reinterpret_cast<char*>(&m_data), sizeof(struct data));
	filestream.close();
}

void	Progress::reset()
{
	m_changeLevel = false;
	m_reverseSprite = false;
	m_validChallenge = false;
	m_spaceShipRepair = false;
	m_npc.clear();
	setup();
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

ResourceKey Progress::getTextFile(void) const
{
	if (m_data.language == Language::en_keyboard)
		return DIALOGS_EN_KEYBOARD_TXT;
	return DIALOGS_FR_KEYBOARD_TXT;
}

void	Progress::addNanoRobot()
{
	m_data.nanoRobotCount++;
	assert(m_data.nanoRobotCount <= 7);
	save();
}

void	Progress::setNanoRobotCount(std::size_t count)
{
	m_data.nanoRobotCount = count;
	assert(m_data.nanoRobotCount <= 7);
	save();
}

void	Progress::setNextDestination(Level const & destination, bool hasTransition)
{
	m_changeLevel = hasTransition;
	m_data.nextDestination = destination;
}

Level	Progress::getNextDestination(void) const
{
	return m_data.nextDestination;
}

void	Progress::setLastDestination(Level destination)
{
	if (destination != Level::Default)
		m_data.lastDestination = destination;
}

Level	Progress::getLastDestination(void) const
{
	return m_data.lastDestination;
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
	return (m_data.nanoRobotCount > 4);
}

bool	Progress::canUseElevator()
{
	return (m_data.nanoRobotCount > 1);
}

bool	Progress::canUseWaterJump()
{
	return (m_data.nanoRobotCount > 5);
}

bool	Progress::changeLevel() const
{
	return m_changeLevel;
}

void	Progress::levelChanged()
{
	m_changeLevel = false;
}

void	Progress::registerNpc(ResourceKey const & key)
{
	if (!m_npc[m_data.nextDestination].insert(std::make_pair(key, false)).second)
		m_npcMax++;
}

bool	Progress::meetNpc(ResourceKey const & key)
{
	if (m_changeLevel == false && !m_npc[m_data.nextDestination][key])
	{
		m_npc[m_data.nextDestination][key] = true;
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
			saveNpc += static_cast<std::string>(it->first);
			saveNpc += " " + std::to_string(it->second) + " ";
		}
		saveNpc += "\n";
	}
	assert(saveNpc.size() < 10000);
	std::strcpy(m_data.npc, saveNpc.c_str());
}

void	Progress::split(std::string const & s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		elems.push_back(item);
}

void	Progress::loadNpc()
{
	std::istringstream savedNpc(m_data.npc);
	std::string line;
	while (std::getline(savedNpc, line))
	{
		std::vector<std::string> splitLine;
		split(line, ' ', splitLine);

		Level level = static_cast<Level>(stoi(splitLine[0]));
		m_npc[level].clear();

		for (std::size_t i = 1; i < splitLine.size(); i += 2)
		{
			if (splitLine[i + 1] == "1")
				m_npc[level].insert(std::make_pair(splitLine[i], true));
			else
				m_npc[level].insert(std::make_pair(splitLine[i], false));
		}
	}
}

std::size_t	Progress::getNpcCount()
{
	m_npcCount = 0u;
	for (auto it = m_npc[m_data.lastDestination].begin(); it != m_npc[m_data.lastDestination].end(); it++)
	{
		if (it->second)
			m_npcCount++;
	}
	return m_npcCount;
}

std::size_t	Progress::getNpcMax()
{
	m_npcMax = m_npc[m_data.lastDestination].size();
	return m_npcMax;
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
