/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Firefly.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/16 16:42:18 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/16 17:08:36 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Firefly.hpp"

void	update(sf::Time frameTime)
{
	int	const		newIntegerPart = static_cast<int>(m_pathPosition);
	sf::Vector2f	newPos;

	m_pathPosition += m_speed * frameTime.asSeconds();
	setPosition(octo::bsplineInterpolation(m_path[0], m_path[1], m_path[2], m_path[3], m_pathPosition));
	if (newIntegerPart > m_oldIntegerPosition)
	{
		if (m_path.size() == 7)
		{
			m_path.pop();
			m_pathPosition -= 1.f;
		}
		m_path.emplace(m_behavior->getTargetPosition(m_position, m_target, m_distance));
		m_oldIntegerPosition = newIntegerPart;
	}
}

void	Firefly::setPosition(sf::Vector2f const& position)
{
	static sf::Vector2f const	TopLeft = {-1.f, -1.f};
	static sf::Vector2f const	TopRight = {1.f, -1.f};
	static sf::Vector2f const	BottomRight = {1.f, 1.f};
	static sf::Vector2f const	BottomLeft = {-1.f, 1.f};
	std::size_t	const			Offset = m_id * 4u;

	m_firstVertex[0].position = position + TopLeft * m_radius;
	m_firstVertex[1].position = position + TopRight * m_radius;
	m_firstVertex[2].position = position + BottomRight * m_radius;
	m_firstVertex[3].position = position + BottomLeft * m_radius;
}

