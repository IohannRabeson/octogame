/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/11 13:40:24 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/11 16:07:31 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BiomeManager.hpp"
#include "ABiome.hpp"

#include <cassert>

ABiome&	BiomeManager::getCurrent()
{
	assert(m_biome != nullptr);
	return (*m_biome);
}
