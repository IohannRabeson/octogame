/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PointHandle.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/16 18:41:27 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/23 04:17:26 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POINTHANDLE_HPP
# define POINTHANDLE_HPP
# include <SFML/Graphics/CircleShape.hpp>

class PointHandle : public sf::Drawable
{
public:
	typedef std::function<void(sf::Vector2f const&)>	MoveCallback;

	PointHandle(sf::Color const& color = sf::Color::Green,
				float radius = 8.f) :
		m_shape(radius, 32),
		m_radius(radius),
		m_handled(false)
	{
		m_shape.setFillColor(sf::Color::Transparent);
		m_shape.setOutlineColor(color);
		setHandled(false);
	}

	PointHandle(sf::Color const& color,
				float radius,
				MoveCallback&& callback) :
		PointHandle(color, radius)
	{
		m_moveCallback = callback;
	}

	void	setMoveCallback(MoveCallback&& callback)
	{
		m_moveCallback = callback;
	}

	void	setHandled(bool handled)
	{
		m_shape.setOutlineThickness((handled) ? 4.f : 2.f);
		m_shape.setOrigin(sf::Vector2f(m_radius, m_radius));
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

	void	setPosition(sf::Vector2f const& position, bool forceCallback)
	{
		if (forceCallback == false && isHandled() == false)
			return;
		m_shape.setPosition(position);
		if (m_moveCallback)
			m_moveCallback(position);
	}

	void	setPosition(sf::Vector2f const& position)
	{
		if (isHandled() == false)
			return;
		m_shape.setPosition(position);
		if (m_moveCallback)
			m_moveCallback(position);
	}

	sf::Vector2f const& getPosition()const
	{
		return (m_shape.getPosition());
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const
	{
		target.draw(m_shape, states);
	}
private:
	sf::CircleShape	m_shape;
	MoveCallback	m_moveCallback;
	float			m_radius;
	bool			m_handled;
};

#endif
