/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleDemoScreen.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/19 06:00:19 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/19 16:43:32 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParticleDemoScreen.hpp"

void	TestSystem::updateParticle(sf::Time frameTime, Particle& particle)
{
	static sf::Vector2f const	Velocity(0.2f, 512.f);
	static float const			AngularVelocity = 30.f;

	std::get<Component::Position>(particle) = std::get<Component::Position>(particle) + (Velocity * frameTime.asSeconds());
	std::get<Component::Rotation>(particle) = std::get<Component::Rotation>(particle) + AngularVelocity * frameTime.asSeconds();
}

bool	TestSystem::isDeadParticle(Particle const& particle)
{
	return (std::get<Component::Position>(particle).y > 512.f);
}

void	ParticleDemoScreen::start()
{
	static float const	Size = 4.f;

	TestSystem::Prototype	prototype;

	prototype.emplace_back(-Size, Size);
	prototype.emplace_back(Size, Size);
	prototype.emplace_back(Size, -Size);
	prototype.emplace_back(-Size, -Size);
	m_system.reset(prototype, sf::Quads, 2000);
	for (float x = 0; x < 3000; x += 16.f)
		m_system.add(TestSystem::Particle(sf::Color::Red, sf::Vector2f(x, 0.f), 0.f));
}

void	ParticleDemoScreen::pause()
{

}

void	ParticleDemoScreen::resume()
{

}

void	ParticleDemoScreen::stop()
{

}

void	ParticleDemoScreen::update(sf::Time frameTime)
{
	m_system.update(frameTime);
}

void	ParticleDemoScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
	m_system.draw(render);
}
