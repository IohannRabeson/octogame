/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PointHandle.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/16 18:41:27 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/16 18:59:54 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POINTHANDLE_HPP
# define POINTHANDLE_HPP
# include <SFML/Graphics/CircleShape.hpp>

class PointHandle : public sf::Drawable
{
public:
	PointHandle(sf::Color const& color = sf::Color::Green,
				float radius = 8.f) :
		m_shape(8.f, 32),
		m_handled(false)
	{
		m_shape.setOrigin(sf::Vector2f(radius / 2.f, radius / 2.f));
		m_shape.setFillColor(sf::Color::Transparent);
		m_shape.setOutlineColor(color);
		setHandled(false);
	}

	void	setHandled(bool handled)
	{
		m_shape.setOutlineThickness((handled) ? 6.f : 3.f);
		m_handled = handled;
	}

	bool	isHandled()const
	{
		return (m_handled);
	}

	bool	hitTest(sf::Vector2f const& position)const
	{
		return (m_shape.getGlobalBounds().contains(position));
	}

	void	setPosition(sf::Vector2f const& position)
	{
		m_shape.setPosition(position);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_shape, states);
	}
private:
	sf::CircleShape	m_shape;
	bool			m_handled;
};

#endif
