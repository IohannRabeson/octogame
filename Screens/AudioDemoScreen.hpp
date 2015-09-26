/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AudioDemoScreen.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/25 22:06:12 by irabeson          #+#    #+#             */
/*   Updated: 2015/08/12 09:59:15 by jbalestr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIODEMOSCREEN_HPP
# define AUDIODEMOSCREEN_HPP
# include <AbstractState.hpp>
# include <DefaultGraphicsListeners.hpp>

# include <functional>

# include "PointHandle.hpp"

class AudioDemoScreen : public octo::AbstractState,
						public octo::DefaultMouseListener
{
public:
	AudioDemoScreen();
	~AudioDemoScreen();

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;

	virtual void	onMoved(sf::Event::MouseMoveEvent const& event);
	virtual void	onPressed(sf::Event::MouseButtonEvent const&);
	virtual void	onReleased(sf::Event::MouseButtonEvent const&);
private:
	PointHandle	m_listenerPoint;
};

#endif
