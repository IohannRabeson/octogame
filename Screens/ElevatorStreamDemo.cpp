/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ElevatorStreamDemo.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/01 04:24:03 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/12 18:26:55 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ElevatorStreamDemo.hpp"
#include <Application.hpp>
#include <Console.hpp>
#include <Camera.hpp>

ElevatorStreamDemo::ElevatorStreamDemo()
{
	octo::Console&	console = octo::Application::getConsole();

	console.addCommand(L"demo.setWidth", [this](float width)
			{
				m_teleportBeam.setWidth(width);
			});
	console.addCommand(L"demo.setColor", [this](sf::Color const& color)
			{
				m_teleportBeam.setColor(color);
			});
}

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
