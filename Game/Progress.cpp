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
	m_mapSize = sf::Vector2u(0u, 0u);
	m_octo = nullptr;
	m_map = nullptr;
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
	/*std::cout << "name:" << m_data.biomeName << std::endl;
	std::cout << "robot:" << m_data.nanoRobotCount << std::endl;
	std::cout << "octo:" << m_data.octoPos.x << "|" << m_data.octoPos.y << std::endl;
	std::cout << "camera:" << m_data.cameraPos.x << "|" << m_data.cameraPos.y << std::endl;
	std::cout << "depth:" << m_data.depthMap << std::endl;*/
}

void	Progress::save()
{
	m_data.octoPos = m_octo->getPhysicsPosition();
	m_data.cameraPos = octo::Application::getCamera().getCenter();
	if (m_data.octoPos.x < 0){
		while (m_data.octoPos.x < 0)
			m_data.octoPos.x += m_mapSize.x * Tile::TileSize;
	}
	else
	{
		while (m_data.octoPos.x > m_mapSize.x * Tile::TileSize)
			m_data.octoPos.x -= m_mapSize.x * Tile::TileSize;
	}
	if (m_data.cameraPos.x < 0){
		while (m_data.cameraPos.x < 0)
			m_data.cameraPos.x += m_mapSize.x * Tile::TileSize;
	}
	else
	{
		while (m_data.cameraPos.x > (m_mapSize.x * Tile::TileSize))
			m_data.cameraPos.x -= m_mapSize.x * Tile::TileSize;
	}
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

void	Progress::setupInfoLevel(ABiome & biome, sf::Vector2f octoPos)
{
	if (m_data.biomeName != biome.getName() || m_newSave)
	{
		m_data.octoPos = octoPos;
		m_data.cameraPos = octoPos;
	}
	m_data.biomeName = biome.getName();
	m_mapSize = biome.getMapSize();
}

void	Progress::setCharacterOcto(CharacterOcto * octo)
{
	m_octo = octo;
}
void	Progress::setGroundManager(GroundManager * manager)
{
	m_map = manager;
}

sf::Vector2f const&	Progress::getOctoPos()
{
	return m_data.octoPos;
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
