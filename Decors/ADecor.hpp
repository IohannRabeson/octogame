/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADecor.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/08 02:40:40 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/17 15:33:39 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADECOR_HPP
# define ADECOR_HPP
# include "AGameObject.hpp"

# include <VertexBuilder.hpp>

# include <SFML/System/Time.hpp>
# include <SFML/System/Vector2.hpp>

class ABiome;

class ADecor : public AGameObject<GameObjectType::Decor>
{
public:
	virtual ~ADecor();

	virtual void			setup(ABiome& biome) = 0;

	virtual void			update(sf::Time frameTime,
								   octo::VertexBuilder& builder,
								   ABiome& biome) = 0;

	inline virtual bool		isDisabledIfOutOfScreen()const
	{
		return (true);
	}

	virtual bool			dieOutOfScreen()
	{
		return true;
	}

	void					setPosition(sf::Vector2f const& pos);
	sf::Vector2f const&		getPosition()const;
private:
	sf::Vector2f	m_position;
};

#endif
