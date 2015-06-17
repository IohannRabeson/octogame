/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lightning.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/12 17:23:00 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/13 17:45:01 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHTNING_HPP
# define LIGHTNING_HPP
# include "ADecor.hpp"

# include <Math.hpp>

# include <list>

class Segment
{
public:
	Segment()
	{
	}

	Segment(sf::Vector2f const& p0, sf::Vector2f const& p1) :
		m_p0(p0),
		m_p1(p1)
	{
	}

	Segment(Segment const&) = default;
	Segment(Segment&&) = default;

	Segment& operator = (Segment const&) = default;

	sf::Vector2f	direction()const
	{
		sf::Vector2f	diff = m_p1 - m_p0;

		octo::normalize(diff);
		return (diff);
	}

	sf::Vector2f	middle()const
	{
		return (sf::Vector2f((m_p0.x + m_p1.x) / 2.f, (m_p0.y + m_p1.y) / 2.f));
	}

	sf::Vector2f const&	p0()const
	{
		return (m_p0);
	}

	sf::Vector2f const&	p1()const
	{
		return (m_p1);
	}
private:
	sf::Vector2f	m_p0;
	sf::Vector2f	m_p1;
};

class Lightning : public ADecor
{
public:
	virtual void	setup(ABiome& biome);
	virtual void	update(sf::Time frameTime, DecorBuilder& builder, ABiome& biome);
private:
	void			createSegment(Segment const& segment, DecorBuilder& builder)const;
	void			fractalizeSegments(std::list<Segment>& segments, std::size_t count);
private:
	std::list<Segment>						m_segments;
	std::mt19937							m_engine{3294};
	std::uniform_real_distribution<float>	m_dist{-1.f, 1.f};
};

#endif
