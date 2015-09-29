#include "TransitionLevelZeroScreen.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <StateManager.hpp>
#include <ResourceManager.hpp>

#include <sstream>
#include <cwchar>

TransitionLevelZeroScreen::TransitionLevelZeroScreen() :
	m_time(sf::Time::Zero)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	std::wstringstream f(resources.getText(DIALOGS_TXT).toWideString());
	std::wstring wkey;
	std::wstring line;
	wchar_t delim = '=';
	while (std::getline(f, wkey, delim))
	{
		std::getline(f, line);
		std::string key(wkey.begin(), wkey.end());
		if (key.compare("transitionLevelZero"))
		{
			bubble.setup(line, sf::Color::White);
			break;
		}
	}
}

void	TransitionLevelZeroScreen::start()
{
}

void	TransitionLevelZeroScreen::pause()
{
}

void	TransitionLevelZeroScreen::resume()
{
}

void	TransitionLevelZeroScreen::stop()
{
}

void	TransitionLevelZeroScreen::update(sf::Time frameTime)
{
	octo::StateManager & states = octo::Application::getStateManager();
	m_time += frameTime;
	bubble.updateContent(frameTime, sf::Vector2f(0.f, 0.f));
	if (m_time > sf::seconds(2.f))
		states.push("game");
}

void	TransitionLevelZeroScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::Green);
	bubble.drawContent(render, sf::RenderStates());
}
