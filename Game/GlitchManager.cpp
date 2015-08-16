/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlitchManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/13 23:55:53 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 01:16:26 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GlitchManager.hpp"

#include <limits>

GlitchManager::GlitchManager() :
	m_glitch(nullptr)
{
}

std::size_t	GlitchManager::addGlitch(std::unique_ptr<AGlitch>&& glitch)
{
	std::size_t	index = m_prototypes.size();

	m_prototypes.emplace_back(std::move(glitch));
	m_randomKey.param(decltype(m_randomKey)::param_type(0u, m_prototypes.size() - 1u));
	return (index);
}

void	GlitchManager::startRandomGlitch(sf::Time duration)
{
	auto	key = m_randomKey(m_rnd);

	m_glitch = m_prototypes.at(key).get();
	m_elapsedTime = sf::Time::Zero;
	m_glitchDuration = duration;
	m_glitch->start();
}

void	GlitchManager::update(sf::Time frameTime)
{
	if (m_glitch)
	{
		m_elapsedTime += frameTime;
		if (m_elapsedTime > m_glitchDuration)
		{
			m_glitch->stop();
			m_glitch = nullptr;
			m_glitchDuration = sf::Time::Zero;
		}
	}
}
