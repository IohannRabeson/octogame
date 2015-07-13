/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceLoadingScreen.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/30 01:22:07 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/13 19:11:41 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResourceLoadingScreen.hpp"
#include "FireflyPositionBehaviors.hpp"
#include <ResourceLoading.hpp>
#include <StateManager.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <AudioManager.hpp>

#include "ResourceDefinitions.hpp"

ResourceLoadingScreen::ResourceLoadingScreen() :
	AbstractResourceLoadingState()
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	m_message.setFont(resources.getFont(VERAMONO_TTF));
	m_borders.setOutlineColor(sf::Color::White);
	m_borders.setFillColor(sf::Color::Transparent);
	m_borders.setOutlineThickness(1.f);
	m_bar.setFillColor(sf::Color::White);
}

void	ResourceLoadingScreen::start()
{
	// TODO: definir les packages a charger ici
	// pushLoading("sounds.pck");
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
	m_message.setString("Loading '" + getCurrentKeyLoaded() + "' - " + std::to_string(getCurrentStep() + 1) + " / " + std::to_string(getTotalSteps()));
	m_message.setPosition(cameraRect.left + padding / 2, cameraCenter.y + height);
	m_borders.setPosition(cameraRect.left + padding / 2, cameraCenter.y - (height / 2));
	m_borders.setSize(sf::Vector2f(cameraRect.width - padding, height));
	m_bar.setPosition(cameraRect.left + padding / 2, cameraCenter.y - (height / 2) + 1);
	m_bar.setSize(sf::Vector2f((cameraRect.width - padding - 2) * progress, height - 2));
}

void	ResourceLoadingScreen::draw(sf::RenderTarget& render)const
{
	render.clear(sf::Color::Black);
	render.draw(m_message);
	render.draw(m_borders);
	render.draw(m_bar);
}

void	ResourceLoadingScreen::onNoMoreLoading()
{
	// TODO: definir l'etat suivant
	octo::Application::getStateManager().change("game");
}
