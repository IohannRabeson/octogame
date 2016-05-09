#include "AnOctonautOdysseyScreen.hpp"
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <StateManager.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>

AnOctonautOdysseyScreen::AnOctonautOdysseyScreen() :
	m_count(23u),
	m_index(0u)
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	octo::Application::getGraphicsManager().setIcon(resources.getTexture(ICON_PNG).copyToImage());

	m_key.resize(m_count);
	m_key[0] = SCREEN07_JPG;
	m_key[1] = SCREEN02_JPG;
	m_key[2] = SCREEN03_JPG;
	m_key[3] = SCREEN04_JPG;
	m_key[4] = SCREEN04_JPG;
	m_key[5] = SCREEN06_JPG;
	m_key[6] = SCREEN07_JPG;
	m_key[7] = SCREEN08_JPG;
	m_key[8] = SCREEN09_JPG;
	m_key[9] = SCREEN10_JPG;
	m_key[10] = SCREEN11_JPG;
	m_key[11] = SCREEN12_JPG;
	m_key[12] = SCREEN13_JPG;
	m_key[13] = SCREEN14_JPG;
	m_key[14] = SCREEN15_JPG;
	m_key[15] = SCREEN16_JPG;
	m_key[16] = SCREEN17_JPG;
	m_key[17] = SCREEN18_JPG;
	m_key[18] = SCREEN19_JPG;
	m_key[19] = SCREEN20_JPG;
	m_key[20] = SCREEN21_JPG;
	m_key[21] = SCREEN22_JPG;
	m_key[22] = SCREEN23_JPG;

	m_timerMax.resize(m_count);
	m_timerMax[0] = sf::seconds(0.03f);
	m_timerMax[1] = sf::seconds(0.03f);
	m_timerMax[2] = sf::seconds(0.03f);
	m_timerMax[3] = sf::seconds(1.3f);
	m_timerMax[4] = sf::seconds(0.03f);
	m_timerMax[5] = sf::seconds(0.04f);
	m_timerMax[6] = sf::seconds(0.5f);
	m_timerMax[7] = sf::seconds(0.02f);
	m_timerMax[8] = sf::seconds(0.1f);
	m_timerMax[9] = sf::seconds(0.03f);
	m_timerMax[10] = sf::seconds(0.02f);
	m_timerMax[11] = sf::seconds(0.3f);
	m_timerMax[12] = sf::seconds(0.1f);
	m_timerMax[13] = sf::seconds(1.3f);
	m_timerMax[14] = sf::seconds(0.03f);
	m_timerMax[15] = sf::seconds(0.05f);
	m_timerMax[16] = sf::seconds(0.04f);
	m_timerMax[17] = sf::seconds(0.02f);
	m_timerMax[18] = sf::seconds(0.03f);
	m_timerMax[19] = sf::seconds(0.04f);
	m_timerMax[20] = sf::seconds(0.02f);
	m_timerMax[21] = sf::seconds(0.05f);
	m_timerMax[22] = sf::seconds(2.1f);

	m_startTextures.resize(m_count);
	m_startSprites.resize(m_count);

	for (std::size_t i = 0u; i < m_count; i++)
	{
		m_startTextures[i] = resources.getTexture(m_key[i]);
		m_startTextures[i].setSmooth(true);
		m_startSprites[i].setTexture(m_startTextures[i]);
		m_startSprites[i].setOrigin(m_startSprites[i].getLocalBounds().width / 2.f, m_startSprites[i].getLocalBounds().height / 2.f);
		m_startSprites[i].setPosition(octo::Application::getCamera().getCenter());
	}
}

void	AnOctonautOdysseyScreen::start()
{
}

void	AnOctonautOdysseyScreen::pause()
{
}

void	AnOctonautOdysseyScreen::resume()
{
}

void	AnOctonautOdysseyScreen::stop()
{
}


void	AnOctonautOdysseyScreen::update(sf::Time frameTime)
{
	m_timer += frameTime;
	if (m_timer > m_timerMax[m_index])
	{
		m_timer = sf::Time::Zero;
		m_index++;
		if (m_index == m_count)
		{
			octo::StateManager & states = octo::Application::getStateManager();
			states.change("logo");
		}
	}
}

void	AnOctonautOdysseyScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::Black);
	if (m_index < m_count)
		render.draw(m_startSprites[m_index]);
}

