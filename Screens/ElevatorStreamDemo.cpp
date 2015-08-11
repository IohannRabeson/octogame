/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ElevatorStreamDemo.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/01 04:24:03 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/07 16:33:19 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ElevatorStreamDemo.hpp"
#include <Application.hpp>
#include <Camera.hpp>

void	ElevatorStreamDemo::start()
{
	m_teleportBeam.setPoints(sf::Vector2f(0.f, -400), sf::Vector2f(0.f, 400.f));
	octo::Application::getCamera().setCenter(sf::Vector2f());	
}

void	ElevatorStreamDemo::pause()
{

}

void	ElevatorStreamDemo::resume()
{

}

void	ElevatorStreamDemo::stop()
{

}

void	ElevatorStreamDemo::update(sf::Time frameTime)
{
	m_teleportBeam.update(frameTime);
}

void	ElevatorStreamDemo::draw(sf::RenderTarget& render)const
{
	render.clear();
	m_teleportBeam.draw(render);
}
