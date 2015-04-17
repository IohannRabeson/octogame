/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Firefly.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/16 16:38:52 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/16 17:07:39 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIREFLY_HPP
# define FIREFLY_HPP

struct Firefly
{
	RingBuffer<sf::Vector2f>	path;
	float						speed;
	float						radius;
	float						pathPosition;
	bool						alive;
};

#endif
