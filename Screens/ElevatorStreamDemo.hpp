/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ElevatorStreamDemo.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/08/01 04:24:26 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/07 16:38:42 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELEVATORSTREAMDEMO_HPP
# define ELEVATORSTREAMDEMO_HPP
# include <AbstractState.hpp>

# include "ElevatorStream.hpp"

class ElevatorStreamDemo : public octo::AbstractState
{
public:
	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
private:
	ElevatorStream	m_teleportBeam;
};

#endif
