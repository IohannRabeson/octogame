/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:20:19 by irabeson          #+#    #+#             */
/*   Updated: 2015/09/16 17:53:19 by jbalestr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BiomeManager.hpp"
#include "ABiome.hpp"

#include <cassert>

void	BiomeManager::changeBiome(Level key, std::size_t seed)
{
	std::unique_ptr<ABiome>	newBiome(m_factory.create(key));

	if (newBiome)
	{
		m_biome = std::move(newBiome);
		m_biome->setup(seed);
	}
}

ABiome&	BiomeManager::getCurrentBiome()
{
	assert(m_biome != nullptr);
	return (*m_biome);
}
