/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AudioDemoScreen.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/25 22:07:16 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/11 23:08:47 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AudioDemoScreen.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Console.hpp>
#include <AudioManager.hpp>
#include <GraphicsManager.hpp>

#include "ResourceDefinitions.hpp"
#include <SFML/Audio/Listener.hpp>

AudioDemoScreen::AudioDemoScreen()
{
	m_listenerPoint.setMoveCallback([](sf::Vector2f const& pos)
									{
										sf::Listener::setPosition(sf::Vector3f(pos.x, pos.y, 0.f));
									});
}

AudioDemoScreen::~AudioDemoScreen()
{
}

void	AudioDemoScreen::start()
{
	octo::Console&			console = octo::Application::getConsole();
	octo::Camera&			camera = octo::Application::getCamera();
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();

	graphics.addMouseListener(this);
	camera.setCenter(sf::Vector2f());
	console.addCommand(L"demo.play", [](std::string const& soundFileName, unsigned int ms)
		{
			octo::ResourceManager&	resources = octo::Application::getResourceManager();
			octo::AudioManager&		audio = octo::Application::getAudioManager();
			octo::Console&			console = octo::Application::getConsole();
			
			try
			{
				audio.startMusic(resources.getSound(soundFileName), sf::milliseconds(ms));
			}
			catch (std::exception const& e)
			{
				console.printError(e);
			}
		});
	console.addCommand(L"demo.stop", [](unsigned int ms)
		{
			octo::AudioManager&		audio = octo::Application::getAudioManager();

			audio.stopMusic(sf::milliseconds(ms));
		});
}

void	AudioDemoScreen::pause()
{
}

void	AudioDemoScreen::resume()
{
}

void	AudioDemoScreen::stop()
{
	octo::AudioManager&		audio = octo::Application::getAudioManager();
	octo::GraphicsManager&	graphics = octo::Application::getGraphicsManager();

	graphics.removeMouseListener(this);
	audio.stopMusic(sf::milliseconds(500));
}

void	AudioDemoScreen::update(sf::Time frameTime)
{
	(void)frameTime;
}

void	AudioDemoScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	render.draw(m_listenerPoint);
}

void	AudioDemoScreen::onPressed(sf::Event::MouseButtonEvent const& event)
{
	octo::Camera&			camera = octo::Application::getCamera();
	octo::AudioManager&		audio = octo::Application::getAudioManager();
	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	sf::Vector2f			clickPos = camera.mapPixelToCoords(sf::Vector2i(event.x, event.y));

	if (m_listenerPoint.hitTest(clickPos))
	{
		m_listenerPoint.setHandled(true);
	}
	else
	{
		audio.playSound(resources.getSound(CRYSTAL_OGG),
						1.f,
						1.f,
						sf::Vector3f(clickPos.x, clickPos.y, 0.f));
	}
}

void	AudioDemoScreen::onReleased(sf::Event::MouseButtonEvent const&)
{
	m_listenerPoint.setHandled(false);
}

void	AudioDemoScreen::onMoved(sf::Event::MouseMoveEvent const& event)
{
	octo::Camera&	camera = octo::Application::getCamera();
	sf::Vector2f	pos = camera.mapPixelToCoords(sf::Vector2i(event.x, event.y));

	m_listenerPoint.setPosition(pos);
}
