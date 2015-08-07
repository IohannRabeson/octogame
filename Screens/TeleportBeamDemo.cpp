/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TeleportBeamDemo.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/01 04:24:03 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/01 05:05:44 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TeleportBeamDemo.hpp"
#include <Application.hpp>
#include <Camera.hpp>

void	TeleportBeamDemo::start()
{
	m_teleportBeam.setPoints(sf::Vector2f(0.f, -400), sf::Vector2f(0.f, 400.f));
	octo::Application::getCamera().setCenter(sf::Vector2f());	
}

void	TeleportBeamDemo::pause()
{

}

void	TeleportBeamDemo::resume()
{

}

void	TeleportBeamDemo::stop()
{

}

void	TeleportBeamDemo::update(sf::Time frameTime)
{
	m_teleportBeam.update(frameTime);
}

void	TeleportBeamDemo::draw(sf::RenderTarget& render)const
{
	render.clear();
	m_teleportBeam.draw(render);
}
