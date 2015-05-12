/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultApplicationListener.hpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/15 19:47:55 by irabeson          #+#    #+#             */
/*   Updated: 2015/05/08 18:35:42 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFAULTAPPLICATIONLISTENER_HPP
# define DEFAULTAPPLICATIONLISTENER_HPP
# include <GraphicsManager.hpp>
# include <GraphicsListeners.hpp>

class DefaultApplicationListener : public octo::IKeyboardListener,
								   public octo::IWindowListener
{
public:
	virtual void	onClosed();
	virtual void	onResized(sf::Event::SizeEvent const& event);
	virtual void	onFocusGained();
	virtual void	onFocusLost();
	virtual bool	onPressed(sf::Event::KeyEvent const& event);
	virtual bool	onReleased(sf::Event::KeyEvent const& event);
};

#endif
