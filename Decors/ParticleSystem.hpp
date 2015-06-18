/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystem.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/18 01:38:20 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/18 02:21:21 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLESYSTEM_HPP
# define PARTICLESYSTEM_HPP
# include <stack>

class ParticleSystem
{
	class Particle
	{
	public:
		sf::Vector2f	position;
		float			rotation;
		sf::Time		lifeTime;
	};
public:
	ParticleSystem();

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states)const;
private:
	virtual void	buildParticleVertices(sf::Vertex* vertices, std::size_t count)
	{

	}
private:
	std::vector<sf::Vertex>			m_vertices;
	std::vector<Particle>			m_particles;
	std::size_t						m_vertexPerParticle;
};

#endif
