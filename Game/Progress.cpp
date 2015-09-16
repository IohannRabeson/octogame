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

Progress::Progress()
{
	setup();
	octo::Console&				console = octo::Application::getConsole();

	console.addCommand(L"octo.setCanUseAction", [this](bool b)
			{
			setCanUseAction(b);
			});
	console.addCommand(L"octo.setCanWalk", [this](bool b)
			{
			setCanWalk(b);
			});
	console.addCommand(L"octo.setCanJump", [this](bool b)
			{
			setCanJump(b);
			});
	console.addCommand(L"octo.setCanDoubleJump", [this](bool b)
			{
			setCanDoubleJump(b);
			});
	console.addCommand(L"octo.setCanSlowFall", [this](bool b)
			{
			setCanSlowFall(b);
			});
	console.addCommand(L"octo.setCanUseElevator", [this](bool b)
			{
			setCanUseElevator(b);
			});
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
	m_action = false;
	m_walk = false;
	m_jump = false;
	m_doubleJump = false;
	m_slowFall = false;
	m_elevator = false;
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
	//TODO
	std::cout << "name:" << m_data.biomeName << std::endl;
	//std::cout << "robot:" << m_data.nanoRobotCount << std::endl;
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

void	Progress::setupInfoLevel(ABiome & biome)
{
	m_data.biomeName = biome.getName();
}

bool	Progress::canUseAction()
{
	return m_action;
}

bool	Progress::canWalk()
{
	return m_walk;
}

bool	Progress::canJump()
{
	return m_jump;
}

bool	Progress::canDoubleJump()
{
	return m_doubleJump;
}

bool	Progress::canSlowFall()
{
	return m_slowFall;
}

bool	Progress::canUseElevator()
{
	return m_elevator;
}
