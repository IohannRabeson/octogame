/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnimatedSpriteDemoScreen.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/23 15:32:28 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/06 10:36:31 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AnimatedSpriteDemoScreen.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <Console.hpp>

#include "ResourceDefinitions.hpp"

AnimatedSpriteDemoScreen::AnimatedSpriteDemoScreen()
{
	octo::SpriteAnimation::FrameList	frames;

	frames.emplace_back(sf::seconds(0.1), 0);
	frames.emplace_back(sf::seconds(0.1), 1);
	frames.emplace_back(sf::seconds(0.1), 2);
	frames.emplace_back(sf::seconds(0.1), 3);
	frames.emplace_back(sf::seconds(0.1), 4);
	frames.emplace_back(sf::seconds(0.1), 5);
	frames.emplace_back(sf::seconds(0.1), 6);
	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::Loop);
}

AnimatedSpriteDemoScreen::~AnimatedSpriteDemoScreen()
{
}

void	AnimatedSpriteDemoScreen::start()
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();
	octo::Camera&			camera = octo::Application::getCamera();
	octo::Console&			console = octo::Application::getConsole();

	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_OSS));
	m_sprite.setAnimation(m_animation);
	m_sprite.play();
	camera.setCenter(m_sprite.getGlobalBounds().left + m_sprite.getGlobalBounds().width / 2,
					 m_sprite.getGlobalBounds().top + m_sprite.getGlobalBounds().height / 2);
	console.addCommand(L"demo.play", m_sprite, &octo::AnimatedSprite::play);
	console.addCommand(L"demo.stop", m_sprite, &octo::AnimatedSprite::stop);
	console.addCommand(L"demo.pause", m_sprite, &octo::AnimatedSprite::pause);
	console.addCommand(L"demo.setSpeed", m_sprite, &octo::AnimatedSprite::setSpeedFactor);
}

void	AnimatedSpriteDemoScreen::pause()
{

}

void	AnimatedSpriteDemoScreen::resume()
{

}

void	AnimatedSpriteDemoScreen::stop()
{

}

void	AnimatedSpriteDemoScreen::update(sf::Time frameTime)
{
	m_sprite.update(frameTime);
}

void	AnimatedSpriteDemoScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	render.draw(m_sprite);
}

void	AnimatedSpriteDemoScreen::setSpriteSheet(octo::SpriteSheet const& spriteSheet)
{
	m_sprite.setSpriteSheet(spriteSheet);
}

void	AnimatedSpriteDemoScreen::setAnimation(octo::SpriteAnimation const& animation)
{
	m_sprite.setAnimation(animation);
}

bool	AnimatedSpriteDemoScreen::onPressed(sf::Event::KeyEvent const&)
{
	return (true);
}
