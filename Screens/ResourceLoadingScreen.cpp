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
#include <ResourceLoading.hpp>
#include <StateManager.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <GraphicsManager.hpp>
#include <Camera.hpp>
#include <Options.hpp>

#include "ResourceDefinitions.hpp"

ResourceLoadingScreen::ResourceLoadingScreen() :
	AbstractResourceLoadingState()
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	octo::Application::getGraphicsManager().setIcon(resources.getTexture(ICON_PNG).copyToImage());
	m_startTexture = resources.getTexture(START_SCREEN_PNG);
	m_startTexture.setSmooth(true);
	m_startSprite.setTexture(m_startTexture);
	m_startSprite.setOrigin(m_startSprite.getLocalBounds().width / 2.f, m_startSprite.getLocalBounds().height / 2.f);
	m_startSprite.setPosition(octo::Application::getCamera().getCenter());
	m_startSprite.setScale(1.2f, 1.2f);
	m_font = resources.getFont(CHINESETROOPS_TTF);
	m_message.setFont(m_font);
	m_message.setColor(sf::Color(253, 235, 62));
	m_message.setCharacterSize(20);
	m_borders.setOutlineColor(sf::Color(253, 235, 62));
	m_borders.setFillColor(sf::Color::Transparent);
	m_borders.setOutlineThickness(1.f);
	m_bar.setFillColor(sf::Color(253, 235, 62));
}

void	ResourceLoadingScreen::start()
{
	pushLoading("default.pck");
	Progress & progress = Progress::getInstance();
	progress.load("save.osv");
	AbstractResourceLoadingState::start();
}

void	ResourceLoadingScreen::update(sf::Time frameTime)
{
	sf::Vector2f	cameraCenter = octo::Application::getCamera().getCenter();
	sf::FloatRect	cameraRect = octo::Application::getCamera().getRectangle();
	float			padding = cameraRect.width * 0.5;
	float			height = cameraRect.height * 0.02;
	float			progress = static_cast<float>(getCurrentStep() + 1) / getTotalSteps();

	AbstractResourceLoadingState::update(frameTime);
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

void	ResourceLoadingScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::Black);
	render.draw(m_startSprite);
	render.draw(m_message);
	render.draw(m_borders);
	render.draw(m_bar);
}

void	ResourceLoadingScreen::onNoMoreLoading()
{
	Progress &				progress = Progress::getInstance();
	octo::StateManager & states = octo::Application::getStateManager();

	//octo::Application::getStateManager().change(octo::Application::getOptions().getValue<std::string>("second_state", "game"));
	if (progress.isFirstTime())
	{
		progress.setFirstTime(false);
		progress.save();
		states.change("zero");
	}
	else
		states.change("game");
}
