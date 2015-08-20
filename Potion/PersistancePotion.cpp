/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PersistancePotion.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 03:36:56 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 13:00:33 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PersistancePotion.hpp"
#include "ResourceDefinitions.hpp"

#include <PostEffectManager.hpp>
#include <ResourceManager.hpp>
#include <Application.hpp>
#include <Math.hpp>

PersistancePotion::PersistancePotion(sf::Time duration, float cycles, float factor) :
	PostEffectPotion(PERSISTANCE_FRAG, duration),
	m_duration(duration.asSeconds()),
	m_cycle(cycles),
	m_factor(factor),
	m_intensity(1.f)
{
}

void	PersistancePotion::updateShader(sf::Time, float relativeTime, sf::Shader& shader)
{
	(void)m_cycle;
	float stepTime = m_duration / (3.f * m_duration);
	float stepTime2 = 2.f * stepTime;

	if (relativeTime < stepTime)
		m_intensity = (stepTime - relativeTime) * (2.f - stepTime) + m_factor;
	else if (relativeTime < stepTime2)
		m_intensity = m_factor + (1 + sin(relativeTime * 100)) * m_factor;
	else
		m_intensity = (relativeTime - stepTime2) / (1.f - stepTime2) + m_factor;

	if (m_intensity >= 1.f)
		m_intensity = 1.f;
	else if (m_intensity <= m_factor)
		m_intensity = m_factor;
	shader.setParameter("intensity", m_intensity);
}

void	PersistancePotion::setupShader(sf::Shader& shader)
{
	m_intensity = 1.f;
	shader.setParameter("intensity", m_intensity);
}

