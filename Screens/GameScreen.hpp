/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameScreen.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/24 05:54:08 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/14 04:30:33 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMESCREEN_HPP
# define GAMESCREEN_HPP
# include <AbstractState.hpp>
# include <GraphicsListeners.hpp>

# include "Game.hpp"
# include "Menu.hpp"

class GameScreen : public octo::AbstractState, public octo::DefaultKeyboardListener
{
	virtual void		start();
	virtual void		pause();
	virtual void		resume();
	virtual void		stop();
	virtual void		update(sf::Time frameTime);
	virtual void		draw(sf::RenderTarget& render)const;
private:
	Menu				m_menu;
	bool				m_isMenu;
	Game				m_game;

	virtual bool onPressed(sf::Event::KeyEvent const & event);
};

#endif
