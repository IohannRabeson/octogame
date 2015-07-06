/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LightningDemoScreen.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/23 02:38:53 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/23 02:56:52 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHTNINGDEMOSCREEN_HPP
# define LIGHTNINGDEMOSCREEN_HPP
# include <AbstractState.hpp>
# include <DefaultGraphicsListeners.hpp>

# include "Lightning.hpp"
# include "PointHandle.hpp"

class LightningDemoScreen : public octo::AbstractState,
							public octo::DefaultMouseListener,
							public octo::DefaultKeyboardListener
{
public:
	LightningDemoScreen();

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
	virtual bool	onPressed(sf::Event::KeyEvent const& event);
	virtual bool	onReleased(sf::Event::KeyEvent const& event);
	virtual void	onMoved(sf::Event::MouseMoveEvent const& event);
	virtual void	onPressed(sf::Event::MouseButtonEvent const& event);
	virtual void	onReleased(sf::Event::MouseButtonEvent const& event);
private:
	Lightning	m_lightning;
	PointHandle	m_p0;
	PointHandle	m_p1;
};

#endif
