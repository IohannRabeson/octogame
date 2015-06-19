/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystem.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/18 21:36:14 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/19 23:33:12 by irabeson         ###   ########.fr       */
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
# include <memory>

# include <VertexBuilder.hpp>

//	TODO: this file must be moved into octolib!

/*!	\ingroup Graphics
 *	\class ParticleSystem
 *	\tparam C Custom component
 *
 *	<h1>Particle is an entity</h1>
 *	Particle management is based on Entity Component System pattern (ECS).
 *	A particle is an entity and you can define at compile time the list
 *	of theses compenents. In other words, you can define at compile time
 *	the contents of the particles for this system.
 *
 *	Exemple, you need to know the life time of each particle, you
 *	can define a ParticleSystem as follow:
 *	\code
 *	class YourParticleSystem : public octo::ParticleSystem<sf::Time>
 *	{
 *		...
 *	};
 *	\endcode
 *	We add an sf::Time as template parameter of the ParticleSystem inerited by YourParticleSystem.
 *	\endcode
 *	Now we need to increase the time elapsed for each particles and kill particles which are
 *	to old. Also, we need our particles move at each frame.
 *	\code
 *	class YourParticleSystem : public octo::ParticleSystem<sf::Time>
 *	{
 *	private:
 *		// This method is called at each frame with each particle
 *		virtual void	updateParticle(sf::Time frameTime, Particle& particle)
 *		{
 *			static sf::Vector2f const	Velocity(0.2f, 512.f);
 *
 *			// increase the life time
 *			std::get<Component::User>(particle) = std::get<Component::User>(particle) + frameTime.asSeconds();
 *			// move the particle
 *			std::get<Component::Position>(particle) = std::get<Component::Position>(particle) + (Velocity * frameTime.asSeconds());
 *		}
 *
 *		// This methods is called when the system is looking for dead particles
 *		virtual bool	isDeadParticle(Particle const& particle)
 *		{
 *			// Particle live 2 seconds only...
 *			return (std::get<Component::User>(particle) >= sf::second(2.f));
 *		}
 *	};
 *	\endcode
 */
template <class ... C>
class ParticleSystem : public sf::Transformable
{
public:
	typedef std::vector<sf::Vector2f>	Prototype;
	typedef std::tuple<sf::Color,
					   sf::Vector2f,
					   float,
					   C...>			Particle;

	enum Component
	{
		Color = 0u,
		Position = 1u,
		Rotation = 2u,
		User = 3u
	};

	ParticleSystem();
	virtual ~ParticleSystem() = default;

	/*!	Reset the system
	 *	\param prototype Point collection used to build particle prototype
	 *	\param type	Primitive type used to render particles
 	 *	\param maxParticleCount Define the maximum of particles
	 *
	 *	Allocate the memory and prepare the system to run.
	 */	 
	void			reset(Prototype const& prototype,
						  sf::PrimitiveType type,
						  std::size_t maxParticleCount);

	/*!	Add a new particle */
	void			add(Particle const& particle);

	/*!	Add a new particle */
	void			add(Particle&& particle);

	/*!	Add a new particle */
	template <class ... T>
	void			emplace(T&& ... args);

	/*!	Remove all particles */
	void			clear();

	virtual void	update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const;
private:
	/*!	This methods is called each frame with each particle */
	virtual void	updateParticle(sf::Time frameTime, Particle& particle) = 0;

	/*!	This methods is called when the system removes the dead particles */
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
