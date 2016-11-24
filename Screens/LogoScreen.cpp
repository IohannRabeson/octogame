#include "LogoScreen.hpp"
#include "Progress.hpp"
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <StateManager.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>

LogoScreen::LogoScreen() :
	m_generator("random"),
	m_count(38u),
	m_index(0u)
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	octo::Application::getGraphicsManager().setIcon(resources.getTexture(ICON_PNG).copyToImage());

	m_key.resize(m_count);
	m_key[0] = LOGO_01_PNG;
	m_key[1] = LOGO_02_PNG;
	m_key[2] = LOGO_03_PNG;
	m_key[3] = LOGO_05_PNG;
	m_key[4] = LOGO_04_PNG;
	m_key[5] = LOGO_06_PNG;
	m_key[6] = LOGO_07_PNG;
	m_key[7] = LOGO_08_PNG;
	m_key[8] = LOGO_09_PNG;
	m_key[9] = LOGO_10_PNG;
	m_key[10] = LOGO_11_PNG;
	m_key[11] = LOGO_12_PNG;
	m_key[12] = LOGO_13_PNG;
	m_key[13] = LOGO_14_PNG;
	m_key[14] = LOGO_15_PNG;
	m_key[15] = LOGO_16_PNG;
	m_key[16] = LOGO_17_PNG;
	m_key[17] = LOGO_18_PNG;
	m_key[18] = LOGO_19_PNG;
	m_key[19] = LOGO_20_PNG;
	m_key[20] = LOGO_21_PNG;
	m_key[21] = LOGO_22_PNG;
	m_key[22] = LOGO_23_PNG;
	m_key[23] = LOGO_24_PNG;
	m_key[24] = LOGO_25_JPG;
	m_key[25] = LOGO_26_JPG;
	m_key[26] = LOGO_27_JPG;
	m_key[27] = LOGO_28_JPG;
	m_key[28] = LOGO_29_JPG;
	m_key[29] = LOGO_30_JPG;
	m_key[30] = LOGO_31_JPG;
	m_key[31] = LOGO_32_JPG;
	m_key[32] = LOGO_33_JPG;
	m_key[33] = LOGO_34_JPG;
	m_key[34] = LOGO_35_JPG;
	m_key[35] = LOGO_36_JPG;
	m_key[36] = LOGO_37_JPG;
	m_key[37] = LOGO_38_JPG;

	m_timerMax.resize(m_count);
	m_timerMax[0] = sf::seconds(0.07f);
	m_timerMax[1] = sf::seconds(0.04f);
	m_timerMax[2] = sf::seconds(0.08f);
	m_timerMax[3] = sf::seconds(0.04f);
	m_timerMax[4] = sf::seconds(0.06f);
	m_timerMax[5] = sf::seconds(0.07f);
	m_timerMax[6] = sf::seconds(0.07f);
	m_timerMax[7] = sf::seconds(2.2f);
	m_timerMax[8] = sf::seconds(0.1f);
	m_timerMax[9] = sf::seconds(0.07f);
	m_timerMax[10] = sf::seconds(0.09f);
	m_timerMax[11] = sf::seconds(0.07f);
	m_timerMax[12] = sf::seconds(0.06f);
	m_timerMax[13] = sf::seconds(0.04f);
	m_timerMax[14] = sf::seconds(2.03f);
	m_timerMax[15] = sf::seconds(0.05f);
	m_timerMax[16] = sf::seconds(0.04f);
	m_timerMax[17] = sf::seconds(0.07f);
	m_timerMax[18] = sf::seconds(0.05f);
	m_timerMax[19] = sf::seconds(0.08f);
	m_timerMax[20] = sf::seconds(0.06f);
	m_timerMax[21] = sf::seconds(0.07f);
	m_timerMax[22] = sf::seconds(0.04f);
	m_timerMax[23] = sf::seconds(0.06f);

	m_timerMax[23] = sf::seconds(0.04f);
	m_timerMax[24] = sf::seconds(0.07f);
	m_timerMax[25] = sf::seconds(0.06f);
	m_timerMax[26] = sf::seconds(0.04f);
	m_timerMax[27] = sf::seconds(0.41f);
	m_timerMax[28] = sf::seconds(0.4f);
	m_timerMax[29] = sf::seconds(0.44f);
	m_timerMax[30] = sf::seconds(0.38f);
	m_timerMax[31] = sf::seconds(0.35f);
	m_timerMax[32] = sf::seconds(0.4f);
	m_timerMax[33] = sf::seconds(0.06f);
	m_timerMax[34] = sf::seconds(0.04f);
	m_timerMax[35] = sf::seconds(0.06f);
	m_timerMax[36] = sf::seconds(0.05f);
	m_timerMax[37] = sf::seconds(0.06f);

	m_startSprites.resize(m_count);

	for (std::size_t i = 0u; i < m_count; i++)
	{
		m_startSprites[i].setTexture(resources.getTexture(m_key[i]));
		m_startSprites[i].setOrigin(m_startSprites[i].getLocalBounds().width / 2.f, m_startSprites[i].getLocalBounds().height / 2.f);
		m_startSprites[i].setPosition(octo::Application::getCamera().getCenter());
	}
}

void	LogoScreen::start()
{
	InputListener::addInputListener();
}

void	LogoScreen::pause()
{
}

void	LogoScreen::resume()
{
}

void	LogoScreen::stop()
{
	InputListener::removeInputListener();
}


void	LogoScreen::update(sf::Time frameTime)
{
	m_timer += frameTime;
	if (m_timer > m_timerMax[m_index])
	{
		m_timer = sf::Time::Zero;
		m_index++;
		if (m_index == 1u || m_index == 8u || m_index == 15u || m_index == 24u || m_index == 33u)
		{
			octo::AudioManager& audio = octo::Application::getAudioManager();
			octo::ResourceManager& resources = octo::Application::getResourceManager();
			audio.playSound(resources.getSound(LOGO_SOUND_OGG), 1.f, m_generator.randomFloat(0.95f, 1.05f));
			if (m_index == 8u)
				audio.playSound(resources.getSound(OCTO_VOICE_PLAY_OGG), 1.f);
		}
		if (m_index >= m_count)
		{
			octo::StateManager & states = octo::Application::getStateManager();
			states.change("menu");
		}
	}
}

void	LogoScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::Black);
	if (m_index < m_count)
		render.draw(m_startSprites[m_index]);
}

bool	LogoScreen::onInputPressed(InputListener::OctoKeys const &)
{
	if (!Progress::getInstance().isFirstTime())
	{
		octo::StateManager & states = octo::Application::getStateManager();
		states.change("menu");
	}
	return true;
}
