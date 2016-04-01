#include "TransitionLevelZeroScreen.hpp"

#include "Progress.hpp"

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <StateManager.hpp>
#include <ResourceManager.hpp>
#include <AudioManager.hpp>
#include "ResourceDefinitions.hpp"

#include <sstream>
#include <cwchar>

TransitionLevelZeroScreen::TransitionLevelZeroScreen() :
	m_bubbleCount(5u),
	m_bubble(new BubbleText[m_bubbleCount]),
	m_time(sf::Time::Zero),
	m_index(0u),
	m_startTimerMax(sf::seconds(3.f)),
	m_soundPlayed1(false),
	m_soundPlayed2(false)
{
	octo::Camera&			camera = octo::Application::getCamera();
	octo::ResourceManager &	resources = octo::Application::getResourceManager();
	camera.setCenter(0.f, 0.f);
	m_timerMax.resize(m_bubbleCount);
	m_timerMax[0] = sf::seconds(5.5f);
	m_timerMax[1] = sf::seconds(6.f);
	m_timerMax[2] = sf::seconds(7.f);
	m_timerMax[3] = sf::seconds(8.f);
	m_timerMax[4] = sf::seconds(9.f);

	std::wstringstream f(resources.getText(Progress::getInstance().getTextFile()).toWideString());
	std::wstring wkey;
	std::wstring line;
	std::string keyName;
	if (Progress::getInstance().getNextDestination() != Level::Default)
		keyName = "transitionLevelZero";
	else
		keyName = "transitionLevelZeroToRandom";
	wchar_t delim = '=';
	std::size_t i = 0;
	while (std::getline(f, wkey, delim))
	{
		std::getline(f, line);
		std::string key(wkey.begin(), wkey.end());
		if (!key.compare(keyName))
		{
			m_bubble[i].setup(line, sf::Color::White, 28u, 700.f);
			m_bubble[i].setPosition(camera.getCenter());
			m_bubble[i].setType(ABubble::Type::None);
			m_bubble[i].setActive(true);
			i++;
		}
	}

	m_sprite.setSpriteSheet(resources.getSpriteSheet(NANO_GROUND_TRANSFORM_OSS));
	m_sprite.setScale(1.f, 1.f);

	typedef octo::SpriteAnimation::Frame	Frame;
	m_animation.setFrames({	Frame(sf::seconds(0.2f), 0u),
							Frame(sf::seconds(0.2f), 1u),
							Frame(sf::seconds(0.2f), 2u),
							Frame(sf::seconds(0.2f), 3u),
							Frame(sf::seconds(0.2f), 2u),
							Frame(sf::seconds(0.2f), 1u)});
	m_animation.setLoop(octo::LoopMode::Loop);
	m_sprite.setAnimation(m_animation);
	m_sprite.play();
	m_sprite.setPosition(camera.getCenter() + sf::Vector2f(-40.f, -4.f));
}

void	TransitionLevelZeroScreen::start()
{
	octo::GraphicsManager &	graphics = octo::Application::getGraphicsManager();
	graphics.addKeyboardListener(this);
}

void	TransitionLevelZeroScreen::pause()
{
}

void	TransitionLevelZeroScreen::resume()
{
}

void	TransitionLevelZeroScreen::stop()
{
	octo::GraphicsManager &	graphics = octo::Application::getGraphicsManager();
	graphics.removeKeyboardListener(this);
}

void	TransitionLevelZeroScreen::playSound(std::size_t index)
{
	if (m_soundPlayed1 == false && index == 0u)
	{
		octo::Application::getAudioManager().playSound(octo::Application::getResourceManager().getSound(NANO_2_OGG), 0.5f);
		m_soundPlayed1 = true;
	}
	if (m_soundPlayed2 == false && index == 3u)
	{
		octo::Application::getAudioManager().playSound(octo::Application::getResourceManager().getSound(NANO_3_OGG), 0.5f);
		m_soundPlayed2 = true;
	}
}

void	TransitionLevelZeroScreen::update(sf::Time frameTime)
{
	if (m_index >= m_bubbleCount)
	{
		octo::StateManager & states = octo::Application::getStateManager();
		states.change("game");
		return;
	}
	if (m_startTimer <= m_startTimerMax)
	{
		m_startTimer += frameTime;
		return;
	}
	m_time += frameTime;
	for (std::size_t i = 0; i < m_bubbleCount; i++)
		m_bubble[i].setType(ABubble::Type::None);

	m_bubble[m_index].setType(ABubble::Type::Speak);
	m_bubble[m_index].update(frameTime);
	if (m_time > m_timerMax[m_index])
	{
		playSound(m_index);
		m_time = sf::Time::Zero;
		m_index++;
		playSound(m_index);
	}

	m_sprite.update(frameTime);
}

bool TransitionLevelZeroScreen::onPressed(sf::Event::KeyEvent const &event)
{
	switch (event.code)
	{
		case sf::Keyboard::Return:
		case sf::Keyboard::Space:
		case sf::Keyboard::Escape:
			{
				m_index++;
				m_time = sf::Time::Zero;
				break;
			}
		default:
			break;
	}
	return (true);
}

void	TransitionLevelZeroScreen::draw(sf::RenderTarget& render)const
{
	if (m_index >= m_bubbleCount)
		return;
	render.clear();

	if (m_startTimer >= m_startTimerMax)
		render.draw(m_sprite);
	if (m_index < m_bubbleCount)
		m_bubble[m_index].draw(render);
}
