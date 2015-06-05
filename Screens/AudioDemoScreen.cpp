/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AudioDemoScreen.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/25 22:07:16 by irabeson          #+#    #+#             */
/*   Updated: 2015/05/31 16:15:10 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AudioDemoScreen.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Console.hpp>
#include <AudioManager.hpp>

#include "../ResourceDefinitions.hpp"

AudioDemoScreen::AudioDemoScreen()
{
}

AudioDemoScreen::~AudioDemoScreen()
{
}

void	AudioDemoScreen::start()
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();
//	octo::Camera&			camera = octo::Application::getCamera();
	octo::Console&			console = octo::Application::getConsole();
	octo::AudioManager&		audio = octo::Application::getAudioManager();

	audio.startMusic(resources.getSound(KRUPA_SOLO_WAV), sf::Time::Zero);
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

	audio.stopMusic(sf::milliseconds(500));
}

void	AudioDemoScreen::update(sf::Time frameTime)
{
	(void)frameTime;
}

void	AudioDemoScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
}

bool	AudioDemoScreen::onPressed(sf::Event::KeyEvent const&)
{
	return (true);
}
