/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystem.hxx                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/19 00:55:39 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/21 01:35:11 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transform.hpp>

//
//	ParticleSystem
//
template <class ... C>
ParticleSystem<C...>::ParticleSystem() :
	m_verticesCount(0u),
	m_primitiveType(sf::Triangles),
	m_maxParticleCount(0u)
{
}

template <class ... C>
void	ParticleSystem<C...>::reset(Prototype const& prototype,
									 sf::PrimitiveType type,
									 std::size_t maxParticleCount)
{
	m_prototype = prototype;
	m_primitiveType = type;
	m_vertices.reset(new sf::Vertex[maxParticleCount * m_prototype.size()]);
	m_verticesCount = maxParticleCount * m_prototype.size();
	m_builder = octo::VertexBuilder(m_vertices.get(), m_verticesCount);
	m_maxParticleCount = maxParticleCount;
}

template <class ... C>
void	ParticleSystem<C...>::add(Particle const& particle)
{
	m_particles.push_front(particle);
}

template <class ... C>
void	ParticleSystem<C...>::add(Particle&& particle)
{
	m_particles.push_front(particle);
}

template <class ... C>
template <class ... T>
void	ParticleSystem<C...>::emplace(T&& ... args)
{
	m_particles.emplace_front(std::forward<T>(args)...);
}

template <class ... C>
void	ParticleSystem<C...>::clear()
{
	m_particles.clear();
}

template <class ... C>
std::size_t	ParticleSystem<C...>::getCapacity()const
{
	return (m_maxParticleCount - m_particles.size());
}

template <class ... C>
void	ParticleSystem<C...>::update(sf::Time frameTime)
{
	sf::Transform	transform;

	m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(), 
									 [this](Particle const& p)
									 {
									 	return (this->isDeadParticle(p));
									 }), m_particles.end());
	m_builder.clear();
	for (auto& particle : m_particles)
	{
		updateParticle(frameTime, particle);
		transform = sf::Transform::Identity;
		transform.translate(std::get<Component::Position>(particle));
		transform.rotate(std::get<Component::Rotation>(particle));
		for (auto const& point : m_prototype)
			m_builder.createVertex(transform * point, std::get<Component::Color>(particle));	
	}
}

template <class ... C>
void	ParticleSystem<C...>::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	states.transform *= getTransform();
	render.draw(m_vertices.get(), m_builder.getUsed(), m_primitiveType, states);
}
