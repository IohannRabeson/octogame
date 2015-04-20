/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PaletteDemoScreen.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/19 21:23:20 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/20 15:37:53 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PALETTEDEMOSCREEN_HPP
# define PALETTEDEMOSCREEN_HPP
# include <AbstractState.hpp>
# include <Palette.hpp>
# include <Array2D.hpp>
# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/View.hpp>

class PaletteDemoScreen : public octo::AbstractState
{
public:
	PaletteDemoScreen();

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
private:
	octo::Palette						m_palette;
	octo::Array2D<sf::RectangleShape>	m_grid;
	sf::View							m_view;
};

#endif
