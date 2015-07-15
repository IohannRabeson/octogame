/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CharacterDemoScreen.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/15 19:50:39 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/15 20:36:57 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CharacterDemoScreen.hpp"
#include "ACharacter.hpp"

class Tama : public ACharacter
{
public:

private:

};

CharacterDemoScreen::CharacterDemoScreen()
{
}

void	CharacterDemoScreen::start()
{

}

void	CharacterDemoScreen::pause()
{

}

void	CharacterDemoScreen::resume()
{

}

void	CharacterDemoScreen::stop()
{

}

void	CharacterDemoScreen::update(sf::Time frameTime)
{
	(void)frameTime;
}

void	CharacterDemoScreen::draw(sf::RenderTarget& render)const
{
	render.clear();
}
