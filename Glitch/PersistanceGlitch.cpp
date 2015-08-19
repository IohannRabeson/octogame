/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PersistanceGlitch.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 16:05:36 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 16:06:09 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PersistanceGlitch.hpp"

#include <ResourceDefinitions.hpp>

PersistanceGlitch::PersistanceGlitch(float intensity) :
	PostEffectGlitch(PERSISTANCE_FRAG),
	m_intensity(intensity)
{
}

void	PersistanceGlitch::setupShader(sf::Shader& shader)
{
	shader.setParameter("intensity", m_intensity);
}
