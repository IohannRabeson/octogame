/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PaletteViewer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/20 22:21:55 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/20 22:26:35 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PALETTEVIEWER_HPP
# define PALETTEVIEWER_HPP

class PaletteViewer : public sf::Drawable
{
public:
	void	setPalette(Palette const& palette);

	void	draw(sf::RenderTarget& render, sf::RenderStates states)const;
private:
	Palette const*	m_palette;
};

#endif
