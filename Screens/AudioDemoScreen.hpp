/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AudioDemoScreen.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/25 22:06:12 by irabeson          #+#    #+#             */
/*   Updated: 2015/05/25 22:24:17 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIODEMOSCREEN_HPP
# define AUDIODEMOSCREEN_HPP
# include <AbstractState.hpp>
# include <DefaultGraphicsListeners.hpp>

class AudioDemoScreen : public octo::AbstractState,
							 	 public octo::DefaultKeyboardListener
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

	bool			onPressed(sf::Event::KeyEvent const& event);
private:
};

#endif
