/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlitchEffectManager.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/13 23:55:53 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 15:57:49 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GlitchEffectManager.hpp"

#include <limits>
#include <ctime>

// 
// class GlitchEffectManager::AGlitch
//
GlitchEffectManager::AGlitch::~AGlitch()
{
}

//
// class GlitchEffectManager
// 
GlitchEffectManager::GlitchEffectManager() :
	m_rnd(std::time(0)),
	m_glitch(nullptr)
{
}

std::size_t	GlitchEffectManager::addGlitch(std::unique_ptr<AGlitch>&& glitch)
{
	std::size_t	index = m_prototypes.size();

	m_prototypes.emplace_back(std::move(glitch));
	m_randomKey.param(decltype(m_randomKey)::param_type(0u, m_prototypes.size() - 1u));
	return (index);
}

void	GlitchEffectManager::startRandomGlitch(sf::Time duration)
{
	auto	key = m_randomKey(m_rnd);

	m_glitch = m_prototypes.at(key).get();
	m_elapsedTime = sf::Time::Zero;
	m_glitchDuration = duration;
	m_glitch->start();
}

void	GlitchEffectManager::update(sf::Time frameTime)
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
