#include "LaboratoryEndScreen.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>

LaboratoryEndScreen::LaboratoryEndScreen(void) :
	m_state(Appear),
	m_timer(sf::Time::Zero),
	m_timeBeforeNextText(sf::seconds(1.f)),
	m_appearDuration(sf::seconds(2.f))
{
}

void	LaboratoryEndScreen::start()
{
	InputListener::addInputListener();
	Progress::getInstance().setBubbleNpc(true);

	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::Camera & camera = octo::Application::getCamera();

	camera.setCenter(camera.getRectangle().width / 2.f, camera.getRectangle().height / 2.f);
	m_background.setTexture(resources.getTexture(LOGO_01_PNG));
	m_background.setOrigin(m_background.getLocalBounds().width / 2.f, m_background.getLocalBounds().height / 2.f);
	m_background.setPosition(octo::Application::getCamera().getCenter());

	m_ju.setPosition(sf::Vector2f(500.f, 500.f));
	m_lu.setPosition(sf::Vector2f(800.f, 800.f));
	m_fran.setPosition(sf::Vector2f(1200.f, 900.f));
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
			}
			m_ju.setDisplayText(false);
			m_lu.setDisplayText(false);
			m_fran.setDisplayText(false);
			m_cedric.setDisplayText(false);
			break;
		case Dialogs:
			m_ju.setDisplayText(true);
			m_lu.setDisplayText(true);
			m_fran.setDisplayText(true);
			m_cedric.setDisplayText(true);
			break;
		default:
			break;
	}

	m_ju.update(frameTime);
	m_lu.update(frameTime);
	m_fran.update(frameTime);
	m_cedric.update(frameTime);
}

void	LaboratoryEndScreen::draw(sf::RenderTarget & render) const
{
	sf::RenderStates states;
	render.clear(sf::Color::Black);
	render.draw(m_background);
	render.draw(m_ju);
	render.draw(m_lu);
	render.draw(m_fran);
	render.draw(m_cedric);
	m_ju.drawText(render, states);
	m_lu.drawText(render, states);
	m_fran.drawText(render, states);
	m_cedric.drawText(render, states);
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
