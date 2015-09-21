#include "Progress.hpp"
#include "CharacterOcto.hpp"
#include "Tile.hpp"
#include "ABiome.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <Console.hpp>
#include <iostream>
#include <fstream>

std::unique_ptr<Progress> Progress::m_instance = nullptr;

Progress::Progress() :
	m_newSave(false),
	m_changeLevel(false)
{
	setup();
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
	m_filename = "save.osv";
	m_data = data();
}

void	Progress::load(std::string const &filename)
{
	std::ifstream filestream(filename, std::ios::in | std::ios::binary);
	if(!filestream)
	{
		m_newSave = true;
		return;
	}
	m_filename = filename;
	filestream.read(reinterpret_cast<char*>(&m_data), sizeof(struct data));
	filestream.close();
	init();
}

void	Progress::init()
{
}

void	Progress::save()
{
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
	setup();
}

void	Progress::setNextDestination(Level destination)
{
	m_data.destination = destination;
	m_changeLevel = true;
}

Level	Progress::getNextDestination(void) const
{
	return m_data.destination;
}

bool	Progress::canMoveMap()
{
	return (m_data.nanoRobotCount > 0);
}

bool	Progress::canRepair()
{
	return (m_data.nanoRobotCount > 1);
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

bool	Progress::changeLevel() const
{
	return m_changeLevel;
}

void	Progress::levelChanged()
{
	m_changeLevel = false;
}
