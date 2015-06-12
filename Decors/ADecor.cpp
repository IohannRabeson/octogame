/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADecor.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/08 10:03:59 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/12 16:31:46 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ADecor.hpp"

void	ADecor::setPosition(sf::Vector2f const& pos)
{
	m_position = pos;
}

sf::Vector2f const&	ADecor::getPosition()const
{
	return (m_position);
}
