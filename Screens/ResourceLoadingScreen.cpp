/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceLoadingScreen.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/30 01:22:07 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/15 14:39:39 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResourceLoadingScreen.hpp"
#include "Progress.hpp"
#include "MusicManager.hpp"
#include <ResourceLoading.hpp>
#include <StateManager.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <Options.hpp>

ResourceLoadingScreen::ResourceLoadingScreen() :
	AbstractResourceLoadingState(),
	m_generator("random"),
	m_count(23u),
	m_index(0u)
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	octo::Application::getGraphicsManager().setIcon(resources.getTexture(ICON_PNG).copyToImage());

	m_key.resize(m_count);
	m_key[0] = SCREEN01_JPG;
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
	m_timerMax[0] = sf::seconds(0.06f);
	m_timerMax[1] = sf::seconds(0.07f);
	m_timerMax[2] = sf::seconds(0.08f);
	m_timerMax[3] = sf::seconds(1.3f);
	m_timerMax[4] = sf::seconds(0.05f);
	m_timerMax[5] = sf::seconds(0.07f);
	m_timerMax[6] = sf::seconds(0.5f);
	m_timerMax[7] = sf::seconds(0.08f);
	m_timerMax[8] = sf::seconds(0.1f);
	m_timerMax[9] = sf::seconds(0.06f);
	m_timerMax[10] = sf::seconds(0.05f);
	m_timerMax[11] = sf::seconds(0.3f);
	m_timerMax[12] = sf::seconds(0.1f);
	m_timerMax[13] = sf::seconds(1.3f);
	m_timerMax[14] = sf::seconds(0.06f);
	m_timerMax[15] = sf::seconds(0.05f);
	m_timerMax[16] = sf::seconds(0.04f);
	m_timerMax[17] = sf::seconds(0.07f);
	m_timerMax[18] = sf::seconds(0.06f);
	m_timerMax[19] = sf::seconds(0.07f);
	m_timerMax[20] = sf::seconds(0.06f);
	m_timerMax[21] = sf::seconds(0.05f);
	m_timerMax[22] = sf::seconds(2.1f);

	m_startTextures.resize(m_count);
	m_startSprites.resize(m_count);

	for (std::size_t i = 0u; i < m_count; i++)
	{
		m_startSprites[i].setTexture(resources.getTexture(m_key[i]));
		m_startSprites[i].setOrigin(m_startSprites[i].getLocalBounds().width / 2.f, m_startSprites[i].getLocalBounds().height / 2.f);
		m_startSprites[i].setPosition(octo::Application::getCamera().getCenter());
	}

	m_font = resources.getFont(CHINESETROOPS_TTF);
	m_message.setFont(m_font);
	m_message.setColor(sf::Color(254, 253, 56));
	m_message.setCharacterSize(20);
	m_borders.setOutlineColor(sf::Color(254, 253, 56));
	m_borders.setFillColor(sf::Color::Transparent);
	m_borders.setOutlineThickness(1.f);
	m_bar.setFillColor(sf::Color(254, 253, 56));
}

void	ResourceLoadingScreen::start()
{
	Progress &				progress = Progress::getInstance();

	pushLoading(octo::Application::getOptions().getPath() + "default.pck");
	progress.load("save.osv");
	AbstractResourceLoadingState::start();
}

void	ResourceLoadingScreen::stop()
{
}

void	ResourceLoadingScreen::updateLoading()
{
	sf::Vector2f	cameraCenter = octo::Application::getCamera().getCenter();
	sf::FloatRect	cameraRect = octo::Application::getCamera().getRectangle();
	float			padding = cameraRect.width * 0.5;
	float			height = cameraRect.height * 0.02;
	float			progress = static_cast<float>(getCurrentStep() + 1) / getTotalSteps();

	AbstractResourceLoadingState::update(sf::Time::Zero);
	m_string = "Loading '" + getCurrentKeyLoaded() + "' - " + std::to_string(getCurrentStep() + 1) + " / " + std::to_string(getTotalSteps());
	m_message.setString(m_string);

	// To center text
	float widthTotalText = m_message.findCharacterPos(m_string.size() - 1).x - m_message.findCharacterPos(0).x + m_font.getGlyph(m_string[m_string.size() - 1], m_message.getCharacterSize(), 0).advance;
	float delta = ((cameraRect.width - padding) - widthTotalText) / 2.f;

	float posY = cameraCenter.y + cameraCenter.y * 0.8;
	m_message.setPosition(cameraRect.left + padding / 2 + delta, posY + height);
	m_borders.setPosition(cameraRect.left + padding / 2, posY - (height / 2));
	m_borders.setSize(sf::Vector2f(cameraRect.width - padding, height));
	m_bar.setPosition(cameraRect.left + padding / 2, posY - (height / 2) + 1);
	m_bar.setSize(sf::Vector2f((cameraRect.width - padding - 2) * progress, height - 2));
}

void	ResourceLoadingScreen::updateScreen(sf::Time frameTime)
{
	m_timer += frameTime;
	if (m_timer > m_timerMax[m_index])
	{
		if (m_index == 1u || m_index == 5u || m_index == 7u || m_index == 9u || m_index == 12u || m_index == 15u)
		{
			octo::AudioManager& audio = octo::Application::getAudioManager();
			octo::ResourceManager& resources = octo::Application::getResourceManager();
			audio.playSound(resources.getSound(LOGO_SOUND_OGG), 0.7f, m_generator.randomFloat(0.95f, 1.05f));
		}
		m_timer = sf::Time::Zero;
		m_index++;
		if (m_index >= m_count)
			m_index = 0u;
	}
}

void	ResourceLoadingScreen::update(sf::Time frameTime)
{
	updateLoading();
	updateScreen(frameTime);
}

void	ResourceLoadingScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::Black);
	render.draw(m_startSprites[m_index]);
	render.draw(m_message);
	render.draw(m_borders);
	render.draw(m_bar);
}

void	ResourceLoadingScreen::onNoMoreLoading()
{
	octo::StateManager & states = octo::Application::getStateManager();

	// Load musics in memory
	MusicManager::getInstance();
	states.change("logo");
}
