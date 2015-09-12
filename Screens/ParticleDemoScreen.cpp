/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleDemoScreen.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/19 06:00:19 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/10 19:32:58 by jbalestr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParticleDemoScreen.hpp"
#include <Application.hpp>
#include <Camera.hpp>
#include <GraphicsManager.hpp>
#include <Console.hpp>
#include <Math.hpp>

#include <ctime>

TestSystem::TestSystem() :
	m_emit(false),
	m_engine(std::time(0)),
	m_lifeTimeDistri(0.5f, 1.f),
	m_directionDistri(0.f, 2.f * octo::Pi)
{
}

void	TestSystem::onMoved(sf::Event::MouseMoveEvent const& event)
{
	octo::Camera&	camera = octo::Application::getCamera();

	m_emitter = camera.mapPixelToCoords(sf::Vector2i(event.x, event.y));	
}

void	TestSystem::onPressed(sf::Event::MouseButtonEvent const&)
{
	m_emit = true;
}

void	TestSystem::onReleased(sf::Event::MouseButtonEvent const&)
{
	m_emit = false;
}

void	TestSystem::update(sf::Time frameTime)
{
	ParticleSystem::update(frameTime);
	if (m_emit)
	{
		emplace(sf::Color::Green, m_emitter, sf::Vector2f(1.f, 1.f), m_directionDistri(m_engine) * 180,
				sf::Time::Zero,
				sf::seconds(m_lifeTimeDistri(m_engine)),
				m_directionDistri(m_engine));
	}
}

void	TestSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	static float const			Velocity = 256.f;
	static float const			AngularVelocity = 30.f;
	sf::Vector2f	direction(std::sin(std::get<MyComponent::Direction>(particle)) * Velocity,
							  std::cos(std::get<MyComponent::Direction>(particle)) * Velocity);

	std::get<Component::Position>(particle) = std::get<Component::Position>(particle) + direction * frameTime.asSeconds();
	std::get<Component::Rotation>(particle) = std::get<Component::Rotation>(particle) + AngularVelocity * frameTime.asSeconds();
	std::get<MyComponent::Time>(particle) = std::get<MyComponent::Time>(particle) + frameTime;
	std::get<Component::Color>(particle).a = 255 * std::max(0.f, (1.f - std::get<MyComponent::Time>(particle).asSeconds() / std::get<MyComponent::Life>(particle).asSeconds()));
}

bool	TestSystem::isDeadParticle(Particle const& particle)
{
	return (std::get<MyComponent::Time>(particle) >= std::get<MyComponent::Life>(particle));
}

void	ParticleDemoScreen::start()
{
	static float const	Size = 8.f;
	octo::Console&		console = octo::Application::getConsole();
	TestSystem::Prototype	prototype;

	//m_rainSystem.setDropSize(sf::Vector2f(0.5f, 50.f));
	//m_rainSystem.setDropSpeed(1024.f);
	prototype.emplace_back(sf::Vertex({-Size, Size}));
	prototype.emplace_back(sf::Vertex({Size, -Size}));
	prototype.emplace_back(sf::Vertex({-Size, -Size}));
	m_system.reset(prototype, sf::Triangles, 2000);
	octo::Application::getGraphicsManager().addMouseListener(&m_system);
	console.addCommand(L"demo.setDropPerSecond", m_rainSystem, &DropSystem::setDropPerSecond);
	//console.addCommand(L"demo.setDropAngle", m_rainSystem, &DropSystem::setDropAngle);
	//console.addCommand(L"demo.setDropSpeed", m_rainSystem, &DropSystem::setDropSpeed);
}

void	ParticleDemoScreen::pause()
{

}

void	ParticleDemoScreen::resume()
{

}

void	ParticleDemoScreen::stop()
{
	octo::Application::getGraphicsManager().removeMouseListener(&m_system);
}

void	ParticleDemoScreen::update(sf::Time frameTime)
{
	m_system.update(frameTime);
	m_smokeSystem.update(frameTime);
	//sf::FloatRect rect = octo::Application::getCamera().getRectangle();
	//rect.top -= octo::Application::getCamera().getSize().y;
	//m_rainSystem.setDropRect(rect);
//	m_rainSystem.update(frameTime);
}

void	ParticleDemoScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	m_system.draw(render);
	m_smokeSystem.draw(render);
//	m_rainSystem.draw(render);
}
