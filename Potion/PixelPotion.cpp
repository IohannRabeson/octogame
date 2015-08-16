/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PixelPotion.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 03:36:56 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 13:00:33 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PixelPotion.hpp"
#include "ResourceDefinitions.hpp"

#include <PostEffectManager.hpp>
#include <ResourceManager.hpp>
#include <Application.hpp>
#include <Math.hpp>

PixelPotion::PixelPotion(sf::Time duration, float cycles, float factor) :
	PostEffectPotion(PIXELATE_FRAG, duration),
	m_cycle(cycles),
	m_factor(factor)
{
}

void	PixelPotion::updateShader(sf::Time, float relativeTime, sf::Shader& shader)
{
	float	x = (std::sin(relativeTime * octo::Pi * m_cycle) * 0.5f + 1.f) * m_factor;
	float	t = 1.f - relativeTime;

	shader.setParameter("pixel_threshold", x * t);
}

void	PixelPotion::setupShader(sf::Shader& shader)
{
	shader.setParameter("pixel_threshold", 0.f);
}

