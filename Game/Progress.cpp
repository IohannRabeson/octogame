#include "Progress.hpp"
#include <Application.hpp>
#include <console.hpp>

std::unique_ptr<Progress> Progress::m_instance = nullptr;

Progress::Progress()
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

void	Progress::load(std::string filename)
{
	m_filename = filename;
}

void	Progress::save()
{
}

bool	Progress::canUseAction()
{
	return m_data.action;
}

bool	Progress::canWalk()
{
	return m_data.walk;
}

bool	Progress::canJump()
{
	return m_data.jump;
}

bool	Progress::canDoubleJump()
{
	return m_data.doubleJump;
}

bool	Progress::canSlowFall()
{
	return m_data.slowFall;
}

bool	Progress::canUseElevator()
{
	return m_data.elevator;
}
