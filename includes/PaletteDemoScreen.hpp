/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PaletteDemoScreen.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/19 21:23:20 by irabeson          #+#    #+#             */
/*   Updated: 2015/05/01 03:44:59 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PALETTEDEMOSCREEN_HPP
# define PALETTEDEMOSCREEN_HPP
# include <AbstractState.hpp>
# include <IColorProvider.hpp>

# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/View.hpp>

# include <vector>

class PaletteDemoScreen : public octo::AbstractState
{
public:
	explicit PaletteDemoScreen(octo::IColorProvider const* colors = nullptr);

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
	void			setColors(octo::IColorProvider const* colors);
private:
	void			updateColors();
private:
	octo::IColorProvider const*			m_colors;
	std::vector<sf::RectangleShape>		m_rectangles;
	sf::View							m_view;
};

#endif
