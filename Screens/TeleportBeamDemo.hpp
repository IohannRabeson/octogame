/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TeleportBeamDemo.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/01 04:24:26 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/01 04:27:15 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TELEPORTBEAMDEMO_HPP
# define TELEPORTBEAMDEMO_HPP
# include <AbstractState.hpp>

# include "TeleportBeam.hpp"

class TeleportBeamDemo : public octo::AbstractState
{
public:
	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
private:
	TeleportBeam	m_teleportBeam;
};

#endif
