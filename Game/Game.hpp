/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:16:26 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/01 13:28:33 by pciavald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_HPP
# define GAME_HPP

# include "BiomeManager.hpp"
# include "SkyManager.hpp"
# include "GroundManager.hpp"

class Game
{
public:
	Game();

	void			setup();
	void			loadLevel(std::string const& fileName);

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states)const;
private:
	BiomeManager	m_biomeManager;
	SkyManager		m_skyManager;
	GroundManager	m_groundManager;
};

#endif
