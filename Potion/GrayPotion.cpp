/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GrayPotion.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/14 15:10:38 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 15:23:00 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GrayPotion.hpp"
#include "ResourceDefinitions.hpp"

GrayPotion::GrayPotion() :
	PostEffectPotion(XFADE_GRAY_FRAG, sf::seconds(4.f))
{
}

void	GrayPotion::updateShader(sf::Time, float relativeTime, sf::Shader& shader)
{
	shader.setParameter("crossfade", relativeTime);	
}
