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

# include "ResourceDefinitions.hpp"

# include <string>
# include <vector>

# include <SFML/System.hpp>
# include <SFML/Graphics/Text.hpp>
# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Sprite.hpp>

class ResourceLoadingScreen : public octo::AbstractResourceLoadingState
{
public:
	ResourceLoadingScreen();
private:
	virtual void	start();
	void			updateLoading();
	void			updateScreen(sf::Time frameTime);
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
	virtual void	onNoMoreLoading();	
private:
	std::size_t					m_count;
	std::vector<ResourceKey>	m_key;
	std::vector<sf::Texture>	m_startTextures;
	std::vector<sf::Sprite>		m_startSprites;
	std::size_t					m_index;
	sf::Time					m_timer;
	std::vector<sf::Time>		m_timerMax;
	sf::Font					m_font;
	std::string					m_string;
	sf::Text					m_message;
	sf::RectangleShape			m_borders;
	sf::RectangleShape			m_bar;
};

#endif
