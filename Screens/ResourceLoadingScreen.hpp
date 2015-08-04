/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceLoadingScreen.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/30 01:20:21 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/06 08:10:41 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESOURCELOADINGSCREEN_HPP
# define RESOURCELOADINGSCREEN_HPP
# include <AbstractResourceLoadingState.hpp>

# include "FireflySwarm.hpp"
# include "FireflyPopulation.hpp"

# include <string>

# include <SFML/Graphics/Text.hpp>
# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Sprite.hpp>

class ResourceLoadingScreen : public octo::AbstractResourceLoadingState
{
public:
	ResourceLoadingScreen();
private:
	virtual void	start();
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
	virtual void	onNoMoreLoading();	
private:
	sf::Texture			m_startTexture;
	sf::Sprite			m_startSprite;
	sf::Text			m_message;
	sf::RectangleShape	m_borders;
	sf::RectangleShape	m_bar;
};

#endif
