/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystem.hxx                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/19 00:55:39 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/19 16:43:31 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transform.hpp>

//
//	ParticleSystem
//
template <class ... P>
ParticleSystem<P...>::ParticleSystem() :
	m_verticesCount(0u),
	m_primitiveType(sf::Triangles)
{
}

template <class ... P>
void	ParticleSystem<P...>::reset(Prototype const& prototype,
									 sf::PrimitiveType type,
									 std::size_t maxParticleCount)
{
	m_prototype = prototype;
	m_primitiveType = type;
	m_vertices.reset(new sf::Vertex[maxParticleCount * m_prototype.size()]);
	m_verticesCount = maxParticleCount * m_prototype.size();
	m_builder = octo::VertexBuilder(m_vertices.get(), m_verticesCount);
}

template <class ... P>
void	ParticleSystem<P...>::add(Particle const& particle)
{
	m_particles.push_front(particle);
}

template <class ... P>
void	ParticleSystem<P...>::add(Particle&& particle)
{
	m_particles.push_front(particle);
}

template <class ... P>
void	ParticleSystem<P...>::clear()
{
	m_particles.clear();
}

template <class ... P>
void	ParticleSystem<P...>::update(sf::Time frameTime)
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

template <class ... P>
void	ParticleSystem<P...>::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	states.transform *= getTransform();
	render.draw(m_vertices.get(), m_builder.getUsed(), m_primitiveType, states);
}
