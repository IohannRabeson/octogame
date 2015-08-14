/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PixelGlitch.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 16:05:36 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 16:06:09 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PixelGlitch.hpp"

#include <ResourceDefinitions.hpp>

PixelGlitch::PixelGlitch(float pixelThreshold) :
	PostEffectGlitch(PIXELATE_FRAG),
	m_pixelThreshold(pixelThreshold)
{
}

void	PixelGlitch::setupShader(sf::Shader& shader)
{
	shader.setParameter("pixel_threshold", m_pixelThreshold);
}
