/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleDemoScreen.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/19 05:58:50 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/19 06:48:24 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLEDEMOSCREEN_HPP
# define PARTICLEDEMOSCREEN_HPP
# include <AbstractState.hpp>

# include "ParticleSystem.hpp"

class TestSystem : public ParticleSystem<>
{
private:
	virtual void	updateParticle(sf::Time frameTime, Particle& particle);
	virtual bool	isDeadParticle(Particle const& particle);
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
};

#endif
