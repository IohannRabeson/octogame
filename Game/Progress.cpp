#include "Progress.hpp"
#include "CharacterOcto.hpp"
#include <Application.hpp>
#include <console.hpp>

std::unique_ptr<Progress> Progress::m_instance = nullptr;

Progress::Progress() :
	m_filename("qwer.osv"),
	m_action(false),
	m_walk(false),
	m_jump(false),
	m_doubleJump(false),
	m_slowFall(false),
	m_elevator(false)
{
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

void	Progress::load(std::string const &filename)
{
	m_filename = filename;
}

void	Progress::save()
{
	m_data.octoPos = m_octo->getPhysicsPosition();
}

void	Progress::setCharacterOcto(CharacterOcto * octo)
{
	m_octo = octo;
}

sf::Vector2f	Progress::getOctoPos()
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
