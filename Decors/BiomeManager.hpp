/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/11 13:37:15 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/11 16:07:16 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIOMEMANAGER_HPP
# define BIOMEMANAGER_HPP
# include "ABiome.hpp"

# include <GenericFactory.hpp>

class BiomeManager
{
public:
	ABiome&	getCurrent();

	template <class T>
	void	registerBiome(std::string const& key)
	{
		m_factory.registerCreator<T>(key);
	}
private:
	octo::GenericFactory<std::string, ABiome>	m_factory;
	std::unique_ptr<ABiome>						m_biome;
};

#endif
