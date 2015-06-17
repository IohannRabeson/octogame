/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DecorManagerDemoScreen.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/10 01:39:09 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/12 16:32:06 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DecorManagerDemoScreen.hpp"
#include "ADecor.hpp"
#include "DecorBuilder.hpp"

#include <Application.hpp>
#include <Camera.hpp>
#include <Console.hpp>
#include <GraphicsManager.hpp>

DecorManagerDemoScreen::DecorManagerDemoScreen() :
	m_manager(20000),
	m_currentDecorType(DecorManager::DecorTypes::Invalid)
{
	std::fill(m_viewDirections.begin(), m_viewDirections.end(), false);
	m_typeMapper["tree"] = DecorManager::DecorTypes::Tree;
	m_typeMapper["rock"] = DecorManager::DecorTypes::Rock;
	m_typeMapper["crystal"] = DecorManager::DecorTypes::Crystal;
	m_typeMapper["star"] = DecorManager::DecorTypes::Star;
	m_typeMapper["cloud"] = DecorManager::DecorTypes::Cloud;
	m_typeMapper["sun"] = DecorManager::DecorTypes::Sun;
	m_typeMapper["moon"] = DecorManager::DecorTypes::Moon;
}

void	DecorManagerDemoScreen::start()
{
	octo::Console&	console = octo::Application::getConsole();
	
	octo::Application::getCamera().setCenter(sf::Vector2f(0, 0));
	octo::Application::getGraphicsManager().addMouseListener(this);
	octo::Application::getGraphicsManager().addKeyboardListener(this);
	console.addCommand(L"demo.select", this, &DecorManagerDemoScreen::selectDecorType);
	console.addCommand(L"demo.setTreeDepth", m_biome, &TestBiome::setTreeDepth);
	console.addCommand(L"demo.setTreeLifeTime", m_biome, &TestBiome::setTreeLifeTime);
	console.addCommand(L"demo.setTreeColor", m_biome, &TestBiome::setTreeColor);
	console.addCommand(L"demo.setCanCreateTree", m_biome, &TestBiome::setCanCreateTree);
	console.addCommand(L"demo.setCrystalSize", m_biome, &TestBiome::setCrystalSize);
	console.addCommand(L"demo.setCrystalPartCount", m_biome, &TestBiome::setCrystalPartCount);
	console.addCommand(L"demo.setCrystalColor", m_biome, &TestBiome::setCrystalColor);
	console.addCommand(L"demo.setRockSize", m_biome, &TestBiome::setRockSize);
	console.addCommand(L"demo.setRockPartCount", m_biome, &TestBiome::setRockPartCount);
	console.addCommand(L"demo.setCanCreateRock", m_biome, &TestBiome::setCanCreateRock);
	console.addCommand(L"demo.setCloudSize", m_biome, &TestBiome::setCloudSize);
	console.addCommand(L"demo.setCloudPartCount", m_biome, &TestBiome::setCloudPartCount);
	console.addCommand(L"demo.setCloudLifeTime", m_biome, &TestBiome::setCloudLifeTime);
	console.addCommand(L"demo.setCloudColor", m_biome, &TestBiome::setCloudColor);
	console.addCommand(L"demo.setStarSize", m_biome, &TestBiome::setStarSize);
	console.addCommand(L"demo.setStarColor", m_biome, &TestBiome::setStarColor);
	console.addCommand(L"demo.setStarLifeTime", m_biome, &TestBiome::setStarLifeTime);
	console.addCommand(L"demo.setCanCreateStar", m_biome, &TestBiome::setCanCreateStar);
	console.addCommand(L"demo.setSunSize", m_biome, &TestBiome::setSunSize);
	console.addCommand(L"demo.setSunColor", m_biome, &TestBiome::setSunColor);
	console.addCommand(L"demo.setCanCreateSun", m_biome, &TestBiome::setCanCreateSun);
	console.addCommand(L"demo.setMoonSize", m_biome, &TestBiome::setMoonSize);
	console.addCommand(L"demo.setMoonColor", m_biome, &TestBiome::setMoonColor);
	console.addCommand(L"demo.setCanCreateMoon", m_biome, &TestBiome::setCanCreateMoon);
	m_manager.setup(&m_biome);
}

void	DecorManagerDemoScreen::pause()
{
}

void	DecorManagerDemoScreen::resume()
{
}

void	DecorManagerDemoScreen::stop()
{
	octo::Application::getGraphicsManager().removeMouseListener(this);
	octo::Application::getGraphicsManager().removeKeyboardListener(this);
}

void	DecorManagerDemoScreen::update(sf::Time frameTime)
{
	octo::Camera&	camera = octo::Application::getCamera();

	moveCamera(frameTime, camera);
	m_manager.update(frameTime, camera);
}

void	DecorManagerDemoScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	m_manager.draw(render);
}

void	DecorManagerDemoScreen::moveCamera(sf::Time frameTime, octo::Camera& camera)const
{
	static float const	CameraSpeed = 512.f;

	if (m_viewDirections[Up])
		camera.move(0.f, -CameraSpeed * frameTime.asSeconds());
	else if (m_viewDirections[Down])
		camera.move(0.f, CameraSpeed * frameTime.asSeconds());
	if (m_viewDirections[Left])
		camera.move(-CameraSpeed * frameTime.asSeconds(), 0.f);
	else if (m_viewDirections[Right])
		camera.move(CameraSpeed * frameTime.asSeconds(), 0.f);
}

std::string	DecorManagerDemoScreen::selectDecorType(std::string const& type)
{
	auto	it = m_typeMapper.find(type);

	if (it != m_typeMapper.end())
	{
		m_currentDecorType = it->second;
		return ("selected decor type: " + type);
	}
	else
	{
		return ("invalid decor type: " + type);
	}
}

void	DecorManagerDemoScreen::createDecor(sf::Vector2f const& pos)
{
	try
	{
		auto it = m_manager.add(m_currentDecorType);

		(*it)->setPosition(pos);
	}
	catch(std::exception const& e)
	{
		octo::Application::getConsole().printError(e);
	}
}

void 	DecorManagerDemoScreen::onPressed(sf::Event::MouseButtonEvent const &event)
{
	octo::Camera&	camera = octo::Application::getCamera();
	sf::Vector2f	position = camera.mapPixelToCoords(sf::Vector2i(event.x, event.y));

	createDecor(position);
}

void 	DecorManagerDemoScreen::onReleased(sf::Event::MouseButtonEvent const &)
{
}

bool	DecorManagerDemoScreen::onPressed(sf::Event::KeyEvent const& event)
{
	switch (event.code)
	{
		case sf::Keyboard::Up:
			m_viewDirections[Up] = true;
			break;
		case sf::Keyboard::Down:
			m_viewDirections[Down] = true;
			break;
		case sf::Keyboard::Left:
			m_viewDirections[Left] = true;
			break;
		case sf::Keyboard::Right:
			m_viewDirections[Right] = true;
			break;
		default:
			break;
	}
	return (true);
}

bool	DecorManagerDemoScreen::onReleased(sf::Event::KeyEvent const& event)
{
	switch (event.code)
	{
		case sf::Keyboard::Up:
			m_viewDirections[0] = false;
			break;
		case sf::Keyboard::Down:
			m_viewDirections[2] = false;
			break;
		case sf::Keyboard::Left:
			m_viewDirections[3] = false;
			break;
		case sf::Keyboard::Right:
			m_viewDirections[1] = false;
			break;
		default:
			break;
	}
	return (true);
}
