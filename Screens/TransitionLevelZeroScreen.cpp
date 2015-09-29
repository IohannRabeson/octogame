#include "TransitionLevelZeroScreen.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <StateManager.hpp>
#include <ResourceManager.hpp>

#include <sstream>
#include <cwchar>

TransitionLevelZeroScreen::TransitionLevelZeroScreen() :
	m_bubbleCount(5u),
	m_bubble(new BubbleText[m_bubbleCount]),
	m_time(sf::Time::Zero),
	m_index(0u)
{
	octo::Camera&			camera = octo::Application::getCamera();
	octo::ResourceManager & resources = octo::Application::getResourceManager();
	
	camera.setCenter(0.f, 0.f);
	m_timerMax.resize(m_bubbleCount);
	m_timerMax[0] = sf::seconds(4.5f);
	m_timerMax[1] = sf::seconds(5.f);
	m_timerMax[2] = sf::seconds(6.f);
	m_timerMax[3] = sf::seconds(7.f);
	m_timerMax[4] = sf::seconds(6.f);

	std::wstringstream f(resources.getText(DIALOGS_TXT).toWideString());
	std::wstring wkey;
	std::wstring line;
	wchar_t delim = '=';
	std::size_t i = 0;
	while (std::getline(f, wkey, delim))
	{
		std::getline(f, line);
		std::string key(wkey.begin(), wkey.end());
		if (!key.compare("transitionLevelZero"))
		{
			m_bubble[i].setup(line, sf::Color::White, 28u, 700.f);
			m_bubble[i].setPosition(camera.getCenter());
			m_bubble[i].setType(ABubble::Type::None);
			m_bubble[i].setActive(true);
			i++;
		}
	}
	m_bubble[m_index].setType(ABubble::Type::Speak);
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
	if (m_index >= m_bubbleCount)
		return;
	octo::StateManager & states = octo::Application::getStateManager();
	m_time += frameTime;
	for (std::size_t i = 0; i < m_bubbleCount; i++)
		m_bubble[i].setType(ABubble::Type::None);

	m_bubble[m_index].setType(ABubble::Type::Speak);
	m_bubble[m_index].update(frameTime);
	if (m_time > m_timerMax[m_index])
	{
		m_time = sf::Time::Zero;
		m_index++;
	}
	if (m_index >= m_bubbleCount)
		states.push("game");
}

void	TransitionLevelZeroScreen::draw(sf::RenderTarget& render)const
{
	if (m_index >= m_bubbleCount)
		return;
	render.clear();
	if (m_index < m_bubbleCount)
		m_bubble[m_index].draw(render);
}
