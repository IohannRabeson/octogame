/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:16:26 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/17 12:43:28 by pciavald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_HPP
# define GAME_HPP

# include "GameClock.hpp"
# include "BiomeManager.hpp"
# include "SkyManager.hpp"
# include "GroundManager.hpp"
# include "ParallaxScrolling.hpp"

class Game : public octo::DefaultKeyboardListener
{
public:
	Game();

	void			setup();
	void			loadLevel(std::string const& fileName);

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render, sf::RenderStates states)const;

private:
	GameClock			m_gameClock;
	BiomeManager		m_biomeManager;
	SkyManager			m_skyManager;
	GroundManager		m_groundManager;
	ParallaxScrolling	m_parallaxScrolling;

	bool onPressed(sf::Event::KeyEvent const & event);

};

#endif
