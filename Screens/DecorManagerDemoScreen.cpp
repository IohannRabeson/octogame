/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DecorManagerDemoScreen.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/10 01:39:09 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/11 19:04:34 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DecorManagerDemoScreen.hpp"
#include "ADecor.hpp"
#include "DecorBuilder.hpp"

#include <Application.hpp>
#include <Camera.hpp>
#include <Console.hpp>
#include <GraphicsManager.hpp>

class DecorTestA : public ADecor
{
public:
	DecorTestA() :
		m_halfSize(30, 30)
	{
	}

	virtual void			update(sf::Time frameTime,
								   DecorBuilder& builder,
								   ABiome& biome)
	{
		sf::Vector2f const&	pos = getPosition();

		builder.createQuad(sf::Vector2f(pos.x - m_halfSize.x, pos.y - m_halfSize.y),
						   sf::Vector2f(pos.x + m_halfSize.x, pos.y - m_halfSize.y),
						   sf::Vector2f(pos.x + m_halfSize.x, pos.y + m_halfSize.y),
						   sf::Vector2f(pos.x - m_halfSize.x, pos.y + m_halfSize.y),
						   sf::Color::White);
		static_cast<void>(frameTime);
		static_cast<void>(random);
		static_cast<void>(biome);
	}
private:
	sf::Vector2f	m_halfSize;
};

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
	m_manager.update(frameTime, m_biome, camera);
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
