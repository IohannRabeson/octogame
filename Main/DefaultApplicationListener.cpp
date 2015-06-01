/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultApplicationListener.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/15 19:49:40 by irabeson          #+#    #+#             */
/*   Updated: 2015/05/29 17:45:59 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DefaultApplicationListener.hpp"
#include <Application.hpp>
#include <Console.hpp>

void	DefaultApplicationListener::onClosed()
{
	octo::Application::stop();
}

void	DefaultApplicationListener::onResized(sf::Event::SizeEvent const&)
{
}

void	DefaultApplicationListener::onFocusGained()
{
}

void	DefaultApplicationListener::onFocusLost()
{
}

bool	DefaultApplicationListener::onPressed(sf::Event::KeyEvent const& event)
{
	switch (event.code)
	{
		case sf::Keyboard::Escape:
			octo::Application::stop();
			break;
		case sf::Keyboard::F5:
			octo::Application::getConsole().setEnabled(true);
			break;
		default:
			break;
	}
	return (true);
}

bool	DefaultApplicationListener::onReleased(sf::Event::KeyEvent const&)
{
	return (true);
}
