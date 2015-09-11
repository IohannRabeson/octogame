/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ElevatorStreamDemo.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/01 04:24:03 by irabeson          #+#    #+#             */
/*   Updated: 2015/09/07 15:59:52 by jbalestr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ElevatorStreamDemo.hpp"
#include "DefaultBiome.hpp"
#include "BiomeManager.hpp"
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
}

void	ElevatorStreamDemo::start()
{
	m_teleportBeam.setPosX(0.f);
	m_teleportBeam.setPosY(-400.f);
	m_teleportBeam.setHeight(800.f);
	octo::Application::getCamera().setCenter(sf::Vector2f());

	BiomeManager manager;
	manager.registerBiome<DefaultBiome>("test");
	manager.changeBiome("test", 0);
	m_teleportBeam.setBiome(manager.getCurrentBiome());
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
	m_teleportBeam.drawBack(render);
	m_teleportBeam.drawFront(render);
}
