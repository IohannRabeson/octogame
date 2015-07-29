/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:16:26 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/29 13:09:47 by jbalestr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_HPP
# define GAME_HPP

# include "GameClock.hpp"
# include "BiomeManager.hpp"
# include "SkyManager.hpp"
# include "GroundManager.hpp"
# include "ParallaxScrolling.hpp"
# include "PhysicsEngine.hpp"
# include "IContactListener.hpp"

class AShape;

class Game : public octo::DefaultKeyboardListener, public IContactListener
{
public:
	Game();

	void	setup();
	void	loadLevel(std::string const& fileName);

	void	update(sf::Time frameTime);
	void	draw(sf::RenderTarget& render, sf::RenderStates states)const;
	void	onShapeCollision(AShape * shapeA, AShape * shapeB);

private:
	GameClock			m_gameClock;
	BiomeManager		m_biomeManager;
	SkyManager			m_skyManager;
	GroundManager		m_groundManager;
	ParallaxScrolling	m_parallaxScrolling;
	PhysicsEngine &		m_physicsEngine;

	bool onPressed(sf::Event::KeyEvent const & event);

};

#endif
