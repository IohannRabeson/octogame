/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystem.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/18 21:36:14 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/19 07:09:36 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLESYSTEM_HPP
# define PARTICLESYSTEM_HPP
# include <SFML/Graphics/Drawable.hpp>
# include <SFML/Graphics/Transformable.hpp>
# include <SFML/Graphics/Vertex.hpp>
# include <SFML/Graphics/PrimitiveType.hpp>
# include <SFML/System/Time.hpp>

# include <functional>
# include <list>
# include <tuple>

# include <VertexBuilder.hpp>

template <class ... P>
class ParticleSystem : public sf::Transformable
{
public:
	typedef std::vector<sf::Vector2f>	Prototype;
	typedef std::tuple<sf::Color,
					   sf::Vector2f,
					   float,
					   P...>			Particle;

	enum Component
	{
		Color = 0u,
		Position = 1u,
		Rotation = 2u,
		User = 3u
	};

	ParticleSystem();
	virtual ~ParticleSystem() = default;

	void			reset(Prototype const& prototype,
						  sf::PrimitiveType type,
						  std::size_t maxParticleCount);

	void			add(Particle const& particle);
	void			add(Particle&& particle);
	void			clear();

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const;
private:
	virtual void	updateParticle(sf::Time frameTime, Particle& particle) = 0;
	virtual bool	isDeadParticle(Particle const& particle) = 0;
private:
	std::list<Particle>			m_particles;
	std::unique_ptr<sf::Vertex>	m_vertices;
	std::size_t					m_verticesCount;
	Prototype					m_prototype;
	sf::PrimitiveType			m_primitiveType;
	octo::VertexBuilder			m_builder;
};

#include "ParticleSystem.hxx"
#endif
