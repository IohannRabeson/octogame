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
	m_cycle(cycles),
	m_factor(factor)
{
}

void	PersistancePotion::updateShader(sf::Time, float relativeTime, sf::Shader& shader)
{
	float	x = (std::sin(relativeTime * octo::Pi * m_cycle) * 0.5f + 1.f) * m_factor;
	float	t = 1.f - relativeTime;

	shader.setParameter("intensity", x * t);
}

void	PersistancePotion::setupShader(sf::Shader& shader)
{
	shader.setParameter("intensity", 0.f);
}

