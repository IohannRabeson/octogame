/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADecor.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/08 02:40:40 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/10 03:02:40 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADECOR_HPP
# define ADECOR_HPP
# include <random>
# include <SFML/System/Time.hpp>
# include <SFML/System/Vector2.hpp>

class DecorBuilder;
class ABiome;

class ADecor
{
public:
	typedef std::mt19937	RandomEngine;

	ADecor() = default;
	virtual ~ADecor() = default;

	virtual void			update(sf::Time frameTime,
								   DecorBuilder& manager,
								   ABiome& biome) = 0;

	void					setPosition(sf::Vector2f const& pos);
	sf::Vector2f const&		getPosition()const;
private:
	sf::Vector2f	m_position;
};

#endif
