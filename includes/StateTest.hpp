/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StateTest.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/15 19:28:19 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/15 19:29:26 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATETEST_HPP
# define STATETEST_HPP
# include <AbstractState.hpp>

class StateTest : public octo::AbstractState
{
public:
	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
};

#endif
