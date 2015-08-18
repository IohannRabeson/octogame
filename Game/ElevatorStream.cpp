/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ElevatorStream.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/01 04:30:42 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/13 22:27:23 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ElevatorStream.hpp"
#include "ABiome.hpp"
#include "ResourceDefinitions.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Math.hpp>
#include <Interpolations.hpp>

#include <random>
#include <ctime>

class ElevatorStream::BeamParticle : public octo::ParticleSystem<sf::Time, float, float>
{
	enum MyComponent
	{
		ElapsedTime = Component::User,
		UpSpeed,
		AngularVelocity
	};
public:
	BeamParticle() :
		m_cycleTime(sf::seconds(4)),
		m_speedUp(512.f),
		m_width(200.f),
		m_height(300.f),
		m_rotationFactor(0.f),
		m_emitInterval(sf::seconds(0.025f)),
		m_random(std::time(0)),
		m_distri(0.f, 1.f),
		m_biome(nullptr)
	{
		float const	size = 16.f;

		reset({sf::Vertex({0.f, 0.f}),
			   sf::Vertex({size, 0.f}),
			   sf::Vertex({size, size})},
			   sf::Triangles, 1000u);

	}

	void	setWidth(float width)
	{
		m_width = width;
	}

	void	setHeight(float height)
	{
		m_height = height;
	}

	void	setRotationFactor(float factor)
	{
		m_rotationFactor = factor;
	}

	void	setBiome(ABiome & biome)
	{
		m_biome = &biome;
	}

	void	updateParticle(sf::Time frameTime, Particle& particle)
	{
		sf::Vector2f	position = std::get<Component::Position>(particle);
		sf::Time		currentTime = std::get<MyComponent::ElapsedTime>(particle) + frameTime;
		sf::Color&		color = std::get<Component::Color>(particle);
		float			cycle = currentTime / m_cycleTime;
		float			heightPos = 0.f;

		position.y -= frameTime.asSeconds() * std::get<MyComponent::UpSpeed>(particle);
		heightPos = (position.y / m_height);
		position.x = std::cos(cycle * octo::Pi2) * m_width * 0.5f;
		color.a = heightPos * 255;
		if (currentTime >= m_cycleTime)
		{
			currentTime -= m_cycleTime;
		}
		std::get<MyComponent::ElapsedTime>(particle) = currentTime;
		std::get<Component::Position>(particle) = position;
		std::get<Component::Rotation>(particle) += m_rotationFactor * std::get<MyComponent::AngularVelocity>(particle);
		std::get<MyComponent::UpSpeed>(particle) *= 1.0001f;
	}

	void	update(sf::Time frameTime)
	{
		bool	emitParticle = false;

		m_emitTimer += frameTime;
		while (m_emitTimer > m_emitInterval)
		{
			emitParticle = true;
			m_emitTimer -= m_emitInterval;
		}
		if (emitParticle)
			createParticle();
		ParticleSystem::update(frameTime);
	}

	void	createParticle()
	{
		//TODO : Voir avec Iohann si on ne devrait pas avoir une instance du biome accessible partout pour eviter ce genre de chose
		sf::Color color;
		if (m_biome)
			color = m_biome->getParticleColorGround();
		else
			color = sf::Color::White;
		emplace(color,
				sf::Vector2f(0, 0),
				sf::Vector2f(1.f, 1.f),
				m_distri(m_random) * 360.f,
				sf::seconds(m_distri(m_random) * m_cycleTime.asSeconds()),
				std::max(0.2f, m_distri(m_random)) * m_speedUp,
				m_distri(m_random) * 720 - 360
				);
	}
private:
	bool	isDeadParticle(Particle const& particle)
	{
		return (std::get<Component::Position>(particle).y < -m_height);
	}
private:
	typedef std::uniform_real_distribution<float>	Distri;

	sf::Time				m_cycleTime;
	float					m_speedUp;
	float					m_width;
	float					m_height;
	float					m_rotationFactor;
	sf::Time				m_emitTimer;
	sf::Time				m_emitInterval;
	std::mt19937			m_random;
	Distri					m_distri;
	ABiome *				m_biome;
};

ElevatorStream::ElevatorStream() :
	m_particles(new BeamParticle()),
	m_waveCycleDuration(sf::seconds(0.5))
{
	octo::ResourceManager&	resources = octo::Application::getResourceManager();

	m_particles->setWidth(150.f);
	m_shader.loadFromMemory(resources.getText(ELEVATOR_VERT), sf::Shader::Vertex);
	m_shader.setParameter("wave_amplitude", 5.f);
}

void	ElevatorStream::setPosX(float x)
{
	sf::Vector2f	pos = m_particles->getPosition();

	pos.x = x;
	m_particles->setPosition(pos);
}

void	ElevatorStream::setPosY(float y)
{
	sf::Vector2f	pos = m_particles->getPosition();

	pos.y = y;
	m_particles->setPosition(pos);
}

void	ElevatorStream::setHeight(float height)
{
	m_particles->setHeight(height);
}

void	ElevatorStream::setWidth(float width)
{
	m_particles->setWidth(width);
}

/*!	Set the rotation factor (0.f to 1.f) */
void	ElevatorStream::setRotationFactor(float factor)
{
	m_particles->setRotationFactor(factor);
}

void	ElevatorStream::setBiome(ABiome & biome)
{
	m_particles->setBiome(biome);
}

void	ElevatorStream::update(sf::Time frameTime)
{
	m_particles->update(frameTime);
	m_waveCycle += frameTime;
	m_shader.setParameter("wave_phase", m_waveCycle.asSeconds());
}

void	ElevatorStream::draw(sf::RenderTarget& render)const
{
	sf::RenderStates	states;

	states.shader = &m_shader;
	m_particles->draw(render, states);
}
