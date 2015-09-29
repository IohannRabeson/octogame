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
	octo::Camera&			camera = octo::Application::getCamera();
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	std::wstringstream f(resources.getText(DIALOGS_TXT).toWideString());
	std::wstring wkey;
	std::wstring line;
	wchar_t delim = '=';
	camera.setCenter(0.f, 0.f);
	while (std::getline(f, wkey, delim))
	{
		std::getline(f, line);
		std::string key(wkey.begin(), wkey.end());
		if (!key.compare("transitionLevelZero"))
		{
			bubble.setPosition(camera.getCenter());
			bubble.setType(ABubble::Type::Speak);
			bubble.setActive(true);
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
	bubble.update(frameTime);
	if (m_time > sf::seconds(2.f))
		states.push("game");
}

void	TransitionLevelZeroScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	bubble.draw(render);
}
