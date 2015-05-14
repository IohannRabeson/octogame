/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StateGame.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/15 19:28:19 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/15 19:29:26 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATEGAME_HPP
# define STATEGAME_HPP
# include <AbstractState.hpp>
# include "MapManager.hpp"

class StateGame : public octo::AbstractState
{
public:
	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time p_deltatime);
	virtual void	draw(sf::RenderTarget& render)const;

private:
	MapManager			m_mapManager;
	sf::RectangleShape	m_back;
};

#endif
