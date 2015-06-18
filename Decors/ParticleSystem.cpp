/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystem.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/18 01:43:58 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/18 01:49:35 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


ParticleSystem::ParticleSystem() :
	m_vertexCount(0u),
	m_vertexUsed(0u),
	m_particleVertexCount(0u),
	m_maxParticleCount(0u)
{
}

void	ParticleSystem::update(sf::Time frameTime)
{
	for (std::size_t i = 0u; i < m_vertexUsed; ++i)
		transformParticle(m_vertices[i]->position);
}

void	ParticleSystem:	draw(sf::RenderTarget& render, sf::RenderStates states)const
{

}
