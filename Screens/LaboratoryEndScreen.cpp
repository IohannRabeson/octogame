#include "LaboratoryEndScreen.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"
#include "ScientistJu.hpp"
#include "ScientistLu.hpp"
#include "ScientistFran.hpp"
#include "ScientistCedric.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>

#include <Console.hpp>
LaboratoryEndScreen::LaboratoryEndScreen(void) :
	m_state(Appear),
	m_timer(sf::Time::Zero),
	m_timeBeforeNextText(sf::seconds(1.f)),
	m_appearDuration(sf::seconds(2.f)),
	m_textIndex(0u),
	m_lastTextIndex(0u)
{
	octo::Application::getConsole().addCommand(L"lu", [this](sf::Vector2f const & p)
			{m_npcs[1]->setPosition(sf::Vector2f(p.x, p.y));});
}

void	LaboratoryEndScreen::start()
{
	InputListener::addInputListener();
	Progress::getInstance().setBubbleNpc(true);

	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::Camera & camera = octo::Application::getCamera();

	camera.setCenter(camera.getRectangle().width / 2.f, camera.getRectangle().height / 2.f);
	m_background.setTexture(resources.getTexture(LABO_BG_PNG));
	m_background.setOrigin(m_background.getLocalBounds().width / 2.f, m_background.getLocalBounds().height / 2.f);
	m_background.setPosition(octo::Application::getCamera().getCenter());
	m_foreground.setTexture(resources.getTexture(LABO_FG_PNG));
	m_foreground.setOrigin(m_background.getLocalBounds().width / 2.f, m_background.getLocalBounds().height / 2.f);
	m_foreground.setPosition(octo::Application::getCamera().getCenter());
	m_water.setTexture(resources.getTexture(LABO_WATER_PNG));
	m_water.setOrigin(m_background.getLocalBounds().width / 2.f, m_background.getLocalBounds().height / 2.f);
	m_water.setPosition(octo::Application::getCamera().getCenter());

	m_npcs.emplace_back(new ScientistJu());
	m_npcs.emplace_back(new ScientistLu());
	m_npcs.emplace_back(new ScientistFran());
	m_npcs.emplace_back(new ScientistCedric());
	m_npcs[0]->setPosition(sf::Vector2f(200.f, 883.f));
	m_npcs[1]->setPosition(sf::Vector2f(950.f, 883.f));
	m_npcs[2]->setPosition(sf::Vector2f(1600.f, 847.f));
	m_npcs[3]->setPosition(sf::Vector2f(350.f, 577.f));

	for (auto & it : m_npcs)
		it->setDisplayText(false);

	for (auto & it : m_npcs)
		m_lastTextIndex = std::max(m_lastTextIndex, it->getLastIndex());
}

void	LaboratoryEndScreen::pause()
{
}

void	LaboratoryEndScreen::resume()
{
}

void	LaboratoryEndScreen::stop()
{
	InputListener::removeInputListener();
	Progress::getInstance().setBubbleNpc(false);
}

void	LaboratoryEndScreen::update(sf::Time frameTime)
{
	switch (m_state)
	{
		case Appear:
			m_timer += frameTime;
			if (m_timer > m_appearDuration)
			{
				m_timer = sf::Time::Zero;
				m_state = State::Dialogs;
				for (auto & it : m_npcs)
				{
					it->setDisplayText(true);
					it->updateText(true);
				}
			}
			break;
		case Dialogs:
			m_timer += frameTime;
			if (m_timer >= m_timeBeforeNextText)
			{
				m_timer = sf::Time::Zero;
				m_textIndex++;
				if (m_textIndex > m_lastTextIndex)
				{
					for (auto & it : m_npcs)
						it->setDisplayText(false);
					m_state = State::CedricPutPotion;
				}
			}
			for (auto & it : m_npcs)
				it->setTextIndex(m_textIndex);
			break;
		default:
			break;
	}

	for (auto & it : m_npcs)
		it->update(frameTime);
}

void	LaboratoryEndScreen::draw(sf::RenderTarget & render) const
{
	sf::RenderStates states;
	render.clear(sf::Color::Black);
	render.draw(m_background);
	render.draw(m_water);
	for (auto & it : m_npcs)
		render.draw(*it);
	render.draw(m_foreground);
	for (auto & it : m_npcs)
		it->drawText(render, states);
}

bool	LaboratoryEndScreen::onInputPressed(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Menu:
			break;
		default:
			break;
	}
	return true;
}

bool	LaboratoryEndScreen::onInputReleased(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		default:
			break;
	}
	return true;
}
