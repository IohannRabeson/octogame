/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PaletteViewer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/20 22:26:43 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/20 22:28:48 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PaletteViewer.hpp"

void	PaletteViewer::setPalette(Palette const& palette)
{
	m_palette = &palette;
}

void	PaletteViewer::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	
}
