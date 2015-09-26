/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:16:51 by irabeson          #+#    #+#             */
/*   Updated: 2015/09/16 17:53:20 by jbalestr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIOMEMANAGER_HPP
# define BIOMEMANAGER_HPP
# include <NonCopyable.hpp>
# include <GenericFactory.hpp>
# include <memory>
# include "ABiome.hpp"

class BiomeManager : public octo::NonCopyable
{
public:
	void	changeBiome(Level key, std::size_t seed);
	
	template <class T>
	void	registerBiome(Level key);

	ABiome&	getCurrentBiome();
private:
	typedef octo::GenericFactory<Level, ABiome>	BiomeFactory;

	BiomeFactory			m_factory;
	std::unique_ptr<ABiome>	m_biome;
};

template <class T>
void	BiomeManager::registerBiome(Level key)
{
	m_factory.registerCreator<T>(key);
}

#endif
