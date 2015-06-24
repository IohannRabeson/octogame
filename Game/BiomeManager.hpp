/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:16:51 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/24 05:57:53 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIOMEMANAGER_HPP
# define BIOMEMANAGER_HPP
# include <NonCopyable.hpp>
# include <GenericFactory.hpp>

# include <string>
# include <memory>

class ABiome;

class BiomeManager : public octo::NonCopyable
{
public:
	void	changeBiome(std::string const& key, std::size_t seed);
	
	template <class T>
	void	registerBiome(std::string const& key);

	ABiome&	getCurrentBiome();
private:
	typedef octo::GenericFactory<std::string, ABiome>	BiomeFactory;

	BiomeFactory			m_factory;
	std::unique_ptr<ABiome>	m_biome;
};

template <class T>
void	BiomeManager::registerBiome(std::string const& key)
{
	m_factory.registerCreator<T>(key);
}

#endif
