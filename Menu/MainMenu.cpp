#include "MainMenu.hpp"
#include "ControlMenu.hpp"
#include "CreditMenu.hpp"
#include "ResetMenu.hpp"
#include "YesNoMenu.hpp"
#include "DifficultyMenu.hpp"
#include "Progress.hpp"
#include "ResourceDefinitions.hpp"
#include <AudioManager.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Application.hpp>

//Sub Menu
class YesNoQuit : public YesNoMenu
{
	inline void setIndex(void) { setIndexCursor(0); }
	inline void actionYes(void)
	{
		Progress &				progress = Progress::getInstance();
		octo::StateManager &	states = octo::Application::getStateManager();

		if (!progress.isMenu())
		{
			states.setTransitionDuration(sf::seconds(0.5f), sf::seconds(0.5f));
			states.change("menu");
		}
		else
			octo::Application::stop();
	}
	inline void actionNo(void) { }
};

//MainMenu
MainMenu::MainMenu(void) :
	m_soundPlayed(false)
{
}

void MainMenu::createMenus(void)
{
	Progress &				progress = Progress::getInstance();

	if (progress.isMenu())
	{
		addMenu(AMenu::getText("menu_quit"), std::unique_ptr<YesNoQuit>(new YesNoQuit()));
		addMenu(AMenu::getText("menu_restart"), std::unique_ptr<ResetMenu>(new ResetMenu()));
		addMenu(AMenu::getText("menu_options"), std::unique_ptr<OptionMenu>(new OptionMenu()));
		addMenu(AMenu::getText("menu_new"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
		if (progress.isFirstTime())
			addMenu(AMenu::getText("menu_play"), std::unique_ptr<DifficultyMenu>(new DifficultyMenu()));
		else
			addMenu(AMenu::getText("menu_play"), std::unique_ptr<EmptyMenu>(new EmptyMenu()));
		setCharacterSize(50);
		setBubbleType(ABubble::Type::MainMenu);
		setCursorAtEnd();
	}
	else
	{
		addMenu(AMenu::getText("menu_controls"), std::unique_ptr<ControlMenu>(new ControlMenu()));
		addMenu(AMenu::getText("menu_options"), std::unique_ptr<OptionMenu>(new OptionMenu()));
		addMenu(AMenu::getText("menu_return"), std::unique_ptr<YesNoQuit>(new YesNoQuit()));
		setCharacterSize(30);
		setBubbleType(ABubble::Type::Think);
	}
}

void MainMenu::onSelection(void)
{
	Progress & progress = Progress::getInstance();

	if (progress.isMenu() && getIndexCursor() == 4u && !progress.isFirstTime())
	{
		octo::StateManager &	states = octo::Application::getStateManager();
		octo::AudioManager &		audio = octo::Application::getAudioManager();
		octo::ResourceManager &		resources = octo::Application::getResourceManager();

		if (!m_soundPlayed)
		{
			m_soundPlayed = true;
			audio.playSound(resources.getSound(OCTO_VOICE_PLAY_OGG), 1.f);
		}
		states.setTransitionDuration(sf::seconds(0.5f), sf::seconds(0.5f));
		states.change("transitionLevel");
	}
	else if (progress.isMenu() && getIndexCursor() == 3u && progress.isMenu())
	{
		octo::StateManager &	states = octo::Application::getStateManager();
		states.change("menu");
		states.setTransitionDuration(sf::seconds(0.5f), sf::seconds(0.5f));
	}
	else
		AMenuSelection::onSelection();
}

void MainMenu::setup(void)
{
	AMenuSelection::setup();
	m_filter.setSize(octo::Application::getCamera().getSize() * 1.2f);
	m_filter.setFillColor(sf::Color(0, 0, 0, 50));
	m_progressionBubble.resize(3);

	for (std::size_t i = 0u; i < 3; i++)
	{
		BubbleText * bubble = new BubbleText();
		m_progressionBubble[i] = std::unique_ptr<BubbleText>(bubble);
		m_progressionBubble[i]->setType(ABubble::Type::Speak);
		m_progressionBubble[i]->setActive(true);
	}
}

void MainMenu::update(sf::Time frameTime, sf::Vector2f const & octoBubblePosition)
{
	AMenuSelection::update(frameTime, octoBubblePosition);
	sf::FloatRect const & camera = octo::Application::getCamera().getRectangle();
	m_filter.setPosition(sf::Vector2f(camera.left, camera.top));
	updateNanoRobots(frameTime, octoBubblePosition);
	updateProgression(frameTime);
}

void MainMenu::updateNanoRobots(sf::Time frameTime, sf::Vector2f const & octoBubblePosition)
{
	for (std::size_t i = 0; i < Progress::getInstance().getSpiritCount() + 1u; i++)
	{
		if (!Progress::getInstance().isMenu())
		{
			if (m_spirit.size() != Progress::getInstance().getSpiritCount() + 1u)
			{
				NanoRobot * spirit = new MenuNanoRobot(octoBubblePosition);
				m_spirit.push_back(std::unique_ptr<NanoRobot>(spirit));
			}
			else if (i < m_spirit.size())
			{
				m_spirit[i]->setState(NanoRobot::State::FollowOcto);
				m_spirit[i]->setPosition(octoBubblePosition);
				m_spirit[i]->update(frameTime);
			}
		}
	}
}

void MainMenu::updateProgression(sf::Time frameTime)
{
	Progress & progress = Progress::getInstance();
	std::size_t current = 0u;
	std::size_t max = 0u;
	std::string key = "";

	for (std::size_t i = 0u; i < m_progressionBubble.size(); i++)
	{
		switch (i)
		{
			case 0u:
				current = progress.getProgression();
				max = static_cast<std::size_t>(Level::Final) + 1u;
				key = "progression_level";
				break;
			case 1u:
				current = progress.countRandomDiscover();
				max = Progress::RandomPortalMax;
				key = "progression_portal";
				break;
			case 2u:
				current = progress.getNanoRobotCount();
				max = 7u;
				key = "progression_octobot";
				break;
			default:
				break;
		}
	
		if (current > 1u)
			key += "_plural";
		else
			key += "_singular";
	
		m_progressionString = std::to_wstring(current) + L" " + AMenu::getText("progression_of") + L" " + std::to_wstring(max) + L" " + AMenu::getText(key);
		m_progressionBubble[i]->setup(m_progressionString, sf::Color(255, 255, 255, 50), 16u);
		if (i < m_spirit.size())
			m_progressionBubble[i]->setPosition(m_spirit[i]->getPosition() - sf::Vector2f(10.f, 16.f));
		m_progressionBubble[i]->update(frameTime);
	}
}

void MainMenu::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_filter, states);
	for (std::size_t i = 0u; i < m_spirit.size(); i++)
	{
		if (!Progress::getInstance().isMenu() && m_spirit[i])
			m_spirit[i]->draw(render, states);
	}
	for (std::size_t i = 0u; i < m_progressionBubble.size(); i++)
	{
		if (!Progress::getInstance().isMenu())
			m_progressionBubble[i]->draw(render, states);
	}
	AMenuSelection::draw(render, states);
}
