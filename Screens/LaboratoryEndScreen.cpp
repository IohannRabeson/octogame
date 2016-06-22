#include "LaboratoryEndScreen.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>

LaboratoryEndScreen::LaboratoryEndScreen(void)
{
}

void	LaboratoryEndScreen::start()
{
//	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	m_startTexture = resources.getTexture(LOGO_01_PNG);

	m_startSprite.setTexture(m_startTexture);
	m_startSprite.setOrigin(m_startSprite.getLocalBounds().width / 2.f, m_startSprite.getLocalBounds().height / 2.f);
	m_startSprite.setPosition(octo::Application::getCamera().getCenter());
	InputListener::addInputListener();
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
}

void	LaboratoryEndScreen::update(sf::Time frameTime)
{
	(void)frameTime;
}

void	LaboratoryEndScreen::draw(sf::RenderTarget & render) const
{
	sf::RenderStates states;
	render.clear(sf::Color::Black);
	render.draw(m_startSprite);
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
