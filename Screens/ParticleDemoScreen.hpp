/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleDemoScreen.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/19 05:58:50 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/24 01:09:49 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLEDEMOSCREEN_HPP
# define PARTICLEDEMOSCREEN_HPP
# include <AbstractState.hpp>
# include <DefaultGraphicsListeners.hpp>
# include <ParticleSystem.hpp>

# include "RainSystem.hpp"

# include <random>

class TestSystem : public octo::ParticleSystem<sf::Time, sf::Time, float>,
				   public octo::DefaultMouseListener
{
public:
	enum MyComponent
	{
		Time = User,
		Life,
		Direction
	};

	TestSystem();

	void			update(sf::Time frameTime);	
private:
	virtual void	updateParticle(sf::Time frameTime, Particle& particle);
	virtual bool	isDeadParticle(Particle const& particle);

	virtual void	onMoved(sf::Event::MouseMoveEvent const&);
	virtual void	onPressed(sf::Event::MouseButtonEvent const&);
	virtual void	onReleased(sf::Event::MouseButtonEvent const&);
private:
	typedef std::uniform_real_distribution<float>	Dist;

	sf::Vector2f	m_emitter;
	bool			m_emit;
	std::mt19937	m_engine;
	Dist			m_lifeTimeDistri;
	Dist			m_directionDistri;
};

class ParticleDemoScreen : public octo::AbstractState
{
private:
	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
private:
	TestSystem	m_system;
	RainSystem	m_rainSystem;
};

#endif
