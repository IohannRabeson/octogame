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
	InputListener::addInputListener();
	octo::ResourceManager &		resources = octo::Application::getResourceManager();
	m_background = resources.getTexture(LOGO_01_PNG);

	octo::Camera & camera = octo::Application::getCamera();
	octo::Application::getCamera().setCenter(camera.getRectangle().width / 2.f, camera.getRectangle().height / 2.f);
	m_backgroundSprite.setTexture(m_background);
	m_backgroundSprite.setOrigin(m_backgroundSprite.getLocalBounds().width / 2.f, m_backgroundSprite.getLocalBounds().height / 2.f);
	m_backgroundSprite.setPosition(octo::Application::getCamera().getCenter());

	m_ju.setPosition(sf::Vector2f(500.f, 500.f));
	m_lu.setPosition(sf::Vector2f(600.f, 500.f));
	m_fran.setPosition(sf::Vector2f(700.f, 500.f));
	m_cedric.setPosition(sf::Vector2f(800.f, 500.f));
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
	m_ju.update(frameTime);
	m_lu.update(frameTime);
	m_fran.update(frameTime);
	m_cedric.update(frameTime);
}

void	LaboratoryEndScreen::draw(sf::RenderTarget & render) const
{
	sf::RenderStates states;
	render.clear(sf::Color::Black);
	render.draw(m_backgroundSprite);
	render.draw(m_ju);
	render.draw(m_lu);
	render.draw(m_fran);
	render.draw(m_cedric);
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
