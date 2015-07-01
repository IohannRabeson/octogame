/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:16:26 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/24 06:04:53 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_HPP
# define GAME_HPP
# include "BiomeManager.hpp"
# include "DecorManager.hpp"
# include "TerrainManager.hpp"
# include "MapManager.hpp" // TODO: remove ???

class Game
{
public:
	Game();

	void			setup();
	void			loadLevel(std::string const& fileName);

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states)const;
private:
	TerrainManager	m_terrainManager;
	BiomeManager	m_biomeManager;
	DecorManager	m_skyDecorManager;
	MapManager		m_mapManager;
	Biome			m_biome;	// TODO: remove
	// @Francois: tu peux fourrer ici des decors manager:
	// DecorManager	m_starDecorManager;
	// DecorManager m_terrainDecorManager;
};

#endif
